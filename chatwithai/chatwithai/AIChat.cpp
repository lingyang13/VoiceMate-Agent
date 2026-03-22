// AIChat.cpp
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "AIChat.h"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <CommonUtils.h>

using json = nlohmann::json;

// 构造函数
AIChat::AIChat(const std::string& api_key) : api_key_(api_key) {}

// 设置API密钥
void AIChat::setApiKey(const std::string& api_key) {
    api_key_ = api_key;
}

// 清空对话历史
void AIChat::clearHistory() {
    conversation_history_.clear();
}

// CURL回调函数(异常安全)
size_t AIChat::writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    if (!output) return 0;

    size_t total_size = size * nmemb;
    try {
        output->append(static_cast<char*>(contents), total_size);
    }
    catch (const std::exception& e) {
        // 记录错误，返回0表示写入失败
        std::cout << CommonUtils::UTF8ToString("writeCallback 异常: ")<< e.what() << std::endl;
        return 0;  // 返回0会告诉CURL写入失败
    }
    return total_size;
}

// 核心API调用函数
std::string AIChat::callAPI(const std::string& question) {
    if (api_key_.empty()) {
        return "error_noApiKey";
    }

    //std::cout << CommonUtils::UTF8ToString("开始API调用，问题长度: ") << question.length() << std::endl;

    CURL* curl = curl_easy_init();

    //std::cout << CommonUtils::UTF8ToString("CURL初始化: ") << (curl ? CommonUtils::UTF8ToString(("成功")) : CommonUtils::UTF8ToString(("失败"))) << std::endl;

    std::string response;

    if (!curl) {
        std::cout << CommonUtils::UTF8ToString("error_initCURL")<< std::endl;
        return "error_initCURL";
    }

    // 构建UTF-8编码的请求
    json request;
    try {
        request = {
            {"model", "deepseek-chat"},
            {"messages", {
                {{"role", "user"}, {"content", question}}
            }},
            {"max_tokens", MaxTokens},
            {"temperature", 0.7}
        };
    } catch (const std::exception& e) {
        std::cout << CommonUtils::UTF8ToString("构建UTF-8编码的请求: ") << e.what() << std::endl;
        return std::string("error_Request: ") + e.what();
    }

    std::string request_json;
    try {
        request_json = request.dump(-1, ' ', false, json::error_handler_t::replace);
    } catch (const std::exception& e) {
        std::cout << CommonUtils::UTF8ToString("JSONSerialization failed: ")<< e.what() << std::endl;
        return std::string("JSONSerialization failed: ") + e.what();
    }

    // 设置请求头，指定UTF-8编码
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key_).c_str());

    // 设置CURL选项
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.deepseek.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_json.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_json.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, MaxTimeoutDuration);

    // 执行请求
    CURLcode res = curl_easy_perform(curl);

    // 清理
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cout << CommonUtils::UTF8ToString("error_Request:")<< curl_easy_strerror(res) << std::endl;
        return std::string("error_Request: ") + curl_easy_strerror(res);
    }

    // 解析响应
    try {
        json response_json = json::parse(response);

        if (response_json.contains("choices") && !response_json["choices"].empty()) {
            std::string content = response_json["choices"][0]["message"]["content"].get<std::string>();
            return content; // 直接返回UTF-8字符串
        }
        else if (response_json.contains("error")) {
            std::cout << CommonUtils::UTF8ToString("API: 错误")<< response_json["error"]["message"].get<std::string>() << std::endl;
            return std::string("error_API: ") + response_json["error"]["message"].get<std::string>();
        }
    }
    catch (const std::exception& e) {
        std::cout << CommonUtils::UTF8ToString("分析相应失败: ") << e.what() << std::endl;
        return std::string("Analysis response failed: ") + e.what();
    }
    std::cout << CommonUtils::UTF8ToString("未知错误")<< std::endl;
    return "Unknown error";
}

// 用户调用的对话函数
std::string AIChat::ask(const std::string& question) {
    try {
        for (int Retry_count = 0; Retry_count < 3; Retry_count++) {  //重试3次(如果API调用失败)
            // 如果有对话历史，添加到问题中
            std::string full_question = "Previous conversation:\n" + conversation_history_ + "Now the user's question:" + question + getSpeaker2Prompt();
            // 调用API
            std::string answer = callAPI(full_question);
            // 保存到对话历史（简单实现，只保存最近一轮）
            if (answer.find("error") == std::string::npos &&
                answer.find("Failure") == std::string::npos) {
                conversation_history_ += question + "\n" + answer + "\n\n";
                //对话轮数++
                conversation_turns_++;
                return answer;
            }
            else {
                std::cout << CommonUtils::UTF8ToString("API调用返回错误: ") << answer << std::endl;
                if (Retry_count == 2) {
                    return "API Call Error";
                }
            }
        }
    }catch (const std::exception& e) {
        std::cout << CommonUtils::UTF8ToString("系统异常")<< std::endl;
        return std::string("系统异常: ") + e.what();
    }
    catch (...) {
        std::cout << CommonUtils::UTF8ToString("未知异常") << std::endl;
        return "未知系统异常";
    }
    
}

// AI风格化函数
std::string AIChat::getSpeaker2Prompt() {
    return CommonUtils::StringToUTF8(R"(
**角色设定**：
你是羚羊

**禁止内容**：
   - 禁止生成"\n"这种换行符，对话要像人一样是连续的句子，所以生成表格类的内容请转化成文字描述，不要生成任何代码类内容。
   - 禁止生成任何表状态的内容，如<仔细思考了以下>、<开始疑惑>、<笑了笑>、<微笑>等等
)");
}