#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <QPixmap>
#include <QString>
#include "AIChat.h"

// 宠物状态枚举（只有3种状态）
enum PetState {
    Normal,     // 0: 正常状态
    Thinking,   // 1: 思考状态
    Saying      // 2: 说话状态
};
const int PET_STATE_COUNT = 3;

// AI角色信息结构体（现在只存储TTS音色字符串）
struct AIPersonality {
    std::string name;                    // 角色名称（显示用）
    std::string description;            // 角色描述
    std::string tts_voice;             // TTS音色字符串（Andre, Cherry等）
    QString imagePaths[PET_STATE_COUNT]; // 3种状态的图片路径
    QPixmap petImages[PET_STATE_COUNT]; // 已加载的图片

    AIPersonality() {}
    AIPersonality(const std::string& n, const std::string& desc, const std::string& voice)
        : name(n), description(desc), tts_voice(voice) {
    }

    // 加载图片
    void loadImages() {
        for (int i = 0; i < PET_STATE_COUNT; i++) {
            if (!imagePaths[i].isEmpty()) {
                petImages[i].load(imagePaths[i]);
            }
        }
    }

    // 获取当前状态的图片
    QPixmap getPetImage(PetState state) const {
        if (state >= Normal && state < PET_STATE_COUNT) {
            return petImages[state];
        }
        return petImages[Normal];  // 默认返回正常状态
    }
};

// AI组件结构体（只需要AI智能指针和角色信息）
struct AIComponents {
    std::unique_ptr<AIChat> ai;        // 多态AI基类指针
    AIPersonality personality;         // 角色信息（包含TTS音色和图片）

    // 构造函数：只初始化AI和角色信息
    AIComponents(std::unique_ptr<AIChat> ai_ptr,
        const AIPersonality& persona)
        : ai(std::move(ai_ptr))
        , personality(persona) {
        personality.loadImages();  // 加载图片
    }
};

//AI管理器类：负责管理多个AI角色，切换当前AI，提供接口给UI调用
class AIManager {
public:
    AIManager(const std::string& api_key_ai);
    ~AIManager() = default;

    // ========== 注册AI角色 ==========
    // 注册熊大
    void register_Bear();
    // 注册猫娘
    void register_CatGirl();
    // 注册自定义AI角色
    template<typename T>
    void register_AI(const AIPersonality& personality) {
        static_assert(std::is_base_of<AIChat, T>::value, "T must derive from AIChat");

        auto ai = std::make_unique<T>(api_key_ai_);
        auto components = std::make_unique<AIComponents>(
            std::move(ai), personality
        );

        ai_components_[personality.name] = std::move(components);
        ai_order_.push_back(personality.name);

        // 如果是第一个注册的AI，自动设为当前
        if (ai_order_.size() == 1) {
            switchAI(personality.name);
        }
    }

    // ========== 切换当前AI ==========
    bool switchAI(const std::string& name);
    bool switchAI(int index);

    // ========== 获取当前AI ==========
    AIChat* getCurrentAI() const;
    std::string getCurrentAIName() const;
    std::string getCurrentTTSVoice() const;  // 获取当前AI的TTS音色
    QPixmap getCurrentPetImage(PetState state) const;
	int getCurrentAiIndex() const;  // 获取当前AI在列表中的索引

    // ========== 获取所有AI信息 ==========
    std::vector<std::string> getAllAINames() const;
    int getAICount() const { return (int)ai_order_.size(); }
    AIPersonality getPersonality(const std::string& name) const;

    // ========== 委托给当前AI的方法 ==========
    std::string ask(const std::string& question);
    void clearHistory();
    void setApiKey(const std::string& api_key_ai);

    // ========== 状态管理 ==========
    void setCurrentState(PetState state) { current_state_ = state; }
    PetState getCurrentState() const { return current_state_; }

    // ========== 回调通知（用于UI更新） ==========
    void setOnAIChanged(std::function<void(const std::string&)> callback);
    void setOnStateChanged(std::function<void(PetState)> callback);

private:
    std::string api_key_ai_;

    // 存储所有AI组件（智能指针自动管理内存）
    std::map<std::string, std::unique_ptr<AIComponents>> ai_components_;
    std::vector<std::string> ai_order_;  // 保持顺序

    // 当前选中的AI
    std::string current_ai_name_;
    PetState current_state_ = Normal;

    // 回调函数
    std::function<void(const std::string&)> on_ai_changed_;
    std::function<void(PetState)> on_state_changed_;
};