#pragma once
#include "AIChat.h"
#include <DataSave.h>

class AIChat_ComputerBoss :
    public AIChat
{
public:
    // 构造函数
    AIChat_ComputerBoss(const std::string& api_key = "") :AIChat(api_key) {
        SetMaxTokens(DataSave::Get().GetAIBossTokenLimit());
        SetMaxTimeoutDuration(DataSave::Get().GetAIBossTimeout());
    }
    // Ai 风格化提示语
    virtual std::string getSpeaker2Prompt() override;
};

