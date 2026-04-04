#pragma once
#include "AIChat.h"
#include <DataSave.h>

class AIChat_ComputerManager : public AIChat
{
public:
    // 构造函数
    AIChat_ComputerManager(const std::string& api_key = "") :AIChat(api_key) {
        SetMaxTokens(DataSave::Get().GetAIManagerTokenLimit());
        SetMaxTimeoutDuration(DataSave::Get().GetAIManagerTimeout());
    }
    // Ai 风格化提示语
    virtual std::string getSpeaker2Prompt() override;
};

