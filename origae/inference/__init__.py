from __future__ import absolute_import

from .audio import AudioInferenceJob
from .images import ImageInferenceJob
from .text import TextInferenceJob
from .job import InferenceJob

__all__ = [
    'InferenceJob',
    'ImageInferenceJob',
    'AudioInferenceJob',
    'TextInferenceJob',
]
