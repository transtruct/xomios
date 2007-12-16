# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from xomios.build.project import Project, ConsoleEnvironment, Target, selector
from xomios.build.path import Path, Matcher

import xomios.build.task.filesystem as filesystem
import xomios.build.task.language.java as java
import xomios.build.task.language.c as c

# Initialize project
xomios = Project(name='Xomios')
environment = ConsoleEnvironment(xomios)

# Determine platform
import os, platform

try:
	source_distribution = selector(os.name, {'nt': 'windows', 'posix': 'posix'})
except ValueError:
	xomios.error('Unknown distribution; cannot build', severity=xomios.error.FAIL)

build_platform = platform.system().lower()
build_architecture = platform.machine()

# Define paths
path_src = Path('src')

path_platform = Path('platform')
path_platform_src = path_platform + '/src'
path_platform_native = path_platform + '/native'
path_platform_tests_src = path_platform + '/tests/src'

path_platform_distribution = path_platform + '/distrib/%s' % source_distribution
path_platform_distribution_src = path_platform_distribution + '/src'
path_platform_distribution_native = path_platform_distribution + '/native'
path_platform_distribution_tests_src = path_platform_distribution + '/tests/src'

path_build = Path('build')
path_build_distribution = path_build + '/%s/%s-%s' % (source_distribution, build_platform, build_architecture)
path_build_distribution_classes = path_build_distribution + '/classes'
path_build_distribution_lib = path_build_distribution + '/lib'
path_build_distribution_jar = path_build_distribution + '/jar'
path_build_distribution_tests = path_build_distribution + '/tests'

path_doc = Path('doc')
path_doc_distribution = path_doc + '/%s' % (source_distribution)

path_tests = Path('tests')
path_tests_src = path_tests + '/src'

src = [path_src, path_platform_src, path_platform_distribution_src]
tests = [path_tests_src, path_platform_tests_src, path_platform_distribution_tests_src]
native = [path_platform_native, path_platform_distribution_native]

# Initialization target
init = Target()

xomios.add_target('init', init, hidden=True)

# Clean target
clean = Target(ignore_errors=True)
clean.add_task(filesystem.RemovePathTask(path_build_distribution))
clean.add_task(filesystem.RemovePathTask(path_doc_distribution))

xomios.add_target('clean', clean, depends='init')

# Compile target
compile = Target()
compile.add_task(filesystem.CreateDirectoryTask(path_build_distribution_classes))
compile.add_task(java.JavaCompileTask(Matcher('\.java$').apply(src), class_destination=path_build_distribution_classes))
#compile.add_task(
#	c.CompileTask(
#		Matcher('\.c$').apply(native),
#		link=False,
#		pic=True,
#		standard=c.STD_C99,
#		errors=c.E_ALL | c.E_UNDEF | c.E_MISSING_PROTOTYPES | c.E_MISSING_DECLARATIONS,
#		warnings_as_errors=True,
#		include=[
#			'/usr/lib/java-6-sun/include',
#			Matcher('/jni_md.h$').apply(Path('/usr/lib/java-6-sun/include')).get_file()]))
#compile.add_task(c.CompileTask(c_sources.find_files('\.c$')))
#compile.add_task(c.LinkTask('libxomios.so', c_sources.match('\.o$')))

xomios.add_target('compile', compile, depends='init')

# Documentation target
doc = Target()
#doc.add_task()

xomios.add_target('doc', doc, depends='init')

# Test target
test = Target()
#test.add_task(java.CompileTask(tests_sources.match('\.java$')))

xomios.add_target('test', test, depends=('compile', 'init'))

# Distrib target
distrib = Target()

xomios.add_target('distrib', distrib, depends=('compile', 'test', 'init'))

environment.execute(default='compile')
