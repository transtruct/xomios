# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.task.core import Task

class PrintTask(Task):
	def __init__(self, message):
		Task.__init__(self)
		
		self.message = message
	
	def execute(self, environment):
		self.logger.info(self.message)
		
		return True
