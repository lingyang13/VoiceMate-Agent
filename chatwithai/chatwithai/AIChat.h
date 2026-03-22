// AIChat.h
#pragma once
#include <string>

class AIChat {
public:
    // 构造函数：传入API密钥
    AIChat(const std::string& api_key = "");

    // 设置API密钥
    void setApiKey(const std::string& api_key);

    // 单次对话
    std::string ask(const std::string& question);

    //Ai 风格化提示语(子类可重载)
    virtual std::string getSpeaker2Prompt();

    // 清空对话历史
    void clearHistory();

	//Debug: 获取当前对话历史
    std::string getConversationHistory() const {
        return conversation_history_;
	}

    void SetMaxTokens(int new_maxtokens){
        MaxTokens = new_maxtokens;
    }
    void SetMaxTimeoutDuration(long new_maxtimeoutduration) {
        MaxTimeoutDuration = new_maxtimeoutduration;
    }

private:
    std::string api_key_;
	std::string conversation_history_; // 记录对话历史，包含用户和AI的交互内容
	int conversation_turns_ = 0; // 记录对话轮数
    int MaxTokens = 500;  //最大Tokens数
    long MaxTimeoutDuration = 30L;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    std::string callAPI(const std::string& question);
};