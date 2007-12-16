# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

import logging

class Task(object):
	def __init__(self, **options):
		self.logger = logging.getLogger(self.__class__.__module__ + '.' + self.__class__.__name__)
		self.logger.setLevel(logging.INFO)
	
	def execute(self, environment):
		raise NotImplementedError('Subclasses must implement Task.execute()')

class PathTask(Task):
	def __init__(self, path):
		Task.__init__(self)
		
		self.path = path
