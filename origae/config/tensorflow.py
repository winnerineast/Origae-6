from __future__ import absolute_import
from . import option_list


def test_tf_import():
    """
    Tests if tensorflow can be imported, returns if it went okay and optional error.
    """
    try:
        import tensorflow  as tf# noqa
        return True
    except ImportError:
        return False


tf_enabled = test_tf_import()

if not tf_enabled:
    print('Tensorflow support disabled.')

if tf_enabled:
    import tensorflow as tf
    version = tf.__version__
    option_list['tensorflow'] = {
        'version': version,
        'enabled': True,
    }
else:
    version = 'N.A.'
    option_list['tensorflow'] = {
        'version': version,
        'enabled': False,
    }
