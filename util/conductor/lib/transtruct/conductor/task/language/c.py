# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from transtruct.conductor.task.compile import CompileTask, CompileEnvironment, CompileError
from transtruct.conductor.command import Command, escape
from transtruct.conductor.location import GlobMatcher, RegexMapper, TimestampDifferentiator

import copy

class CCompileEnvironment(CompileEnvironment):
	DEFAULT_OPTIONS = {
		'output': RegexMapper(r'\.c$', r'.o'),
		'include': [],
		'pedantic': False,
		'pic': False,
		'macro_assignments': {},
		'warnings_as_errors': False
	}

class GccCompileEnvironment(CCompileEnvironment):
	executable = 'gcc'
	
	def execute(self, task, build_environment):
		if len(self.container) == 0:
			task.logger.info('nothing to be done')
			return
		
		argument_list = []
		argument_list.append('-c')
		
		if self.options['pedantic']:
			argument_list.append('-pedantic')
		if self.options['warnings_as_errors']:
			argument_list.append('-Werror')
		if self.options['pic']:
			argument_list.append('-fPIC')
		for name, value in self.options['macro_assignments'].iteritems():
			if value and value != True:
				argument_list.append('-D%s=%s' % (escape(name), escape(value)))
			elif value == True:
				argument_list.append('-D%s' % escape(name))
		argument_list.extend(['-I%s' % str(include) for include in self.options['include']])
		
		# For C builds, compile each file independently.
		for location, output in self.options['output'].map(self.container).iteritems():
			location_argument_list = copy.copy(argument_list)
			
			location_argument_list.append('-o')
			location_argument_list.append(escape(str(output)))
			
			# hax
			#location_argument_list.extend('-pedantic -std=c99 -Wall -Wundef -Wmissing-prototypes -Wmissing-declarations -Werror -fPIC -D _LARGEFILE_SOURCE -D _FILE_OFFSET_BITS=64 -D _XOPEN_SOURCE=600'.split())
			
			location_argument_list.append(escape(str(location)))
			
			cc = Command(build_environment, self.executable, location_argument_list)
			try:
				if cc.execute() != 0:
					raise CompileError('C compiler execution failed')
			except OSError, ose:
				raise CompileError('C compiler execution failed', ose)

class SunccCompileEnvironment(CCompileEnvironment):
	pass

class MsvcCompileEnvironment(CCompileEnvironment):
	pass

class CompileCTask(CompileTask):
	def __init__(self, container, matcher=GlobMatcher('*.c'), differentiator=TimestampDifferentiator(), **options):
		CompileTask.__init__(self, container)
		self.matcher = matcher
		self.differentiator = differentiator
		self.compile_environment_options = options
	
	def execute(self, environment):
		# We hold off on this until execute() since some files might have been
		# created since instantiation.
		compile_environment = GccCompileEnvironment()
		compile_environment.options = self.compile_environment_options
		
		self.differentiator.mappers.append(compile_environment.options['output'])
		compile_environment.container = self.differentiator.filter(self.matcher.filter(self.container))
		
		self.logger.info('building C sources')
		try:
			compile_environment.execute(self, environment)
		except CompileError, ce:
			self.logger.error(ce)
			return False
		return True
