# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

class Selector(object):
	def __init__(self, options={}, default=None):
		self.options = options
		self.default = default
	
	def select(self, value):
		if value in self.options:
			return self.options[value]
		elif self.default:
			return self.default
		else:
			raise KeyError('No appropriate option for "%s" found' % value)
