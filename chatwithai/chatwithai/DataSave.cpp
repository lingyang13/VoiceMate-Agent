#include "DataSave.h"
#include<iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#include <random>
#include "CommonUtils.h"

DataSave::DataSave() {
    Load();
}

DataSave& DataSave::Get()
{
    static DataSave instance;
    return instance;
}

std::string DataSave::GetConfigPath() const
{
    // 获取 exe 所在目录
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string exeDir = exePath;
    size_t lastSlash = exeDir.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        exeDir = exeDir.substr(0, lastSlash);
    }

    // 创建 IUserData 目录
    std::string dir = exeDir + "\\IUserData";
    CreateDirectoryA(dir.c_str(), NULL);

    // 返回完整路径
    return dir + "\\config.dat";
}

// 简单加密（XOR）
std::string DataSave::Encrypt(const std::string& plaintext)
{
    std::string result = plaintext;
    const char key = 0x5A;
    for (char& c : result) {
        c ^= key;
    }
    return result;
}

std::string DataSave::Decrypt(const std::string& ciphertext)
{
    return Encrypt(ciphertext);  // XOR 加密是对称的
}

void DataSave::Load()
{
    std::string path = GetConfigPath();
    if (!std::filesystem::exists(path)) {
        ResetAgentRolesToDefaults();
        ResetChatRoleToDefaults();
        return;
    }
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        ResetAgentRolesToDefaults();
        ResetChatRoleToDefaults();
        return;
    }
    std::string encrypted((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    std::string data = Decrypt(encrypted);
    // 解析键值对
    std::stringstream ss(data);
    std::string line;
    while (std::getline(ss, line)) {
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key == "DeepSeekApiKey") m_deepSeekApiKey = value;
        else if (key == "Qwen3TtsApiKey") m_qwen3TtsApiKey = value;
        else if (key == "ChatRoleTimeout") m_chatRoleTimeout = std::stoi(value);
        else if (key == "AIBossTimeout") m_aiBossTimeout = std::stoi(value);
        else if (key == "AIManagerTimeout") m_aiManagerTimeout = std::stoi(value);
        else if (key == "AISecurityOfficerTimeout") m_aiSecurityOfficerTimeout = std::stoi(value);
        else if (key == "ChatRoleTokenLimit") m_chatRoleTokenLimit = std::stoi(value);
        else if (key == "AIBossTokenLimit") m_aiBossTokenLimit = std::stoi(value);
        else if (key == "AIManagerTokenLimit") m_aiManagerTokenLimit = std::stoi(value);
        else if (key == "AISecurityOfficerTokenLimit") m_aiSecurityOfficerTokenLimit = std::stoi(value);
    }
}

void DataSave::Save()
{
    // 构建保存数据
    std::stringstream ss;
    ss << "DeepSeekApiKey=" << m_deepSeekApiKey << "\n";
    ss << "Qwen3TtsApiKey=" << m_qwen3TtsApiKey << "\n";
    ss << "ChatRoleTimeout=" << m_chatRoleTimeout << "\n";
    ss << "AIBossTimeout=" << m_aiBossTimeout << "\n";
    ss << "AIManagerTimeout=" << m_aiManagerTimeout << "\n";
    ss << "AISecurityOfficerTimeout=" << m_aiSecurityOfficerTimeout << "\n";
    ss << "ChatRoleTokenLimit=" << m_chatRoleTokenLimit << "\n";
    ss << "AIBossTokenLimit=" << m_aiBossTokenLimit << "\n";
    ss << "AIManagerTokenLimit=" << m_aiManagerTokenLimit << "\n";
    ss << "AISecurityOfficerTokenLimit=" << m_aiSecurityOfficerTokenLimit << "\n";

    std::string data = ss.str();
    std::string encrypted = Encrypt(data);

    std::ofstream file(GetConfigPath(), std::ios::binary);
    if (file.is_open()) {
        file.write(encrypted.c_str(), encrypted.size());
        file.close();
    }
}

// ========== API Key 相关 ==========
void DataSave::SetDeepSeekApiKey(const std::string& key)
{
    m_deepSeekApiKey = key;
    Save();
}

std::string DataSave::GetDeepSeekApiKey() const
{
    return m_deepSeekApiKey;
}

void DataSave::SetQwen3TtsApiKey(const std::string& key)
{
    m_qwen3TtsApiKey = key;
    Save();
}

std::string DataSave::GetQwen3TtsApiKey() const
{
    return m_qwen3TtsApiKey;
}

// ========== 超时设置 ==========
void DataSave::SetChatRoleTimeout(int seconds) { m_chatRoleTimeout = seconds; Save(); }
int DataSave::GetChatRoleTimeout() const { return m_chatRoleTimeout; }

void DataSave::SetAIBossTimeout(int seconds) { m_aiBossTimeout = seconds; Save(); }
int DataSave::GetAIBossTimeout() const { return m_aiBossTimeout; }

void DataSave::SetAIManagerTimeout(int seconds) { m_aiManagerTimeout = seconds; Save(); }
int DataSave::GetAIManagerTimeout() const { return m_aiManagerTimeout; }

void DataSave::SetAISecurityOfficerTimeout(int seconds) { m_aiSecurityOfficerTimeout = seconds; Save(); }
int DataSave::GetAISecurityOfficerTimeout() const { return m_aiSecurityOfficerTimeout; }

// ========== Token 限制 ==========
void DataSave::SetChatRoleTokenLimit(int tokens) { m_chatRoleTokenLimit = tokens; Save(); }
int DataSave::GetChatRoleTokenLimit() const { return m_chatRoleTokenLimit; }

void DataSave::SetAIBossTokenLimit(int tokens) { m_aiBossTokenLimit = tokens; Save(); }
int DataSave::GetAIBossTokenLimit() const { return m_aiBossTokenLimit; }

void DataSave::SetAIManagerTokenLimit(int tokens) { m_aiManagerTokenLimit = tokens; Save(); }
int DataSave::GetAIManagerTokenLimit() const { return m_aiManagerTokenLimit; }

void DataSave::SetAISecurityOfficerTokenLimit(int tokens) { m_aiSecurityOfficerTokenLimit = tokens; Save(); }
int DataSave::GetAISecurityOfficerTokenLimit() const { return m_aiSecurityOfficerTokenLimit; }

// ========== 重置聊天角色为默认值 ==========
void DataSave::ResetChatRoleToDefaults()
{
    m_chatRoleTimeout = 30L;
    m_chatRoleTokenLimit = 500;
    Save();
}

// ========== 重置所有智能体角色为默认值 ==========
void DataSave::ResetAgentRolesToDefaults()
{
    // 重置 timeout
    m_aiBossTimeout = 100L;
    m_aiManagerTimeout = 200L;
    m_aiSecurityOfficerTimeout = 30L;

    // 重置 token 限制
    m_aiBossTokenLimit = 2000;
    m_aiManagerTokenLimit = 5000;
    m_aiSecurityOfficerTokenLimit = 500;

    Save();
}

bool DataSave::IsDeepSeekConfigured() const
{
    return !m_deepSeekApiKey.empty();
}

bool DataSave::IsQwen3TtsConfigured() const
{
    return !m_qwen3TtsApiKey.empty();
}