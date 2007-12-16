# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.task.core import PathTask
import os
import shutil

class CreatePathTask(PathTask):
	def __init__(self, path, fail_if_exists=False):
		PathTask.__init__(self, path)
		self.fail_if_exists = fail_if_exists

class CreateDirectoryTask(CreatePathTask):
	def execute(self, environment):
		try:
			path = self.path.get_path()
			
			if self.path.exists():
				if not self.path.is_directory():
					self.logger.error('%s: exists and is not a directory', path.path)
					return False
				elif self.fail_if_exists:
					self.logger.error('%s: directory exists', path.path)
					return False
				else:
					self.logger.warning('%s: directory exists (ignoring)', path.path)
					return True
			
			self.logger.info('creating directory %s', path.path)
			os.makedirs(path.path)
		except OSError, ose:
			self.logger.error('%s: %s', path.path, ose[1])
			return False
		return True

class CreateFileTask(CreatePathTask):
	def __init__(self, path, fail_if_exists=False, contents=None):
		CreatePathTask.__init__(self, path, fail_if_exists=fail_if_exists)
		self.contents = contents
	
	def execute(self, environment):
		try:
			path = self.path.get_path()
			
			if self.path.exists():
				if not self.path.is_file():
					self.logger.error('%s: exists and is not a file', path.path)
					return False
				elif self.fail_if_exists:
					self.logger.error('%s: file exists', path.path)
					return False
				else:
					self.logger.warning('%s: file exists (ignoring)', path.path)
					return True
			
			self.logger.info('creating file %s', path.path)
			f = open(path.path, 'w')
			if self.contents:
				f.write(self.contents)
			f.close()
		except OSError, ose:
			self.logger.error('%s: %s', path.path, ose[1])
			return False
		return True

class RemovePathTask(PathTask):
	def execute(self, environment):
		try:
			path = self.path.get_path()
			self.logger.info('removing %s', path.path)
			if path.is_directory():
				shutil.rmtree(path.path)
			else:
				os.remove(path.path)
		except OSError, ose:
			self.logger.error('%s: %s', path.path, ose[1])
			return False
		return True
