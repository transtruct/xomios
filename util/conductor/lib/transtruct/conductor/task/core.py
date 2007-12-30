# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

import logging

class Task(object):
	def __init__(self, **options):
		self.logger = logging.getLogger('%s.%s' % (self.__class__.__module__, self.__class__.__name__))
		self.logger.setLevel(logging.INFO)
	
	def execute(self, environment):
		raise NotImplementedError('Subclasses must implement Task.execute()')

class LocationTask(Task):
	def __init__(self, location):
		Task.__init__(self)
		
		self.location = location

class ContainerTask(Task):
	def __init__(self, container):
		Task.__init__(self)
		
		self.container = container

class Environment(object):
	DEFAULT_OPTIONS = {}
	
	def __init__(self):
		self.__stored_options = {}
	
	def __get_options(self):
		return self.__stored_options
	
	def __set_options(self, options):
		self.__stored_options = self.DEFAULT_OPTIONS.copy()
		self.__stored_options.update(options)
	
	options = property(__get_options, __set_options)
	
	def execute(self, task, build_environment):
		raise NotImplementedError('Subclasses must implement Environment.execute()')

class ContainerEnvironment(Environment):
	def __init__(self):
		self.container = None

class EnvironmentError(Exception):
	def __init__(self, message, encapsulated_exception=None):
		Exception.__init__(self, message)
		self.encapsulated_exception = encapsulated_exception
	
	def __str__(self):
		if self.encapsulated_exception:
			return '%s: %s' % (Exception.__str__(self), str(self.encapsulated_exception))
		return Exception.__str__(self)
