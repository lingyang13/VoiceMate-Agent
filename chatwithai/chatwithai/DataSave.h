#pragma once
#include <string>
#include <mutex>

class DataSave
{
public:
    // 获取单例实例
    static DataSave& Get();

    // 加载/保存
    void Load(); //程序开始时自动Load,外部没必要再调用
    void Save(); //自动Save,外部没必要调用

    // ========== API Key 相关 ==========
    void SetDeepSeekApiKey(const std::string& key);
    std::string GetDeepSeekApiKey() const;

    void SetQwen3TtsApiKey(const std::string& key);
    std::string GetQwen3TtsApiKey() const;

    // ========== 聊天角色超时设置 ==========
    void SetChatRoleTimeout(int seconds);
    int GetChatRoleTimeout() const;

    void SetChatRoleTokenLimit(int tokens);
    int GetChatRoleTokenLimit() const;

    // ========== 智能体角色超时设置 ==========
    void SetAIBossTimeout(int seconds);
    int GetAIBossTimeout() const;

    void SetAIManagerTimeout(int seconds);
    int GetAIManagerTimeout() const;

    void SetAISecurityOfficerTimeout(int seconds);
    int GetAISecurityOfficerTimeout() const;

    // ========== 智能体角色 Token 限制 ==========
    void SetAIBossTokenLimit(int tokens);
    int GetAIBossTokenLimit() const;

    void SetAIManagerTokenLimit(int tokens);
    int GetAIManagerTokenLimit() const;

    void SetAISecurityOfficerTokenLimit(int tokens);
    int GetAISecurityOfficerTokenLimit() const;

    // ========== 重置为默认值 ==========
    void ResetChatRoleToDefaults();      // 重置聊天角色的 timeout 和 token 限制
    void ResetAgentRolesToDefaults();    // 重置所有智能体角色的 timeout 和 token 限制

    // ========== 检查是否已配置 ==========
    bool IsDeepSeekConfigured() const;
    bool IsQwen3TtsConfigured() const;

private:
    DataSave();
    ~DataSave() = default;
    DataSave(const DataSave&) = delete;
    DataSave& operator=(const DataSave&) = delete;

    // 加密/解密
    std::string Encrypt(const std::string& plaintext);
    std::string Decrypt(const std::string& ciphertext);

    // 获取配置文件路径
    std::string GetConfigPath() const;

    // 成员变量
    std::string m_deepSeekApiKey="";
    std::string m_qwen3TtsApiKey="";

    // 聊天角色配置
    long m_chatRoleTimeout = 30L;
    int m_chatRoleTokenLimit = 500;

    // 智能体角色配置
    long m_aiBossTimeout = 100L;
    long m_aiManagerTimeout = 200L;
    long m_aiSecurityOfficerTimeout = 30L;

    int m_aiBossTokenLimit = 2000;
    int m_aiManagerTokenLimit = 5000;
    int m_aiSecurityOfficerTokenLimit = 500;

};