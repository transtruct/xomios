# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

import logging
import sys
from optparse import OptionParser

class Executor(object):
	def __init__(self, environments):
		self.environments = environments
	
	def execute(self):
		raise NotImplementedError('Subclasses must implement Executor.execute()')

class ConsoleExecutor(Executor):
	def execute(self):
		console = logging.StreamHandler(sys.stdout)
		console.setLevel(logging.INFO)
		console.setFormatter(logging.Formatter('%(asctime)s %(levelname)-8s %(name)s: %(message)s', '%H:%M:%S'))
		logging.getLogger().addHandler(console)
		
		parser = OptionParser(usage='usage: %prog [options] target...', version='%prog 1.0')
		parser.add_option('-D', action='callback', callback=self.__parse_macro_argument, callback_args=(self,), type='string',
			help='assign the macro NAME to VALUE', metavar='NAME=VALUE')
		parser.add_option('-e', '--environment', help='use the environment ENVIRONMENT', metavar='ENVIRONMENT', dest='environment')
		
		(options, targets) = parser.parse_args()
		
		if len(self.environments) != 1 and not options.environment:
			parser.error('An environment must be specified')
		
		try:
			environment = options.environment and self.environments[options.environment] or self.environments.values()[0]
		except KeyError:
			parser.error('Invalid environment specified')
		
		environment.execute(targets)
	
	@staticmethod
	def __parse_macro_argument(option, string, passed_value, parser, self):
		try:
			(name, value) = passed_value.split('=', 1)
		except ValueError:
			name = passed_value
			value = True
		
		self.macros[name] = value
