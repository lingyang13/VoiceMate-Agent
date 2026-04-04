#pragma once
#include "AIChat.h"
#include <DataSave.h>

class AIChat_ComputerSecurityOfficer :
    public AIChat
{
public:
    AIChat_ComputerSecurityOfficer(const std::string& api_key = "") :AIChat(api_key) {
        SetMaxTokens(DataSave::Get().GetAISecurityOfficerTokenLimit());
        SetMaxTimeoutDuration(DataSave::Get().GetAISecurityOfficerTimeout());
    }
    // Ai 风格化提示语
    virtual std::string getSpeaker2Prompt() override;
};

