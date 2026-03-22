#pragma once
#include<string>

//程序库类
class CommonUtils
{
public:
	// UTF-8字符串转换为系统默认编码字符串
	static std::string UTF8ToString(const std::string& utf8_str);
	// 系统默认编码字符串转换为UTF-8字符串
	static std::string StringToUTF8(const std::string& str);
	// 清理临时文件，保留指定文件（如果提供）
	static void CleanTemporaryFiles(const std::string& keepFile = "");
	// 播放音频文件,阻塞
	static bool playAudioSilent(const std::string& audioFile);
	static bool playAudioSilent2(const std::string& audioFile);
	// 运行PowerShell命令并获取输出结果
	static std::string RunPS(const std::string& cmd);
	// 写入文件
	static bool WriteFile(const std::string& filePath, const std::string& content);
	// 读取文件
	static std::string ReadFile(const std::string& filePath);
	// 写入AI调试日志
	static void WriteAIDebugLog(const std::string& type, const std::string& content);
	// 打开控制台窗口
	static void OpenConsole();
	// 关闭控制台窗口
	static void CloseConsole();
};

