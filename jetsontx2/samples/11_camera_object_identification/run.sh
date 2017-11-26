export CAFFE_ROOT=/home/nvidia/caffe
export LD_LIBRARY_PATH=$CAFFE_ROOT/build/lib:/usr/local/cuda/lib64
./camera_caffe -width 1920 -height 1080 \
           -lib opencv_consumer_lib/libopencv_consumer.so \
           -model $CAFFE_ROOT/models/bvlc_reference_caffenet/deploy.prototxt \
           -trained $CAFFE_ROOT/models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel \
           -mean $CAFFE_ROOT/data/ilsvrc12/imagenet_mean.binaryproto \
           -label $CAFFE_ROOT/data/ilsvrc12/synset_words.txt
