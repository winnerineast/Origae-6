#!/usr/bin/env bash
set -e
cd ..
export CAFFE_ROOT=/Volumes/d/github/caffe
export TORCH_ROOT=/Users/liling/torch
export PYTHONPATH=/usr/local/Cellar/numpy/1.13.3/lib/python2.7/site-packages/numpy;$CAFFE_ROOT/python;$PYTHONPATH
python2 -m origae $@