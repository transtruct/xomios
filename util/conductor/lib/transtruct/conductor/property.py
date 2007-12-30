# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

def from_file(file):
	locals = {}
	imported = execfile(str(file), {}, locals)
	return locals
