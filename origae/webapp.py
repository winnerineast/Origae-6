from __future__ import absolute_import

import os

import flask
from flask.ext.socketio import SocketIO
from gevent import monkey
monkey.patch_all()

from .config import config_value  # noqa
from origae import utils  # noqa
from origae.utils import filesystem as fs  # noqa
from origae.utils.store import StoreCache  # noqa
import origae.scheduler  # noqa

# Create Flask, Scheduler and SocketIO objects

url_prefix = config_value('url_prefix')
app = flask.Flask(__name__, static_url_path=url_prefix+'/static')
app.config['DEBUG'] = True
# Disable CSRF checking in WTForms
app.config['WTF_CSRF_ENABLED'] = False
# This is still necessary for SocketIO
app.config['SECRET_KEY'] = os.urandom(12).encode('hex')
app.url_map.redirect_defaults = False
app.config['URL_PREFIX'] = url_prefix
socketio = SocketIO(app, async_mode='gevent', path=url_prefix+'/socket.io')
app.config['store_cache'] = StoreCache()
app.config['store_url_list'] = config_value('model_store')['url_list']
scheduler = origae.scheduler.Scheduler(config_value('gpu_list'), True)

# Register filters and views

app.jinja_env.globals['server_name'] = config_value('server_name')
app.jinja_env.globals['server_version'] = origae.__version__
app.jinja_env.globals['caffe_version'] = config_value('caffe')['version']
app.jinja_env.globals['caffe_flavor'] = config_value('caffe')['flavor']
app.jinja_env.globals['dir_hash'] = fs.dir_hash(
    os.path.join(os.path.dirname(origae.__file__), 'static'))
app.jinja_env.filters['print_time'] = utils.time_filters.print_time
app.jinja_env.filters['print_time_diff'] = utils.time_filters.print_time_diff
app.jinja_env.filters['print_time_since'] = utils.time_filters.print_time_since
app.jinja_env.filters['sizeof_fmt'] = utils.sizeof_fmt
app.jinja_env.filters['has_permission'] = utils.auth.has_permission
app.jinja_env.trim_blocks = True
app.jinja_env.lstrip_blocks = True

import origae.views  # noqa
app.register_blueprint(origae.views.blueprint,
                       url_prefix=url_prefix)
import origae.dataset.views  # noqa
app.register_blueprint(origae.dataset.views.blueprint,
                       url_prefix=url_prefix+'/datasets')
import origae.dataset.generic.views  # noqa
app.register_blueprint(origae.dataset.generic.views.blueprint,
                       url_prefix=url_prefix+'/datasets/generic')
import origae.dataset.images.views  # noqa
app.register_blueprint(origae.dataset.images.views.blueprint,
                       url_prefix=url_prefix+'/datasets/images')
import origae.dataset.images.classification.views  # noqa
app.register_blueprint(origae.dataset.images.classification.views.blueprint,
                       url_prefix=url_prefix+'/datasets/images/classification')
import origae.dataset.images.generic.views  # noqa
app.register_blueprint(origae.dataset.images.generic.views.blueprint,
                       url_prefix=url_prefix+'/datasets/images/generic')
import origae.model.views  # noqa
app.register_blueprint(origae.model.views.blueprint,
                       url_prefix=url_prefix+'/models')
import origae.model.images.views  # noqa
app.register_blueprint(origae.model.images.views.blueprint,
                       url_prefix=url_prefix+'/models/images')
import origae.model.images.classification.views  # noqa
app.register_blueprint(origae.model.images.classification.views.blueprint,
                       url_prefix=url_prefix+'/models/images/classification')
import origae.model.images.generic.views  # noqa
app.register_blueprint(origae.model.images.generic.views.blueprint,
                       url_prefix=url_prefix+'/models/images/generic')
import origae.pretrained_model.views  # noqa
app.register_blueprint(origae.pretrained_model.views.blueprint,
                       url_prefix=url_prefix+'/pretrained_models')
import origae.store.views  # noqa
app.register_blueprint(origae.store.views.blueprint,
                       url_prefix=url_prefix+'/store')


def username_decorator(f):
    from functools import wraps

    @wraps(f)
    def decorated(*args, **kwargs):
        this_username = flask.request.cookies.get('username', None)
        app.jinja_env.globals['username'] = this_username
        return f(*args, **kwargs)
    return decorated

for endpoint, function in app.view_functions.iteritems():
    app.view_functions[endpoint] = username_decorator(function)

# Setup the environment

scheduler.load_past_jobs()
