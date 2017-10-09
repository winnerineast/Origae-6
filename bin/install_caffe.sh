#!/bin/sh
# Install brew
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

brew install -vd snappy leveldb gflags glog szip lmdb
brew tap homebrew/science
brew install hdf5 opencv

brew install --build-from-source --with-python -vd protobuf
brew install --build-from-source -vd boost boost-python

for x in snappy leveldb protobuf gflags glog szip boost boost-python lmdb homebrew/science/opencv; do brew edit $x; done
#      # ADD THE FOLLOWING:
#      ENV.append "CXXFLAGS", "-stdlib=libstdc++"
#      ENV.append "CFLAGS", "-stdlib=libstdc++"
#      ENV.append "LDFLAGS", "-stdlib=libstdc++ -lstdc++"
#      # The following is necessary because libtool likes to strip LDFLAGS:
#      ENV["CXX"] = "/usr/bin/clang++ -stdlib=libstdc++"
for x in snappy leveldb gflags glog szip lmdb homebrew/science/opencv; do brew uninstall $x; brew install --build-from-source -vd $x; done
brew uninstall protobuf; brew install --build-from-source --with-python -vd protobuf
brew install --build-from-source -vd boost boost-python
# Clone the caffe repo
cd ~/Documents
git clone https://github.com/BVLC/caffe.git

curl https://gist.githubusercontent.com/rizkyario/bcae50a1042d86d6bf01f1b30159cea7/raw/2ad48060413975b52dc5f0e8c5841e502406dbfb/Makefile.config -o Makefile.config
# I prepared a ready-for-go Makefile.config in Origae-6/bin.
# Download the XCode Command Line Tools for 7.3, since NVIDIA does not yet support Xcode 8.0's tools
#   http://adcdownload.apple.com/Developer_Tools/Command_Line_Tools_OS_X_10.11_for_Xcode_7.3.1/Command_Line_Tools_OS_X_10.11_for_Xcode_7.3.1.dmg
# Now, choose those tools instead
sudo xcode-select --switch /Library/Developer/CommandLineTools
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer

#export LD_LIBRARY_PATH=/Developer/NVIDIA/CUDA-8.0/cuda/lib/:${LD_LIBRARY_PATH}
#export DYLD_LIBRARY_PATH=/Developer/NVIDIA/CUDA-8.0/cuda/lib
#export DYLD_FALLBACK_LIBRARY_PATH=/usr/local/cuda/lib:/Developer/NVIDIA/CUDA-8.0/lib:/usr/local/cuda
mkdir build
cd build
cmake -D CPU_ONLY=ON ..
# Go ahead and build.
make -j8 all
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
# I prepared a leveldb.rb in Origae-6/bin.
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
# Create local git in order to do brew update.
cd /usr/local
git checkout -b caffe
git add .
git commit -m "Update Caffe dependencies to use libstdc++"
cd /usr/local/Library/Taps/homebrew/homebrew-science
git checkout -b caffe
git add .
git commit -m "Update Caffe dependencies"