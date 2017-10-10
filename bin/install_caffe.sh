#!/bin/sh
# Install XCode from Apple Store
# Install Xcode Command Line Tools.
xcode-select --install
# Install brew
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

# removed leveldb because it's conflict with pip install leveldb
brew install -vd snappy gflags glog szip lmdb
brew tap homebrew/science
brew install hdf5 opencv

brew install --build-from-source --with-python -vd protobuf
brew install --build-from-source -vd boost boost-python

cd /Volumes/d/github/
git clone https://github.com/BVLC/caffe.git
cd caffe

sudo xcode-select --switch /Library/Developer/CommandLineTools
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer

mkdir build
cd build
cmake -D CPU_ONLY=ON ..
# Go ahead and build.
make -j8 all
make runtest
make pytest

pip2 install --user -r ../python/requirements.txt

# Fix Malloc error. This is a leveldb issue
# I prepared a leveldb.rb in Origae-6/bin.
#brew unlink leveldb
brew install https://gist.githubusercontent.com/hellysmile/ffd665fb1bd1bf978bc99cb7f57250c9/raw/c0a06f1b98388333955f49e30e01dfdde2d82526/leveldb.rb

make -j8 pytest
# Now, install the package
#   Make the distribution folder
make install
#   Install the caffe package into your local site-packages
cp -r install/python/caffe /usr/local/lib/python2.7/site-packages/
#   Finally, we have to update references to where the libcaffe libraries are located.
#   You can see how the paths to libraries are referenced relatively
#     otool -L ~/Library/Python/2.7/lib/python/site-packages/caffe/_caffe.so
#   Generally, on a System Integrity Protection -enabled (SIP-enabled) Mac this is no good.
#   So we're just going to change the paths to be direct
cp install/lib/libcaffe.1.0.0.dylib /usr/local/lib/python2.7/site-packages/caffe/libcaffe.1.0.0.dylib
install_name_tool -change @rpath/libcaffe.dylib /usr/local/lib/python2.7/site-packages/caffe/libcaffe.dylib /usr/local/lib/python2.7/site-packages/caffe/_caffe.dylib
ls -lrt /usr/local/lib/python2.7/site-packages/caffe/
export PYTHONPATH=/Volumes/d/github/caffe/python:$PYTHONPATH
# Verify that everything works
#   start python and try to import caffe
python -c 'import caffe'