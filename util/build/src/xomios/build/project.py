# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.task.io import PrintTask
import sys
from optparse import OptionParser, TitledHelpFormatter
from threading import RLock
import logging

class BuildError(Exception):
	pass

class TargetResolutionError(BuildError):
	pass

class TargetExecutionError(BuildError):
	pass

class DependencyError(BuildError):
	pass

class Environment(object):
	def __init__(self, project):
		self.project = project
		self.macros = {}
		self.targets = []
	
	def execute(self, default=None):
		targets = []
		if len(self.targets) != 0:
			targets = self.targets
		elif default is not None:
			targets = [default]
		else:
			raise TargetResolutionError('No targets specified for execution')
		
		self.project.logger.info('executing targets: %s' % ', '.join(targets))
		self.project.execute(targets, self)
		self.project.logger.info('execution complete')

class ConsoleEnvironment(Environment):
	def __init__(self, project):
		Environment.__init__(self, project)
		self.stream = sys.stdout
		
		console = logging.StreamHandler(self.stream)
		console.setLevel(logging.INFO)
		console.setFormatter(logging.Formatter('%(asctime)s %(levelname)-8s %(name)s: %(message)s', '%H:%M:%S'))
		logging.getLogger().addHandler(console)
		
		parser = OptionParser(usage='usage: %prog [options] target...', version='%prog 1.0')
		parser.add_option('-D', action='callback', callback=self.__parse_macro_argument, callback_args=(self,), type='string',
						help='assign the macro NAME to VALUE', metavar='NAME=VALUE')
		
		(options, self.targets) = parser.parse_args()
	
	@staticmethod
	def __parse_macro_argument(option, string, passed_value, parser, self):
		try:
			(name, value) = passed_value.split('=', 1)
		except ValueError:
			name = passed_value
			value = True
		
		self.macros[name] = value

class Project(object):	
	def __init__(self, name=None):
		self.name = name
		self.managed_targets = {}
		self.logger = logging.getLogger('project')
		self.logger.setLevel(logging.INFO)
	
	def add_target(self, name, target, hidden=False, depends=[]):
		self.managed_targets[name.lower()] = ManagedTargetProxy(target, self, hidden=hidden, depends=depends)
	
	def has_target(self, name):
		return self.managed_targets.has_key(name)
	
	def get_available_targets(self):
		return self.managed_targets.keys()
	
	__execute_target_lock = RLock()
	def execute_target(self, name, environment):
		if not self.has_target(name):
			raise TargetResolutionError('Could not find target "%s"' % name)
		
		self.__execute_target_lock.acquire()
		target = self.managed_targets[name]
		
		try:
			if target.status == ManagedTargetProxy.STATUS_EXECUTED:
				# Target has already been executed in this project instance; simply
				# return
				return
			elif target.status == ManagedTargetProxy.STATUS_EXECUTING:
				# Target is being executed right now in a higher level of
				# dependency, meaning we have a circular dependency -- break here.
				raise DependencyError('Circular dependency; check target specification')
			elif target.status == ManagedTargetProxy.STATUS_WAIT:
				target.status = ManagedTargetProxy.STATUS_EXECUTING
			
			for depend in target.depends:
				self.execute_target(depend, environment)
			
			self.logger.info('%s:', name)
			target.target.execute(environment)
			
			target.status = ManagedTargetProxy.STATUS_EXECUTED
		except DependencyError, de:
			self.logger.error('Dependency error: %s' % de.message)
			target.status = ManagedTargetProxy.STATUS_WAIT
		self.__execute_target_lock.release()
	
	def execute(self, targets, environment):
		for target_name in targets:
			self.execute_target(target_name, environment)

class TargetProxy(object):
	def __init__(self, target):
		self.target = target
	pass

class ManagedTargetProxy(TargetProxy):
	STATUS_WAIT = 1
	STATUS_EXECUTING = 2
	STATUS_EXECUTED = 3
	 
	def __init__(self, target, project, hidden=False, depends=()):
		TargetProxy.__init__(self, target)
		self.project = project
		self.hidden = hidden
		if isinstance(depends, str):
			depends = (depends,)
		self.depends = tuple(depends)
		self.status = self.STATUS_WAIT

class Target(object):
	def __init__(self, ignore_errors=False):
		self.tasks = []
		self.ignore_errors = ignore_errors
	
	def add_task(self, task):
		task.target = self
		self.tasks.append(task)
	
	def add_print(self, text):
		self.add_task(PrintTask(text))
	
	def execute(self, environment):
		for task in self.tasks:
			if not task.execute(environment):
				if self.ignore_errors:
					environment.project.logger.warning('(ignoring errors)')
				else:
					raise TargetExecutionError('Target execution failed')

def selector(value, options, default=None):
	if options.has_key(value):
		return options[value]
	elif default is not None:
		return default
	else:
		raise ValueError('No appropriate option for "%s" found' % value)
