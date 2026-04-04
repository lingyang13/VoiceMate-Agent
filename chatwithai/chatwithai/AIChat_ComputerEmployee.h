#pragma once
#include "AIChat.h"

class AIChat_ComputerEmployee :// 该类已废弃
    public AIChat
{
public:
    // 构造函数
    AIChat_ComputerEmployee(const std::string& api_key = "") :AIChat(api_key) {
        SetMaxTokens(2000);
        SetMaxTimeoutDuration(100L);
    }
    // Ai 风格化提示语
    virtual std::string getSpeaker2Prompt() override;
};

