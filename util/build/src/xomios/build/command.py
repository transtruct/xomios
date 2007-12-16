# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from subprocess import Popen

class Command(object):
	def __init__(self, environment, command):
		self.environment = environment
		self.command = command
		self.arguments = []
	
	def execute(self):
		self.environment.stream.write('%s %s\n' % (self.command, ' '.join(self.arguments)))
		process = Popen([self.command] + self.arguments, stdout=self.environment.stream)
		process.wait()
		return process.returncode
	
	@staticmethod
	def escape(string):
		return string.replace(' ', '\\ ')
