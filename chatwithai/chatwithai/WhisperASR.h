// WhisperASR.h - 语音识别封装类
#pragma once

#include <string>
#include <vector>

// 前向声明，避免包含复杂头文件
struct whisper_context;
struct whisper_full_params;

class WhisperASR {
public:
    WhisperASR();
    ~WhisperASR();

    bool loadModel(const std::string& modelPath);
    std::string transcribe(const std::vector<char>& audioData);

private:
    whisper_context* ctx_;  // 使用具体类型，不是void*
};