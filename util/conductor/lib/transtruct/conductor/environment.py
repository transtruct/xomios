# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

import sys
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
	environments = {}
	
	def __init__(self, name):
		if name in Environment.environments:
			self = Environment.environments[name]
			return
		
		self.name = name
		self.stream = sys.stdout # TEMPORARY
		self.managed_targets = {}
		self.logger = logging.getLogger('project')
		self.logger.setLevel(logging.INFO)
		
		Environment.environments[name] = self
	
	def add_target(self, name, target, hidden=False, depends=()):
		self.managed_targets[name.lower()] = ManagedTargetProxy(target, self, hidden=hidden, depends=depends)
	
	def has_target(self, name):
		return name in self.managed_targets
	
	@property
	def available_targets(self):
		return self.managed_targets.keys()
	
	__execute_target_lock = RLock()
	def execute_target(self, name):
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
				self.execute_target(depend)
			
			self.logger.info('%s:', name)
			target.target.execute(self)
			
			target.status = ManagedTargetProxy.STATUS_EXECUTED
		except DependencyError, de:
			self.logger.error('Dependency error: %s' % de.message)
			target.status = ManagedTargetProxy.STATUS_WAIT
		self.__execute_target_lock.release()
	
	def execute(self, targets):
		self.logger.info('executing targets: %s' % ', '.join(targets))
		try:			
			for target_name in targets:
				self.execute_target(target_name)
			self.logger.info('execution complete')
		except TargetExecutionError, tse:
			self.stream.write('\n')
			self.stream.write('BUILD FAILED\n')
			self.stream.write('%s\n' % tse)

class TargetProxy(object):
	def __init__(self, target):
		self.target = target

class ManagedTargetProxy(TargetProxy):
	STATUS_WAIT = 1
	STATUS_EXECUTING = 2
	STATUS_EXECUTED = 3
	 
	def __init__(self, target, project, hidden=False, depends=()):
		TargetProxy.__init__(self, target)
		self.project = project
		self.hidden = hidden
		if not hasattr(depends, '__iter__') or isinstance(depends, basestring):
			depends = (depends,)
		self.depends = tuple(depends)
		self.status = self.STATUS_WAIT

class Target(object):
	def __init__(self):
		self.managed_tasks = []
	
	def add_task(self, task, ignore_errors=False):
		self.managed_tasks.append(ManagedTaskProxy(task, ignore_errors=ignore_errors))
	
	@property
	def tasks(self):
		return self.managed_tasks
	
	def execute(self, environment):
		for task in self.managed_tasks:
			if not task.task.execute(environment):
				if task.ignore_errors:
					environment.logger.info('ignoring errors')
				else:
					raise TargetExecutionError('Target execution failed')

class TaskProxy(object):
	def __init__(self, task):
		self.task = task

class ManagedTaskProxy(TaskProxy):
	def __init__(self, task, ignore_errors=False):
		TaskProxy.__init__(self, task)
		self.ignore_errors = ignore_errors
