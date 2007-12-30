#! /usr/bin/env python
# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from distutils.core import setup

# Load the property file.
import sys
sys.path.append('./lib')

from transtruct.conductor.property import from_file

setup(
	name='Conductor',
	version=from_file('version.properties')['version'],
	description='Conductor Build System',
	url='http://conductor.transtruct.org/',
	download_url='http://conductor.transtruct.org/distribution/',
	package_dir={'': 'lib'},
	packages=['transtruct', 'transtruct.conductor', 'transtruct.conductor.task', 'transtruct.conductor.task.language'],
	scripts=['bin/conduct']
)
