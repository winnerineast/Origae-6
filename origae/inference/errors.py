from __future__ import absolute_import

from origae.utils import subclass, override


@subclass
class Error(Exception):
    pass


@subclass
class InferenceError(Error):
    """
    Errors that occur during inference
    """

    def __init__(self, message):
        self.message = message

    @override
    def __str__(self):
        return repr(self.message)
