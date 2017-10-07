#!/bin/sh
# Install brew
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
# Apple hides old versions of stuff at https://developer.apple.com/download/more/
# Install the latest XCode (8.0).
#   We used to install the XCode Command Line Tools 7.3 here, but that would just upset the most recent versions of brew.
#   So we're going to install all our brew dependencies first, and then downgrade the tools. You can switch back after
#   you have installed caffe.
# Install CUDA toolkit 8.0 release candidate
#   Register and download from https://developer.nvidia.com/cuda-release-candidate-download
#   or this path from https://developer.nvidia.com/compute/cuda/8.0/rc/local_installers/cuda_8.0.29_mac-dmg
#   Select both the driver and the toolkit, no documentation necessary
# Install the experimental NVIDIA Mac drivers
#   Download from http://www.nvidia.com/download/driverResults.aspx/103826/en-us
# Install cuDNN v5 for 8.0 RC or use the latest when it's available
#   Register and download from https://developer.nvidia.com/rdp/cudnn-download 
#   or this path: https://developer.nvidia.com/rdp/assets/cudnn-8.0-osx-x64-v5.0-ga-tgz
#   extract to the NVIDIA CUDA folder and perform necessary linking
#   into your /usr/local/cuda/lib and /usr/local/cuda/include folders
#   You will need to use sudo because the CUDA folder is owned by root
sudo tar -xvzf ~/Downloads/cudnn-8.0-osx-x64-v5.0-ga.tgz -C /Developer/NVIDIA/CUDA-8.0/
sudo ln -s /Developer/NVIDIA/CUDA-8.0/lib/libcudnn.dylib /usr/local/cuda/lib/libcudnn.dylib
sudo ln -s /Developer/NVIDIA/CUDA-8.0/lib/libcudnn.5.dylib /usr/local/cuda/lib/libcudnn.5.dylib
sudo ln -s /Developer/NVIDIA/CUDA-8.0/lib/libcudnn_static.a /usr/local/cuda/lib/libcudnn_static.a
sudo ln -s /Developer/NVIDIA/CUDA-8.0/include/cudnn.h /usr/local/cuda/include/cudnn.h
# Install the brew dependencies
#   Do not install python through brew. Only misery lies there
#   We'll use the versions repository to get the right version of boost and boost-python
#   We'll also explicitly upgrade libpng because it's out of date
#   Do not install numpy via brew. Your system python already has it.

brew install -vd snappy leveldb gflags glog szip lmdb
brew tap homebrew/science
brew install hdf5 opencv
# brew upgrade libpng
brew tap homebrew/versions

brew install --build-from-source --with-python -vd protobuf
# brew install --build-from-source -vd boost159 boost-python159
brew install --build-from-source -vd boost boost-python

# Clone the caffe repo
cd ~/Documents
git clone https://github.com/BVLC/caffe.git
# Setup Makefile.config
#   You can download mine directly from here, but I'll explain all the selections
#     For XCode 8.0 and later (Sierra):
#       https://gist.github.com/rizkyario/bcae50a1042d86d6bf01f1b30159cea7
#   First, we'll enable cuDNN
#     USE_CUDNN := 1
#   In order to use the built-in Accelerate.framework, you have to reference it.
#   Astonishingly, nobody has written this anywhere on the internet.
#     BLAS := atlas
#     If you use El Capitan (10.11), we'll use the 10.11 sdk path for vecLib:
#       BLAS_INCLUDE := /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers
#     Otherwise (10.12), let's use the 10.12 sdk path:
#       BLAS_INCLUDE := /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers
#     BLAS_LIB := /System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A
#   Configure to use system python and system numpy
#     PYTHON_INCLUDE := /System/Library/Frameworks/Python.framework/Headers \
#          /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy/core/include
#     PYTHON_LIB := /System/Library/Frameworks/Python.framework/Versions/2.7/lib
#   Configure to enable Python layers. Some projects online need this
#     WITH_PYTHON_LAYER := 1

curl https://gist.githubusercontent.com/rizkyario/bcae50a1042d86d6bf01f1b30159cea7/raw/2ad48060413975b52dc5f0e8c5841e502406dbfb/Makefile.config -o Makefile.config

# Download the XCode Command Line Tools for 7.3, since NVIDIA does not yet support Xcode 8.0's tools
#   http://adcdownload.apple.com/Developer_Tools/Command_Line_Tools_OS_X_10.11_for_Xcode_7.3.1/Command_Line_Tools_OS_X_10.11_for_Xcode_7.3.1.dmg
# Now, choose those tools instead

sudo xcode-select --switch /Library/Developer/CommandLineTools
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer

export LD_LIBRARY_PATH=/Developer/NVIDIA/CUDA-8.0/cuda/lib/:${LD_LIBRARY_PATH}
export DYLD_LIBRARY_PATH=/Developer/NVIDIA/CUDA-8.0/cuda/lib

export DYLD_FALLBACK_LIBRARY_PATH=/usr/local/cuda/lib:/Developer/NVIDIA/CUDA-8.0/lib:/usr/local/cuda

# Go ahead and build.
make -j8 all

make all
make test
make runtest
make pycaffe
make pytest

# To get python going, first we need the dependencies
#   On a super-clean Mac install, you'll need to easy_install pip.
sudo -H easy_install pip
#   Now, we'll install the requirements system-wide. You may also muck about with a virtualenv.
#   Astonishingly, --user is not better known. 
pip install --user -r python/requirements.txt
#   Go ahead and run pytest now. Horrible @rpath warnings which can be ignored.

# Fix Malloc error. This is a leveldb issue
brew install https://gist.githubusercontent.com/hellysmile/ffd665fb1bd1bf978bc99cb7f57250c9/raw/c0a06f1b98388333955f49e30e01dfdde2d82526/leveldb.rb

make -j8 pytest
# Now, install the package
#   Make the distribution folder
make distribute
#   Install the caffe package into your local site-packages
cp -r distribute/python/caffe ~/Library/Python/2.7/lib/python/site-packages/
#   Finally, we have to update references to where the libcaffe libraries are located.
#   You can see how the paths to libraries are referenced relatively
#     otool -L ~/Library/Python/2.7/lib/python/site-packages/caffe/_caffe.so
#   Generally, on a System Integrity Protection -enabled (SIP-enabled) Mac this is no good.
#   So we're just going to change the paths to be direct
cp distribute/lib/libcaffe.so.1.0.0-rc3 ~/Library/Python/2.7/lib/python/site-packages/caffe/libcaffe.so.1.0.0-rc3
install_name_tool -change @rpath/libcaffe.so.1.0.0-rc3 ~/Library/Python/2.7/lib/python/site-packages/caffe/libcaffe.so.1.0.0-rc3 ~/Library/Python/2.7/lib/python/site-packages/caffe/_caffe.so
# Verify that everything works
#   start python and try to import caffe
python -c 'import caffe'
# If you got this far without errors, congratulations, you installed Caffe on a modern Mac OS X 

# Make matlab interface
# https://github.com/BVLC/caffe/issues/3831
make all matcaffe
make mattest
