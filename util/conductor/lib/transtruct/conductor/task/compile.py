# This file is part of Conductor, a free build system.
# Copyright (c) 2007, 2008 Transtruct Foundation
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

from transtruct.conductor.task.core import ContainerTask, ContainerEnvironment, EnvironmentError

class CompileError(EnvironmentError):
	pass

class CompileEnvironment(ContainerEnvironment):
	pass

class CompileTask(ContainerTask):
	pass
