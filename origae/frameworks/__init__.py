# Copyright (c) 2015-2017, NVIDIA CORPORATION.  All rights reserved.
from __future__ import absolute_import

from .framework import Framework
from origae.config import config_value

__all__ = [
    'Framework',
    'CaffeFramework',
    'TorchFramework',
]

if config_value('tensorflow')['enabled']:
    from .tensorflow_framework import TensorflowFramework
    __all__.append('TensorflowFramework')

#
#  create framework instances
#
# tensorflow is optional
tensorflow = TensorflowFramework() if config_value('tensorflow')['enabled'] else None
#
#  utility functions
#


def get_frameworks():
    """
    return list of all available framework instances
    there may be more than one instance per framework class
    """
    frameworks = [tensorflow]
    return frameworks


def get_framework_by_id(framework_id):
    """
    return framework instance associated with given id
    """
    for fw in get_frameworks():
        if fw.get_id() == framework_id:
            return fw
    return None
