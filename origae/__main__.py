# Copyright (c) 2014-2017, NVIDIA CORPORATION.  All rights reserved.

import argparse
import os.path
import sys


# Update PATH to include the local DIGITS directory
PARENT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
found_parent_dir = False
for p in sys.path:
    if os.path.abspath(p) == PARENT_DIR:
        found_parent_dir = True
        break
if not found_parent_dir:
    sys.path.insert(0, PARENT_DIR)


def main():
    parser = argparse.ArgumentParser(description='DIGITS server')
    parser.add_argument(
        '-p', '--port',
        type=int,
        default=5000,
        help='Port to run app on (default 5000)'
    )
    parser.add_argument(
        '-d', '--debug',
        action='store_true',
        help=('Run the application in debug mode (reloads when the source '
              'changes and gives more detailed error messages)')
    )
    parser.add_argument(
        '--version',
        action='store_true',
        help='Print the version number and exit'
    )

    args = vars(parser.parse_args())

    import origae

    if args['version']:
        print origae.__version__
        sys.exit()

    print 'Origae-6 is coming with ', origae.__version__
    print

    import origae.config
    import origae.log
    import origae.webapp

    try:
        if not origae.webapp.scheduler.start():
            print 'ERROR: Scheduler would not start'
        else:
            origae.webapp.app.debug = args['debug']
            origae.webapp.socketio.run(origae.webapp.app, '0.0.0.0', args['port'])
    except KeyboardInterrupt:
        pass
    finally:
        origae.webapp.scheduler.stop()


if __name__ == '__main__':
    main()
