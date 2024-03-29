/*
 * Copyright (c) 2016, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

@page 11_camera_object_identification_build_and_run Building and Running
@{

This sample uses Caffe to classify the kinds of objects that appear in
the camera stream.

### Prerequisites ###
* You have followed Steps 1-3 in @ref mmapi_build.
* You have a working camera module.

### To build and run

1. Set up the Caffe environment.

   a) Install packages from APT with the following commands:

        $ sudo add-apt-repository universe
        $ sudo add-apt-repository multiverse
        $ sudo apt-get update
        $ sudo apt-get install libboost-all-dev libprotobuf-dev libleveldb-dev libsnappy-dev
        $ sudo apt-get install libhdf5-serial-dev protobuf-compiler libgflags-dev libgoogle-glog-dev
        $ sudo apt-get install liblmdb-dev libblas-dev libatlas-base-dev

    b) Download Caffe source package from the following website:

        https://github.com/BVLC/caffe

       And copy the package to `$HOME` directory on the target board
       with the following command:

           $ mkdir -pv $HOME/Work/caffe
           $ cp caffe-master.zip $HOME/Work/caffe/
           $ cd $HOME/Work/caffe/ && unzip caffe-master.zip

    c) Build Caffe source with the following commands:

        $ cd $HOME/Work/caffe/caffe-master
       Locate and edit `Makefile.config.example`.

       Uncomment the following line to enable cuDNN acceleration:

            USE_CUDNN := 1

        Add two lines to CUDA architecture setting:

            -gencode arch=compute_53,code=sm_53 \
            -gencode arch=compute_62,code=sm_62
       And modify the following two lines. Then save and close the file.

            INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial
            LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/aarch64-linux-gnu/hdf5/serial
       Then enter:

            $ cp Makefile.config.example Makefile.config
            $ make -j4

       The library libcaffe.so is generated in the `build/lib` directory.

2. Compile the OpenCV consumer library. This library is needed for Caffe and
   must be compiled on the target board.

       $ cd 11_camera_object_identification/opencv_consumer_lib

   Check the Makefile to ensure the following variables are set correctly:

        CUDA_DIR:=/usr/local/cuda
        CAFFE_DIR:=$HOME/Work/caffe/caffe-master
   Then enter:

        $ make

    This command generates the library `libopencv_consumer.so` in the current directory.

3. Download the Caffe model binaries with the following commands:

       $ sudo apt-get install python-pip
       $ sudo pip install pyyaml six
       $ cd $HOME/Work/caffe/caffe-master
       $ ./scripts/download_model_binary.py  models/bvlc_reference_caffenet/

    Get the `ImageNet` labels file with the following command:

        $ ./data/ilsvrc12/get_ilsvrc_aux.sh

4. Build and run the sample with the following commands:

       $ cd 11_camera_object_identification
       $ make
       $ export LD_LIBRARY_PATH=$HOME/Work/caffe/caffe-master/build/lib:/usr/local/cuda/lib64
       $ ./camera_caffe -width 1920 -height 1080 \
           -lib opencv_consumer_lib/libopencv_consumer.so \
           -model $HOME/Work/caffe/caffe-master/models/bvlc_reference_caffenet/deploy.prototxt \
           -trained $HOME/Work/caffe/caffe-master/models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel \
           -mean $HOME/Work/caffe/caffe-master/data/ilsvrc12/imagenet_mean.binaryproto \
           -label $HOME/Work/caffe/caffe-master/data/ilsvrc12/synset_words.txt
@}
