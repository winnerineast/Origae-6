# Origae-6
![Alt text](img/header.jpg)

Warning: this is a playground where I build a machine learning system with all open source codes MERELY since 2018. Now it's 2023 and AI goes into new era and I enter this project again and make it better.
## Background
AI is something interesting but not easy to play with it. So I would like put up some kind of visual works, Origae-6 to lower down the difficulty for most of people who love AI.
I wrote these words in 2018 and I have tried many and now I catch up AIGC (Artificial Intelligence Generated Content) here.
## Features
A more detailed working progress can be found in my daily [work notes](docs/dailynotes.md)
- [ ] LLaMA: Open and Efficient Foundation Language Models
- [ ] Stanford Alpca: An Instruction-following LLaMA Model
- [ ] llama.cpp: Port of Meta LLaMA model in C/C++
- [x] alpaca.cpp: Locally run an Instruction-Tuned Chat-Style LLM
- [ ] dalai: The simplest way to run LLaMA on your local machine.
- [ ] point-alpaca: one of way to further train LLaMA.
- [ ] ColossaAI: Making large AI models cheaper, faster and more accessaible
- [ ] ...
- [x] Create datasets for images. 
- [x] MNIST dataset
- [x] CIFAR-10 dataset
- [x] CIFAR-100 dataset
- [x] Create datasets for texts.
- [x] Create datasets for audios.
- [x] Create datasets for videos.
- [x] Train caffe models.
- [x] Train Tensorflow models.
- [x] Train Torch7 models.
- [x] Test caffe models.
- [x] Test Tensorflow models.
- [x] Test Torch7 models.
- [ ] ...

## Pre-requirements
### here is just my own testing environments.
- Apple Macbook Pro M2 MAX (32GB)
- HP Elitebook 845 G8 (32GB)
- Software requirements will be moved to individual software projects.
### Warning: the following instruction has been outdated and merely for your information.

- Ubuntu 16.04.3 or Mac OS 10.13: it's OS for all others.
- [Pycharm](https://www.jetbrains.com/pycharm/download/download-thanks.html?platform=linux&code=PCC): it's main editor for coding.
- NVidia driver It's okay to use v384 in ubuntu software update manager and it's used to drive GPU. (skip it with Mac OS.)
- [CUDA Toolkits](https://developer.nvidia.com/cuda-downloads): C library for GPU computing and v8.0. (skip it with Mac OS.)
- [cuDNN](https://developer.nvidia.com/rdp/cudnn-download): deep learning library based on CUDA and v7.0.3. (skip it with Mac OS.)
- pip: Python package manager and v9.0.1. (Warning: if you're using Mac OS, strongly suggest you uninstall all other pythons including the ones from brew because I could only make the installation successfully when I follow up the installation instructions here.)
- [Google/Protobuf](https://github.com/google/protobuf.git)(use 3.4.x) used to exchange data and messages.
- [opencv 3.3.0 and 2.4.13.3](https://github.com/opencv/opencv.git) you could install it by brew when you're using Mac OS.
- [NVIDIA caffe](https://github.com/NVIDIA/caffe) the official version supported by DIGITs. Please install this version when you're new to C/C++ coding. Some installation guide can be found [here](docs/BuildCaffe.md)
- [caffe](https://github.com/BVLC/caffe) the original version supported by wider scope. Don't use this version if you have no confident to build caffe by yourself. Some installation guide can be found [here](docs/BuildCaffe.md). The key thing to build caffe in mac os is to make sure your PYTHONLIB pointing to right python installed by brew instead of Apple MacOS itself. (do this right before you cmake or make caffe. PYTHONLIB:=/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib:$PYTHONLIB). Another tricky part is PYTHON_INCLUDE := /usr/include/python2.7 \
        /usr/local/lib/python2.7/site-packages/numpy/core/include/.
- [Tensorflow](tensorflow.org): a famous deep learning framework and v1.3.
- [Keras](keras.io): a wrapper for tensorflow and others.
- [PyTorch](pytorch.org):a deep learning framework in python from Facebook and v0.2.0.
- [NVIDIA/DIGITS](https://github.com/NVIDIA/DIGITS.git)
- [jetson_inference](https://github.com/dusty-nv/jetson-inference.git)(use L4T-R27.1)
## Installing and Developing Origae-6
Clone Origae-6 project and, run the following command:
```bash
cd Origae-6
bin/run.sh
```