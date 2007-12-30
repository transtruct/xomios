# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from transtruct.conductor.task.core import Task

class PrintTask(Task):
	def __init__(self, message):
		Task.__init__(self)
		self.message = message
	
	def execute(self, environment):
		self.logger.info(self.message)
		return True
