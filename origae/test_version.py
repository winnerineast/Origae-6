# Copyright (c) 2016-2017, NVIDIA CORPORATION.  All rights reserved.
from __future__ import absolute_import

import os.path
import re

from origae import test_utils


test_utils.skipIfNotFramework('none')


class TestVersion():
    DEV_REGEX = re.compile('^(0|[1-9]\d*)\.(0|[1-9]\d*)-dev$')

    # Copyright (c) Sindre Sorhus <sindresorhus@gmail.com> (sindresorhus.com)
    # The MIT License (MIT)
    # https://github.com/sindresorhus/semver-regex/blob/v1.0.0/index.js
    STANDARD_SEMVER_REGEX = re.compile(
        '^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)'
        '(-[\da-z\-]+(\.[\da-z\-]+)*)?(\+[\da-z\-]+(\.[\da-z\-]+)*)?$')

    def check_version(self, version):
        standard_match = re.match(self.STANDARD_SEMVER_REGEX, version)
        dev_match = re.match(self.DEV_REGEX, version)
        assert (standard_match is not None or dev_match is not None), \
            'Version string "%s" is ill-formatted' % version

    def test_package_version(self):
        import origae
        self.check_version(origae.__version__)

    def test_import_version(self):
        import origae.version
        self.check_version(origae.version.__version__)

    # Test a programmatic and reliable way to check the version
    # python -c "execfile('origae/version.py'); print __version__"
    def test_execfile_version(self):
        import origae
        filename = os.path.join(os.path.dirname(origae.__file__), 'version.py')
        file_locals = {}
        execfile(filename, {}, file_locals)
        assert file_locals.keys() == ['__version__'], \
            'version.py should only declare a single variable'
        self.check_version(file_locals['__version__'])

    # Make sure somebody doesn't overwrite the version in __init__.py
    def test_package_version_matches_import_version(self):
        import origae
        import origae.version

        assert origae.__version__ == origae.version.__version__
