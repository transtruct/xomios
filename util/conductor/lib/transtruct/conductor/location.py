# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

import os
import copy
import re
import fnmatch
import md5

class Location(object):
	""" Represents a filesystem location. """
	def __init__(self, *location):
		Container.__init__(self)
		self.location = os.path.join(*[str(component) for component in location])
	
	def exists(self):
		return os.path.exists(self.location)
	
	def __str__(self):
		return self.location
	
	def __eq__(self, other):
		if isinstance(other, Location):
			return self.location == other.location
		elif isinstance(other, basestring):
			return self.location == other
		return False
	
	def __hash__(self):
		return hash(self.location)

class Container(object):
	""" Represents any object that contains at least one Location. """
	contents = []
	bases = []
	
	def __iter__(self):
		for content in self.contents:
			yield content
	
	def __len__(self):
		return len(self.contents)
	
	def __getitem__(self, item):
		return self.contents[item]

class File(Location, Container):
	""" Represents a physical file on a filesystem. """
	def __init__(self, *location):
		Location.__init__(self, *location)
		Container.__init__(self)
		
		self.directory = os.path.dirname(self.location)
	
	@property
	def bases(self):
		return [self]
	
	@property
	def contents(self):
		return [self]

class Directory(Location, Container):
	""" Represents a physical directory on a filesystem. """
	def __init__(self, *location):
		Location.__init__(self, *location)
		Container.__init__(self)
	
	@property
	def bases(self):
		return [self]
	
	@property
	def contents(self):
		if self.exists():
			result = []
			for root, directories, files in os.walk(self.location):
				result.append(Directory(root))
				for file in files:
					result.append(File(os.path.join(root, file)))
			return result
		else:
			return [self]
	
	@property
	def entries(self):
		""" Retrieves a list of Files and Directories in the Directory without
		recursing. """
		return [os.path.isdir(path) and Directory(path) or File(path) for path in os.listdir(self.location)]

class Aggregate(Container):
	""" Represents a collection of Locations. """
	def __init__(self, locations=[]):
		Container.__init__(self)
		self.bases = locations
	
	@property
	def contents(self):
		result = []
		for location in self.bases:
			result.extend(location.contents)
		return result

class Mapper(object):
	""" Maps one Location to some other Location. """
	def map(self, container):
		raise NotImplementedError('Subclasses must implement Mapper.map()')

class EmptyMapper(Mapper):
	""" Returns a dictionary original Container with each element in a tuple. """
	def map(self, container):
		return dict([(location, location) for location in container.contents])

class RegexMapper(Mapper):
	""" Maps one Location to a different Location by performing a regular
	expression substitution on the original Location. """
	def __init__(self, pattern, replacement, flags=0):
		Mapper.__init__(self)
		self.pattern = re.compile(pattern, flags)
		self.replacement = replacement
	
	def map(self, container):
		result = {}
		for location in container.contents:
			# Copy everything to the result, it's all liable to be changed
			result[location] = copy.copy(location)
			result[location].location = self.pattern.sub(self.replacement, result[location].location)
		return result

class StaticMapper(Mapper):
	""" Maps one Location to a different Location by using a dictionary to test
	the Location's path. """
	def __init__(self, replacements={}):
		self.replacements = replacements
	
	def map(self, container):
		result = {}
		for location in container.contents:
			if location in self.replacements:
				result[location] = self.replacements[location]
			else:
				result[location] = location
		return result

class Filter(object):
	""" Reduces any Container to an Aggregate that matches given criteria. """
	def filter(self, container):
		raise NotImplementedError('Subclasses must implement Filter.filter()')

class Matcher(Filter):
	""" Reduces a Container to an Aggregate by matching the Container's
	contents against a given search pattern. """
	pass

class EmptyMatcher(Matcher):
	""" Returns the original Container as an Aggregate. """
	def filter(self, container):
		return Aggregate(container.contents)

class RegexMatcher(Matcher):
	""" Reduces a Container to an Aggregate by matching the Container's
	contents against a given regular expression. """
	def __init__(self, pattern, flags=0):
		Matcher.__init__(self)
		self.pattern = re.compile(pattern, flags)
	
	def filter(self, container):
		return Aggregate([location for location in container.contents if self.pattern.search(location.location)])

class GlobMatcher(Matcher):
	""" Reduces a Container to an Aggregate by matching the Container's
	contents against a given glob pattern. """
	def __init__(self, pattern):
		Matcher.__init__(self)
		self.pattern = pattern
	
	def filter(self, container):
		return Aggregate([location for location in container.contents if fnmatch.fnmatch(location.location, self.pattern)])

class Differentiator(Filter):
	""" Reduces a Container to an Aggregate by matching the Container's
	contents against any number of other Containers that correspond to it. """
	def __init__(self):
		Filter.__init__(self)
		
		# Mappers are guaranteed to work because differentiating between two
		# identical locations should be a no-op.
		self.mappers = []
	
	def _prepare_filterable(self, container):
		# Start with an empty location -> set.
		filterable = dict([(location, set()) for location in container.contents])
		
		for mapper in self.mappers:
			mapped = mapper.map(container)
			for location, value in mapped.iteritems():
				filterable[location].update(value)
		
		return filterable

class EmptyDifferentiator(Differentiator):
	""" Returns the original Container. """
	def filter(self, container):
		return Aggregate(container.contents)

class TimestampDifferentiator(Differentiator):
	""" Reduces a Container to an Aggregate by matching the Container's
	last-modified time to the last-modified time of any number of other
	Containers. """
	def __init__(self, granularity=1000):
		Differentiator.__init__(self)
		self.granularity = granularity
	
	def filter(self, container):
		result = []
		for location, comparisons in self._prepare_filterable(container).iteritems():
			try:
				location_modified = os.path.getmtime(str(location))
				for comparison in comparisons:
					if location_modified - os.path.getmtime(str(comparison)) > (self.granularity / 1000):
						result.append(location)
			except OSError, ose:
				# One of the files being compared against didn't exist, and
				# probably needs to be built.
				result.append(location)
		return Aggregate(result)
