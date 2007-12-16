# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.task.core import PathTask

class CompileEnvironment(object):
	def __init__(self):
		self.options = {}
		self.files = []
	
	def get_arguments(self):
		raise NotImplementedError('Subclasses must implement CompileEnvironment.get_arguments()')
	
	def set_options(self, options):
		for name, value in options.iteritems():
			if self.options.has_key(name):
				self.options[name] = value
	
	def set_files(self, files):
		self.files += files

class CompileTask(PathTask):
	pass