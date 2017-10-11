#!/usr/bin/env bash

BASEDIR=$(dirname "$0")
echo "$BASEDIR"
SRC_DIR=$BASEDIR
DST_DIR=$BASEDIR
protoc -I=$SRC_DIR --python_out=$DST_DIR $SRC_DIR/caffe.proto