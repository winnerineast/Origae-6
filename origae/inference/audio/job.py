from __future__ import absolute_import

from ..job import InferenceJob
from origae.utils import subclass, override


@subclass
class AudioInferenceJob(InferenceJob):
    """
    A Job that exercises the forward pass of an audio neural network
    """

    @override
    def job_type(self):
        return 'Audio Inference'
