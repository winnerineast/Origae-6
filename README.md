# Origae-6
![Alt text](img/header.jpg)

Warning: this is a playground where I build a machine learning system with all open source codes MERELY.
## Background
AI is something interesting but not easy to play with it. So I would like put up some kind of visual works, Origae-6 to lower down the difficulty for most of people who love AI.
## Features
A more detailed working progress can be found in my daily [work notes](docs/dailynotes.md)
- [x] Create datasets for images.
- [ ] Create datasets for texts.
- [ ] Create datasets for audios.
- [ ] Create datasets for videos.
- [x] Train caffe models.
- [x] Train Tensorflow models.
- [x] Train Torch7 models.
- [x] Test caffe models.
- [x] Test Tensorflow models.
- [x] Test Torch7 models.
- [ ] Transfer trained models to remote machines.
## Pre-requirements
- Ubuntu 16.04.3 or Mac OS 10.13: it's OS for all others.
- [Pycharm](https://www.jetbrains.com/pycharm/download/download-thanks.html?platform=linux&code=PCC): it's main editor for coding.
- NVidia driver It's okay to use v384 in ubuntu software update manager and it's used to drive GPU. (skip it with Mac OS.)
- [CUDA Toolkits](https://developer.nvidia.com/cuda-downloads): C library for GPU computing and v8.0. (skip it with Mac OS.)
- [cuDNN](https://developer.nvidia.com/rdp/cudnn-download): deep learning library based on CUDA and v7.0.3. (skip it with Mac OS.)
- pip: Python package manager and v9.0.1. (Warning: if you're using Mac OS, strongly suggest you uninstall all other pythons including the ones from brew because I could only make the installation successfully when I follow up the installation instructions here.)
- [Google/Protobuf](https://github.com/google/protobuf.git)(use 3.4.x) used to exchange data and messages.
- [opencv 3.3.0 and 2.4.13.3](https://github.com/opencv/opencv.git) you could install it by brew when you're using Mac OS.
- [NVIDIA caffe](https://github.com/NVIDIA/caffe) the official version supported by DIGITs. Please install this version when you're new to C/C++ coding.
- [caffe](https://github.com/BVLC/caffe) the original version supported by wider scope. Don't use this version if you have no confident to build caffe by yourself.
- [Tensorflow](tensorflow.org): a famous deep learning framework and v1.3.
- [Keras](keras.io): a wrapper for tensorflow and others.
- [PyTorch](pytorch.org):a deep learning framework in python from Facebook and v0.2.0.
- [NVIDIA/DIGITS](https://github.com/NVIDIA/DIGITS.git)
- [jetson_inference](https://github.com/dusty-nv/jetson-inference.git)(use L4T-R27.1)
## Installing and Developing Origae-6
An actual install script can be referenced here [installation script](https://github.com/winnerineast/Origae-6/blob/master/bin/install.sh) but you have to take care of any difference of versions and models.
When you want to run Origae-6, run the following command:
```bash
cd Origae-6
bin/run.sh
```