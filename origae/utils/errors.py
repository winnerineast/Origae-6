# Copyright (c) 2014-2017, NVIDIA CORPORATION.  All rights reserved.


class DigitsError(Exception):
    """
    Origae-6 custom exception
    """
    pass


class DeleteError(DigitsError):
    """
    Errors that occur when deleting a job
    """
    pass


class LoadImageError(DigitsError):
    """
    Errors that occur while loading an image
    """
    pass


class UnsupportedPlatformError(DigitsError):
    """
    Errors that occur while performing tasks in unsupported platforms
    """
    pass
