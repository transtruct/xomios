# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from transtruct.conductor.task.core import ContainerTask
from transtruct.conductor.location import File, Directory

import os
import shutil

class CreateContainerTask(ContainerTask):
	HANDLERS = {
		File: lambda location: open(str(location), 'w').close(),
		Directory: lambda location: os.makedirs(str(location))
	}
	
	def execute(self, environment):
		for location in self.container.bases:
			self.logger.info('creating location "%s"', location)
			if type(location) in self.HANDLERS:
				try:
					self.HANDLERS[type(location)](location)
				except OSError, ose:
					self.logger.error('%s: %s', location, ose[1])
					return False
			else:
				raise NotImplementedError('No handler found to create location "%s" of type %s.%s' % (location, location.__class__.__module__, location.__class__.__name__))
		return True

class RemoveContainerTask(ContainerTask):
	HANDLERS = {
		File: lambda location: os.remove(str(location)),
		Directory: lambda location: shutil.rmtree(str(location))
	}
	
	def execute(self, environment):
		for location in self.container.bases:
			self.logger.info('removing location "%s"', location)
			if type(location) in self.HANDLERS:
				try:
					self.HANDLERS[type(location)](location)
				except OSError, ose:
					self.logger.error('%s: %s', location, ose[1])
					return False
			else:
				raise NotImplementedError('No handler found to remove location "%s" of type %s.%s' % (location, location.__class__.__module__, location.__class__.__name__))
		return True
