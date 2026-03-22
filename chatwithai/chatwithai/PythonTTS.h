#pragma once
#include <string>

class PythonTTS {
public:
    PythonTTS(const std::string& api_key);  // 构造函数传入 API key
    ~PythonTTS() = default;

    void setApiKey(const std::string& api_key);  // 设置 API key 的方法

	// 合成文本为语音，返回音频文件路径
    std::string synthesize(const std::string& text,
        const std::string& voice = "Andre",
        const std::string& model = "qwen3-tts-flash");

private:
    std::string api_key_;
};