from __future__ import absolute_import

from ..job import InferenceJob
from origae.utils import subclass, override


@subclass
class TextInferenceJob(InferenceJob):
    """
    A Job that exercises the forward pass of an image neural network
    """

    @override
    def job_type(self):
        return 'Text Inference'
