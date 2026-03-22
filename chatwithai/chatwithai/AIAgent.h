#pragma once
#include <string>
#include "AIChat_ComputerManager.h"
#include "AIChat_ComputerBoss.h"
#include "AIChat_ComputerEmployee.h"

class AIAgent
{
public:
	AIAgent(const std::string& apiKey_AI);
	~AIAgent() = default;

	std::string askToDo(const std::string& question);  //让AI去做某件事，并返回AI的最终回复
private:

	std::string apiKey_AI_;
};

