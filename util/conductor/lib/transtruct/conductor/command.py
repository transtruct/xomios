# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from subprocess import Popen

class Command(object):
	def __init__(self, environment, command, arguments=[]):
		self.environment = environment
		self.command = command
		self.arguments = arguments
	
	def execute(self):
		self.environment.stream.write('%s %s\n' % (self.command, ' '.join(self.arguments)))
		process = Popen([self.command] + self.arguments, stdout=self.environment.stream)
		process.wait()
		return process.returncode

def escape(string):
	return str(string).replace(' ', '\\ ')
