# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from transtruct.conductor.task.compile import CompileTask, CompileEnvironment, CompileError
from transtruct.conductor.command import Command, escape
from transtruct.conductor.location import GlobMatcher, StaticMapper, RegexMapper, TimestampDifferentiator

from subprocess import Popen

import re
import os

class JavaCompileEnvironment(CompileEnvironment):
	DEBUG_DEFAULT = 0
	DEBUG_NONE = 1
	DEBUG_LINES = 2
	DEBUG_VARS = 4
	DEBUG_SOURCE = 8
	DEBUG_ALL = DEBUG_LINES | DEBUG_VARS | DEBUG_SOURCE
	
	PROC_DEFAULT = 0
	PROC_NONE = 1
	PROC_ONLY = 2
	
	IMPLICIT_DEFAULT = 0
	IMPLICIT_NONE = 1
	IMPLICIT_CLASS = 2
	
	DEFAULT_OPTIONS = {
		'debug': DEBUG_DEFAULT,
		'no_warning': False,
		'verbose': False,
		'deprecation': False,
		'class_path': [],
		'source_path': [],
		'boot_class_path': [],
		'extension_directories': [],
		'endorsed_directories': [],
		'annotation_processing': PROC_DEFAULT,
		'annotation_processors': [],
		'annotation_processor_path': [],
		'class_destination': None,
		'source_destination': None,
		'implicit': IMPLICIT_DEFAULT,
		'encoding': None,
		'compatibility': None,
		'target': None,
		'annotation_options': {},
		'runtime_options': []
	}

class SunJava6CompileEnvironment(JavaCompileEnvironment):
	executable = 'javac'
	
	def execute(self, task, build_environment):
		if len(self.container) == 0:
			task.logger.info('nothing to be done')
			return 
		
		argument_list = []
		
		if self.options['debug'] & self.DEBUG_ALL:
			argument_list.append('-g')
		elif self.options['debug'] == self.DEBUG_NONE:
			argument_list.append('-g:none')
		else:
			if self.options['debug'] & self.DEBUG_LINES:
				argument_list.append('-g:lines')
			if self.options['debug'] & self.DEBUG_VARS:
				argument_list.append('-g:vars')
			if self.options['debug'] & self.DEBUG_SOURCE:
				argument_list.append('-g:source')
		if self.options['no_warning']:
			argument_list.append('-nowarn')
		if self.options['verbose']:
			argument_list.append('-verbose')
		if self.options['deprecation']:
			argument_list.append('-deprecation')
		if len(self.options['class_path']) > 0:
			argument_list.append('-classpath')
			argument_list.append(escape(':'.join([str(class_path) for class_path in self.options['class_path']])))
		if len(self.options['source_path']) > 0:
			argument_list.append('-sourcepath')
			argument_list.append(escape(':'.join([str(source_path) for source_path in self.options['source_path']])))
		if len(self.options['boot_class_path']) > 0:
			argument_list.append('-bootclasspath')
			argument_list.append(escape(':'.join([str(boot_class_path) for boot_class_path in self.options['boot_class_path']])))
		if len(self.options['extension_directories']) > 0:
			argument_list.append('-extdirs')
			argument_list.append(escape(':'.join([str(extension_directory) for extension_directory in self.options['extension_directories']])))
		if len(self.options['endorsed_directories']) > 0:
			argument_list.append('-endorseddirs')
			argument_list.append(escape(':'.join([str(endorsed_directory) for endorsed_directory in self.options['endorsed_directories']])))
		if self.options['annotation_processing'] == self.PROC_NONE:
			argument_list.append('-proc:none')
		elif self.options['annotation_processing'] == self.PROC_ONLY:
			argument_list.append('-proc:only')
		if len(self.options['annotation_processors']) > 0:
			argument_list.append('-processor')
			argument_list.append(escape(','.join(self.options['annotation_processors'])))
		if len(self.options['annotation_processor_path']) > 0:
			argument_list.append('-bootclasspath')
			argument_list.append(escape(':'.join([str(annotation_processor_path) for annotation_processor_path in self.options['annotation_processor_path']])))
		if self.options['class_destination']:
			argument_list.append('-d')
			argument_list.append(escape(str(self.options['class_destination'])))
		if self.options['source_destination']:
			argument_list.append('-s')
			argument_list.append(escape(str(self.options['source_destination'])))
		if self.options['implicit'] == self.IMPLICIT_NONE:
			argument_list.append('-implicit:none')
		if self.options['implicit'] == self.IMPLICIT_CLASS:
			argument_list.append('-implicit:class')
		if self.options['encoding']:
			argument_list.append('-encoding')
			argument_list.append(escape(self.options['encoding']))
		if self.options['compatibility']:
			argument_list.append('-source')
			argument_list.append(escape(self.options['encoding']))
		if self.options['target']:
			argument_list.append('-target')
			argument_list.append(escape(self.options['target']))
		for name, value in self.options['annotation_options'].iteritems():
			if value and value != True:
				argument_list.append('-A%s=%s' % (escape(name), escape(value)))
			elif value == True:
				argument_list.append('-A%s' % escape(name))
		argument_list.extend(['-J%s' % option for option in self.options['runtime_options']])
		
		argument_list.extend([str(location) for location in self.container])
		
		javac = Command(build_environment, self.executable, argument_list)
		try:
			if javac.execute() != 0:
				raise CompileError('Java compiler execution failed')
		except OSError, ose:
			raise CompileError('Java compiler execution failed', ose)

class CompileJavaTask(CompileTask):
	def __init__(self, container, matcher=GlobMatcher(r'*.java'), differentiator=TimestampDifferentiator(), **options):
		CompileTask.__init__(self, container)
		self.compile_environment_options = options
		self.differentiator = differentiator
		self.matcher = matcher
	
	def execute(self, environment):
		# We hold off on this until execute() since some files might have been
		# created since instantiation.
		compile_environment = SunJava6CompileEnvironment()
		compile_environment.options = self.compile_environment_options
		
		# Figure out which files we need to compile.
		mapper = StaticMapper()
		
		# The contents of each base will be mapped to the class_destination
		# option to the environment (defaults to None, i.e., this directory).
		# This isn't perfect, but the worst scenario is that every class
		# specified ends up being compiled every time. And people should fix
		# their build scripts to make this work, since parsing each Java file
		# for the 'package' statement is too much of a pain in the ass.
		class_destination = compile_environment.options['class_destination'] and compile_environment.options['class_destination'] or Directory('.')
		for base in self.container.bases:
			replacements = RegexMapper(r'^%s%s(.+)\.[^\.]+$' % (base, os.sep), r'%s%s\1.class' % (class_destination, os.sep)).map(self.matcher.filter(base))
			for location, replacement in replacements.iteritems():
				mapper.replacements[location] = replacement
		
		self.differentiator.mappers.append(mapper)
		compile_environment.container = self.differentiator.filter(self.matcher.filter(self.container))
		
		self.logger.info('building Java sources')
		try:
			compile_environment.execute(self, environment)
		except CompileError, ce:
			self.logger.error(ce)
			return False
		return True
