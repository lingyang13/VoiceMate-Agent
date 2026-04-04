#include "AIAgent.h"
#include "CommonUtils.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json; // 使用json库

AIAgent::AIAgent(const std::string& apiKey_AI) : 
	apiKey_AI_(apiKey_AI)
{}

std::string AIAgent::askToDo(const std::string& question)
{
	CommonUtils::OpenConsole(); // 打开控制台窗口
    
    AIChat_ComputerSecurityOfficer ai_computer_securityOfficer_(apiKey_AI_);
    AIChat_ComputerBoss ai_computer_boss_(apiKey_AI_);
    AIChat_ComputerManager ai_computer_manager_(apiKey_AI_);

	CommonUtils::WriteAIDebugLog("TaskStartRun:", CommonUtils::UTF8ToString(question) + "\n");

    //任务安全性判定
    std::string security_report_str = ai_computer_securityOfficer_.ask("用户Task：" + question);
    CommonUtils::WriteAIDebugLog("Security determination:", CommonUtils::UTF8ToString(security_report_str) + "\n");
    try {
        json security_report = json::parse(security_report_str);

        if (security_report.contains("status")) {
            if (security_report["status"].get<std::string>() == "SAFE") {
                std::cout << CommonUtils::UTF8ToString("任务安全: ") << CommonUtils::UTF8ToString(CommonUtils::UTF8ToString(security_report["reason"].get<std::string>()))<< "\n" << std::endl;
            }
            else if (security_report["status"].get<std::string>() == "WARNING") {
                std::cout << CommonUtils::UTF8ToString("任务警告: ") << CommonUtils::UTF8ToString(security_report["reason"].get<std::string>()) << "\n" << std::endl;
            }
            else if (security_report["status"].get<std::string>() == "CRITICAL") {
                std::cout << CommonUtils::UTF8ToString("任务极危险: ") << CommonUtils::UTF8ToString(security_report["reason"].get<std::string>()) << "\n" << std::endl;
                CommonUtils::CloseConsole(); // 关闭控制台窗口
                return "任务危险,拒绝执行:" + security_report["reason"].get<std::string>();
            }
            else {  //AI错误行为
                std::cout << CommonUtils::UTF8ToString("安全性判定失效: ") << CommonUtils::UTF8ToString(security_report["status"].get<std::string>()) << "\n" << std::endl;
                CommonUtils::CloseConsole(); // 关闭控制台窗口
                return "任务危险性未知,拒绝执行:";
            }
        }
    }catch (const json::parse_error& e) {
        CommonUtils::WriteAIDebugLog("JSON Parse Error:", e.what());
        std::cerr << "JSON解析失败，原始内容: " << security_report_str << std::endl;
    }catch (const std::exception& e) {
        CommonUtils::WriteAIDebugLog("Exception:", e.what());
        std::cerr << "安全判定发生未知异常: " << e.what() << std::endl;
    }


    // 原始任务
    std::string init_msg = "用户原始Task：" + question + "\n请记住这个Task.";
    // 最大Agent循环次数
    const int MAX_LOOPS = 20;
    //Boss回复
    std::string Boss_response_str = "";

    for (int loop_count = 0; loop_count < MAX_LOOPS; loop_count++) {
		
		// 获取Boss的回复
		if (loop_count == 0) {  // 第一次循环，发送原始任务
			Boss_response_str = ai_computer_boss_.ask(init_msg);
        }

        // 解析成json对象
        json boss_response = json::parse(Boss_response_str);

        //如果任务完成
        if (boss_response.contains("status") && boss_response["status"] == "DONE") {

			std::string feedback = boss_response["summary"].get<std::string>();

			std::cout << CommonUtils::UTF8ToString("任务完成： ") << CommonUtils::UTF8ToString(feedback) << std::endl;
			CommonUtils::WriteAIDebugLog("Boss Final Response:\n", CommonUtils::UTF8ToString(feedback) + "\n\n");
			CommonUtils::CloseConsole(); // 关闭控制台窗口

            return "任务成功" + feedback;
        }

		// 从Boss的回复中提取下一步任务描述
        std::string next_task = boss_response["next_task"]["description"];
        std::string manager_input = "大Task：" + next_task;

        std::cout << "Boss Response:\n" <<CommonUtils::UTF8ToString(next_task) + "\n\n" << std::endl;
        CommonUtils::WriteAIDebugLog("Boss Response:\n", CommonUtils::UTF8ToString(next_task) + "\n\n");

		// 将下一步任务描述发送给Manager，获取具体执行方案
		std::string Manager_response_str = ai_computer_manager_.ask(manager_input);
        json manager_response = json::parse(Manager_response_str);

        CommonUtils::WriteAIDebugLog("Manager Response:\n", CommonUtils::UTF8ToString(Manager_response_str) +"\n\n");

        // 解析steps并执行
        json steps = manager_response["steps"];
        std::string step_results;

        for (auto& step : steps) {
            std::string action = step["action"];
            std::string content = step["content"];
            std::string result;

            std::cout << "Manager Response:\n" << action<< " : " << CommonUtils::UTF8ToString(content) + "\n\n" << std::endl;

            if (action == "powershell") {
                result = CommonUtils::RunPS(step["content"].get<std::string>());

                std::cout << "Employee Pre_RunResult:\n" << CommonUtils::UTF8ToString(result) + "\n\n" << std::endl;
                CommonUtils::WriteAIDebugLog("Employee RunResult:\n", CommonUtils::UTF8ToString(result) + "\n\n");
            }
            else if (action == "write_file") {
                // 写入文件工具
                std::string path = step["path"];
                bool success = CommonUtils::WriteFile(path, content);
                result = success ? "文件写入Success" : "文件写入Failure";

                std::cout << "Employee WriteFile:\n" << path<< " : " << CommonUtils::UTF8ToString(content) + "\n\n" << std::endl;
                CommonUtils::WriteAIDebugLog("File Writing: ", CommonUtils::UTF8ToString(path));
                CommonUtils::WriteAIDebugLog("File Write Content: ", CommonUtils::UTF8ToString(content));
                CommonUtils::WriteAIDebugLog("Is Successfully: ", CommonUtils::UTF8ToString(result) + "\n\n");
            }

            step_results += step["description"].get<std::string>() + " → " + result + "\n";
        }

        // 把结果告诉Boss
        std::string feedback = "执行结果：\n" + step_results + "\n请记住这些新信息。";
        Boss_response_str = ai_computer_boss_.ask(feedback + "根据你记住的信息，下一步做什么？");

    }

    // 超过最大循环次数
    std::cout << CommonUtils::StringToUTF8("警告：达到最大循环次数") << MAX_LOOPS << CommonUtils::StringToUTF8(" 强制停止") << std::endl;
    CommonUtils::CloseConsole(); // 关闭控制台窗口
    return "任务超时";
}