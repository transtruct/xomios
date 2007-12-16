# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.task.core import Task

class CompileTask(Task):
	def __init__(self, path, **options):
		Task.__init__(self, **options)
		self.path = path
