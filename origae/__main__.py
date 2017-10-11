import argparse
import os.path
import sys


# Update PATH to include the local Origae-6 directory
def set_origae_directory_into_path():
    PARENT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    found_parent_dir = False
    for p in sys.path:
        if os.path.abspath(p) == PARENT_DIR:
            found_parent_dir = True
            break
    if not found_parent_dir:
        sys.path.insert(0, PARENT_DIR)


def main():
    set_origae_directory_into_path()
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
        print(origae.__version__)
        sys.exit()

    print('This is Origae-6: ', origae.__version__)
    print('')

    import origae.webapp
    # enable message channel.
    try:
        if not origae.webapp.scheduler.start():
            print('ERROR: Scheduler would not start')
        else:
            origae.webapp.app.debug = args['debug']
            origae.webapp.socketio.run(origae.webapp.app, '0.0.0.0', args['port'])
    except KeyboardInterrupt:
        pass
    finally:
        origae.webapp.scheduler.stop()


if __name__ == '__main__':
    main()
