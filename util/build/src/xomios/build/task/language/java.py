# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.task.compile import CompileTask, CompileEnvironment
from xomios.build.command import Command
from subprocess import Popen

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
	
	def __init__(self):
		CompileEnvironment.__init__(self)
		self.options['debug'] = self.DEBUG_DEFAULT
		self.options['no_warning'] = False
		self.options['verbose'] = False
		self.options['deprecation'] = False
		self.options['class_path'] = []
		self.options['source_path'] = []
		self.options['boot_class_path'] = []
		self.options['extension_directories'] = []
		self.options['endorsed_directories'] = []
		self.options['annotation_processing'] = self.PROC_DEFAULT
		self.options['annotation_processors'] = []
		self.options['annotation_processor_path'] = []
		self.options['class_destination'] = None
		self.options['source_destination'] = None
		self.options['implicit'] = self.IMPLICIT_DEFAULT
		self.options['encoding'] = None
		self.options['compatibility'] = None
		self.options['target'] = None
		self.options['annotation_options'] = {}
		self.options['runtime_options'] = []
		self.files = []

class SunJava6CompileEnvironment(JavaCompileEnvironment):
	executable = 'javac'
	
	def get_arguments(self):
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
			argument_list.append(Command.escape(':'.join(self.options['class_path'])))
		if len(self.options['source_path']) > 0:
			argument_list.append('-sourcepath')
			argument_list.append(Command.escape(':'.join(self.options['source_path'])))
		if len(self.options['boot_class_path']) > 0:
			argument_list.append('-bootclasspath')
			argument_list.append(Command.escape(':'.join(self.options['boot_class_path'])))
		if len(self.options['extension_directories']) > 0:
			argument_list.append('-extdirs')
			argument_list.append(Command.escape(':'.join(self.options['extension_directories'])))
		if len(self.options['endorsed_directories']) > 0:
			argument_list.append('-endorseddirs')
			argument_list.append(Command.escape(':'.join(self.options['endorsed_directories'])))
		if self.options['annotation_processing'] == self.PROC_NONE:
			argument_list.append('-proc:none')
		elif self.options['annotation_processing'] == self.PROC_ONLY:
			argument_list.append('-proc:only')
		if len(self.options['annotation_processors']) > 0:
			argument_list.append('-processor')
			argument_list.append(Command.escape(','.join(self.options['annotation_processors'])))
		if len(self.options['annotation_processor_path']) > 0:
			argument_list.append('-bootclasspath')
			argument_list.append(Command.escape(':'.join(self.options['annotation_processor_path'])))
		if self.options['class_destination']:
			argument_list.append('-d')
			argument_list.append(Command.escape(str(self.options['class_destination'])))
		if self.options['source_destination']:
			argument_list.append('-s')
			argument_list.append(Command.escape(str(self.options['source_destination'])))
		if self.options['implicit'] == self.IMPLICIT_NONE:
			argument_list.append('-implicit:none')
		if self.options['implicit'] == self.IMPLICIT_CLASS:
			argument_list.append('-implicit:class')
		if self.options['encoding']:
			argument_list.append('-encoding')
			argument_list.append(Command.escape(self.options['encoding']))
		if self.options['compatibility']:
			argument_list.append('-source')
			argument_list.append(Command.escape(self.options['encoding']))
		if self.options['target']:
			argument_list.append('-target')
			argument_list.append(Command.escape(self.options['target']))
		for name, value in self.options['annotation_options'].iteritems():
			if value:
				argument_list.append('-A%s=%s' % (Command.escape(name), Command.escape(value)))
			else:
				argument_list.append('-A%s' % Command.escape(name))
		for option in self.options['runtime_options']:
			argument_list.append('-J%s' % Command.escape(name))
		
		for file in self.files:
			argument_list.append(str(file))
		
		return argument_list

class JavaCompileTask(CompileTask):
	def __init__(self, path, **options):
		CompileTask.__init__(self, path)
		self.environment = SunJava6CompileEnvironment()
		self.environment.set_options(options)
		self.environment.set_files(self.path.get_files())
	
	def execute(self, environment):
		javac = Command(environment, self.environment.executable)
		javac.arguments = self.environment.get_arguments()
		
		self.logger.info('building java sources')
		try:
			if javac.execute() < 0:
				self.logger.error('java build failed')
				return False
		except OSError, ose:
			self.logger.error('java build failed: %s' % ose[1])
			return False
		return True
