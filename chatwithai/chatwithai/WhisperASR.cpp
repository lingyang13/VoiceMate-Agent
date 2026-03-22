// WhisperASR.cpp
#include "WhisperASR.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <stringapiset.h>

// 包含必要的头文件（使用正确的相对路径）
extern "C" {
#include "../include/whisper.h"
    // 不需要ggml-cpu.h，只需要ggml.h
#include "../include/ggml.h"
}

// 如果没有ggml.h，从GitHub下载或从whisper.cpp/ggml/include复制

WhisperASR::WhisperASR() : ctx_(nullptr) {}

WhisperASR::~WhisperASR() {
    if (ctx_) {
        whisper_free(ctx_);
        ctx_ = nullptr;
    }
}

bool WhisperASR::loadModel(const std::string& modelPath) {
    if (ctx_) return true;

    std::cout << "加载语音识别模型: " << modelPath << std::endl;

    // 检查文件是否存在
    std::ifstream file(modelPath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "错误：找不到模型文件 " << modelPath << std::endl;
        return false;
    }
    file.close();

    // 使用正确的初始化函数
    whisper_context_params params = whisper_context_default_params();
    ctx_ = whisper_init_from_file_with_params(modelPath.c_str(), params);

    if (ctx_) {
        std::cout << "✓ 模型加载成功" << std::endl;
        return true;
    }

    std::cerr << "✗ 模型加载失败" << std::endl;
    return false;
}

std::string WhisperASR::transcribe(const std::vector<char>& audioData) {
    if (!ctx_ || audioData.empty()) {
        return "[错误: 没有音频数据]";
    }

    std::cout << "处理音频数据: " << audioData.size() << " 字节" << std::endl;

    // 1. 转换16位PCM到32位浮点
    std::vector<float> pcmf32;
    size_t sampleCount = audioData.size() / 2;  // 16位 = 2字节/样本

    for (size_t i = 0; i + 1 < audioData.size(); i += 2) {
        short sample = 0;
        std::memcpy(&sample, &audioData[i], sizeof(short));
        pcmf32.push_back(sample / 32768.0f);
    }

    // 2. 设置识别参数
    whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    params.print_realtime = false;
    params.print_progress = false;
    params.print_timestamps = false;
    params.language = "zh";  // 中文
    params.n_threads = 4;    // 使用4个线程

    // 3. 运行识别
    std::cout << "正在识别语音..." << std::endl;

    if (whisper_full(ctx_, params, pcmf32.data(), pcmf32.size()) != 0) {
        return "[识别过程出错]";
    }

    // 4. 获取结果
    std::string text;
    int segmentCount = whisper_full_n_segments(ctx_);

    for (int i = 0; i < segmentCount; i++) {
        const char* segmentText = whisper_full_get_segment_text(ctx_, i);
        if (segmentText) {
            text += segmentText;
        }
    }

    // 清理空格
    if (!text.empty()) {
        // 移除开头空格
        size_t start = text.find_first_not_of(" \t\n\r");
        if (start != std::string::npos) text = text.substr(start);

        // 移除结尾空格
        size_t end = text.find_last_not_of(" \t\n\r");
        if (end != std::string::npos) text = text.substr(0, end + 1);
    }

    return text.empty() ? "[未识别到语音]" : text;
}