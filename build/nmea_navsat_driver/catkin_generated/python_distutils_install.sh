#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/achu/aurora2018/aurora2018/src/nmea_navsat_driver"

# snsure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/achu/aurora2018/aurora2018/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/achu/aurora2018/aurora2018/install/lib/python2.7/dist-packages:/home/achu/aurora2018/aurora2018/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/achu/aurora2018/aurora2018/build" \
    "/usr/bin/python" \
    "/home/achu/aurora2018/aurora2018/src/nmea_navsat_driver/setup.py" \
    build --build-base "/home/achu/aurora2018/aurora2018/build/nmea_navsat_driver" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/achu/aurora2018/aurora2018/install" --install-scripts="/home/achu/aurora2018/aurora2018/install/bin"
