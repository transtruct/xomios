# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from transtruct.conductor.task.core import ContainerTask, ContainerEnvironment, EnvironmentError
from transtruct.conductor.location import GlobMatcher, TimestampDifferentiator, StaticMapper, Aggregate
from transtruct.conductor.command import Command, escape

class LinkError(EnvironmentError):
	pass

class LinkEnvironment(ContainerEnvironment):
	DEFAULT_OPTIONS = {
		'shared': False,
		'libraries': ['c'],
		'output': None
	}

class LdLinkEnvironment(LinkEnvironment):
	def __init__(self):
		LinkEnvironment.__init__(self)
		self.DEFAULT_OPTIONS.update({'output': 'a.out'})
	
	executable = 'ld'
	
	def execute(self, task, build_environment):
		if len(self.container) == 0:
			task.logger.info('nothing to be done')
			return
		
		argument_list = []
		
		if self.options['shared']:
			argument_list.append('-Bshareable')
		if self.options['output']:
			argument_list.append('-o')
			argument_list.append(escape(str(self.options['output'])))
		argument_list.extend(['-l%s' % escape(str(library)) for library in self.options['libraries']])
		
		argument_list.extend([str(location) for location in self.container])
		
		ld = Command(build_environment, self.executable, argument_list)
		try:
			if ld.execute() != 0:
				raise LinkError('Linker execution failed')
		except OSError, ose:
			raise LinkError('Linker execution failed', ose)

class LinkTask(ContainerTask):
	def __init__(self, container, matcher=GlobMatcher('*.o'), differentiator=TimestampDifferentiator(), **options):
		ContainerTask.__init__(self, container)
		self.matcher = matcher
		self.differentiator = differentiator
		self.link_environment_options = options
	
	def execute(self, environment):
		link_environment = LdLinkEnvironment()
		link_environment.options = self.link_environment_options
		
		filtered = self.matcher.filter(self.container)
		output = link_environment.options['output'] and link_environment.options['output'] or None
		mapper = StaticMapper(dict([(location, output) for location in filtered]))
		
		self.differentiator.mappers.append(mapper)
		if len(self.differentiator.filter(filtered)) != 0:
			# Regenerate link
			link_environment.container = filtered
		else:
			link_environment.container = Aggregate()
		
		self.logger.info('linking sources')
		try:
			link_environment.execute(self, environment)
		except LinkError, le:
			self.logger.error(le)
			return False
		return True
