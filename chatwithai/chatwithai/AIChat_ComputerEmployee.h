#pragma once
#include "AIChat.h"
class AIChat_ComputerEmployee :
    public AIChat
{
public:
    // 构造函数：基类传入API密钥
    AIChat_ComputerEmployee(const std::string& api_key = "") :AIChat(api_key) {
        SetMaxTokens(2000);
        SetMaxTimeoutDuration(100L);
    }
    // Ai 风格化提示语,重载基类函数
    virtual std::string getSpeaker2Prompt() override;
};

