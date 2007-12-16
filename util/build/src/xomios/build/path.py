# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

import os
import re

class Container(object):
	def get_directories(self):
		raise NotImplementedError('Subclasses must implement Container.get_directories()')
	
	def get_directory(self):
		raise NotImplementedError('Subclasses must implement Container.get_directory()')
	
	def get_files(self):
		raise NotImplementedError('Subclasses must implement Container.get_files()')
	
	def get_file(self):
		raise NotImplementedError('Subclasses must implement Container.get_file()')
	
	def get_paths(self):
		raise NotImplementedError('Subclasses must implement Container.get_paths()')
	
	def get_path(self):
		raise NotImplementedError('Subclasses must implement Container.get_path()')

class Path(Container):
	def __init__(self, path):
		Container.__init__(self)
		self.path = path
	
	def __add__(self, path):
		if not isinstance(path, Path):
			path = Path(path)
		
		return Path(self.path + path.path)
	
	def exists(self):
		return os.path.exists(self.path)
	
	def is_directory(self):
		return self.exists() and os.path.isdir(self.path)
	
	def is_file(self):
		return self.exists() and os.path.isfile(self.path)
	
	def is_link(self):
		return self.exists() and os.path.islink(self.path)
	
	def get_directories(self):
		if self.is_directory():
			result = []
			for root, directories, files in os.walk(self.path):
				result.append(Path(root))
			return result
		elif self.exists():
			return Path(os.path.dirname(self.path))
		else:
			return []
	
	def get_directory(self):
		if self.is_directory():
			return self
		elif self.exists():
			return Path(os.path.dirname(self.path))
		else:
			return None
	
	def get_files(self):
		if self.is_directory():
			result = []
			for root, directories, files in os.walk(self.path):
				for file in files:
					result.append(Path(os.path.join(root, file)))
			return result
		else:
			return [self]
	
	def get_file(self):
		if self.is_directory():
			for root, directories, files in os.walk(self.path):
				if len(files) > 0:
					return files[0]
		else:
			return self
	
	def get_paths(self):
		if self.is_directory():
			result = []
			for root, directories, files in os.walk(self.path):
				result.append(Path(root))
				for file in files:
					result.append(Path(os.path.join(root, file)))
			return result
		else:
			return [self]
	
	def get_path(self):
		return self
	
	def __str__(self):
		return self.path

class Matcher(Container):
	def __init__(self, pattern, flags=0):
		Container.__init__(self)
		self.pattern = re.compile(pattern, flags)
		self.paths = []
	
	def apply(self, paths):
		self.paths += list(paths)
		return self
	
	def get_directories(self):
		result = []
		for path in self.paths:
			for sub_path in path.get_directories():
				if self.pattern.search(sub_path.path):
					result.append(sub_path)
		return result
	
	def get_directory(self):
		for path in self.paths:
			for sub_path in path.get_directories():
				if sub_path.is_directory() and self.pattern.search(sub_path.path):
					return sub_path
	
	def get_files(self):
		result = []
		for path in self.paths:
			for sub_path in path.get_files():
				if self.pattern.search(sub_path.path):
					result.append(sub_path)
		return result
	
	def get_file(self):
		for path in self.paths:
			for sub_path in path.get_files():
				if self.pattern.search(sub_path.path):
					return sub_path
	
	def get_paths(self):
		result = []
		for path in self.paths:
			for sub_path in path.get_paths():
				if self.pattern.search(sub_path.path):
					result.append(sub_path)
		return result
	
	def get_path(self):
		for path in self.paths:
			for sub_path in path.get_paths():
				if self.pattern.search(sub_path.path):
					return sub_path
