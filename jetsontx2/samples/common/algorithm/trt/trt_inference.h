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
#ifndef TRT_INFERENCE_H_
#define TRT_INFERENCE_H_

#include <fstream>
#include <queue>
#include "NvInfer.h"
#include "NvCaffeParser.h"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
using namespace nvinfer1;
using namespace nvcaffeparser1;
using namespace std;

// Model Index
#define GOOGLENET_SINGLE_CLASS 0
#define GOOGLENET_THREE_CLASS  1
#define HELNET_THREE_CLASS  2

class Logger;

class Profiler;

class TRT_Context
{
public:
    //net related parameter
    int getNetWidth() const;

    int getNetHeight() const;

    uint32_t getBatchSize() const;

    int getChannel() const;

    int getModelClassCnt() const;

    // Buffer is allocated in TRT_Conxtex,
    // Expose this interface for inputing data
    void*& getBuffer(const int& index);

    float*& getInputBuf();

    uint32_t getNumTrtInstances() const;

    void setForcedFp32(const bool& forced_fp32);

    void setDumpResult(const bool& dump_result);

    void setTrtProfilerEnabled(const bool& enable_trt_profiler);

    int getFilterNum() const;
    void setFilterNum(const unsigned int& filter_num);

    TRT_Context();

    void setModelIndex(int modelIndex);

    void buildTrtContext(const string& deployfile,
            const string& modelfile, bool bUseCPUBuf = false);

    void doInference(
        queue< vector<cv::Rect> >* rectList_queue,
        float *input = NULL);

    void destroyTrtContext(bool bUseCPUBuf = false);

    ~TRT_Context();

private:
    int net_width;
    int net_height;
    int filter_num;
    void  **buffers;
    float *input_buf;
    float *output_cov_buf;
    float *output_bbox_buf;
    float helnet_scale[4];
    IRuntime *runtime;
    ICudaEngine *engine;
    IExecutionContext *context;
    uint32_t *pResultArray;
    int channel;              //input file's channel
    int num_bindings;
    int trtinstance_num;      //inference channel num
    int batch_size;
    bool forced_fp32;
    bool dump_result;
    ofstream fstream;
    bool enable_trt_profiler;
    IHostMemory *trtModelStream{nullptr};
    vector<string> outputs;
    string result_file;
    Logger *pLogger;
    Profiler *pProfiler;
    int frame_num;
    uint64_t elapsed_frame_num;
    uint64_t elapsed_time;
    int inputIndex;
    int outputIndex;
    int outputIndexBBOX;
    DimsCHW inputDims;
    DimsCHW outputDims;
    DimsCHW outputDimsBBOX;
    size_t inputSize;
    size_t outputSize;
    size_t outputSizeBBOX;

    int parseNet(const string& deployfile);
    void parseBbox(vector<cv::Rect>* rectList, int batch_th);
    void allocateMemory(bool bUseCPUBuf);
    void releaseMemory(bool bUseCPUBuf);
    void caffeToTRTModel(const string& deployfile, const string& modelfile);
};

#endif
