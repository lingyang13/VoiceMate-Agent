#include "AIManager.h"
#include "AIChat_Bear.h"
#include "AIChat_CatGirl.h"
#include <QDir>

AIManager::AIManager(const std::string& api_key_ai)
: api_key_ai_(api_key_ai) 
{
	// 注册默认AI角色（可以根据需要添加更多角色）
	register_Bear();
	register_CatGirl();
}

// ========== 注册熊大 ==========
void AIManager::register_Bear() {
    AIPersonality personality;
	personality.name = "熊";
    personality.description = "智者，亲切的朋友，直白有深度";
    personality.tts_voice = "Andre";  // 默认"Andre"

    // 设置3种状态的图片路径（根据你的实际路径修改）
    personality.imagePaths[Normal] = "img/Bear.png";
    personality.imagePaths[Thinking] = "img/Bear_Thinking.png";
    personality.imagePaths[Saying] = "img/Bear_Saying.png";

    register_AI<AIChat_Bear>(personality);
}

// ========== 注册猫娘 ==========
void AIManager::register_CatGirl() {
    AIPersonality personality;
    personality.name = "糯糯";
    personality.description = "可爱的猫娘伴侣，会撒娇、有小脾气";
    personality.tts_voice = "Cherry";  // 默认"Cherry"

    // 设置3种状态的图片路径（根据你的实际路径修改）
    personality.imagePaths[Normal] = "img/CatGirl.png";
    personality.imagePaths[Thinking] = "img/CatGirl_Thinking.png";
    personality.imagePaths[Saying] = "img/CatGirl_Saying.png";

    register_AI<AIChat_CatGirl>(personality);
}

// ========== 切换当前AI ==========
bool AIManager::switchAI(const std::string& name) {
    auto it = ai_components_.find(name);
    if (it == ai_components_.end()) {
        return false;
    }

    current_ai_name_ = name;

    // 触发AI切换回调
    if (on_ai_changed_) {
        on_ai_changed_(name);
    }

    return true;
}

bool AIManager::switchAI(int index) {
    if (index < 0 || index >= (int)ai_order_.size()) {
        return false;
    }
    return switchAI(ai_order_[index]);
}

// ========== 获取当前AI ==========
AIChat* AIManager::getCurrentAI() const {
    auto it = ai_components_.find(current_ai_name_);
    if (it != ai_components_.end()) {
        return it->second->ai.get();
    }
    return nullptr;
}

std::string AIManager::getCurrentAIName() const {
    return current_ai_name_;
}

std::string AIManager::getCurrentTTSVoice() const {
    auto it = ai_components_.find(current_ai_name_);
    if (it != ai_components_.end()) {
        return it->second->personality.tts_voice;
    }
    return u8"Andre";  // 默认音色
}

QPixmap AIManager::getCurrentPetImage(PetState state) const {
    auto it = ai_components_.find(current_ai_name_);
    if (it != ai_components_.end()) {
        return it->second->personality.getPetImage(state);
    }
    return QPixmap();  // 返回空图片
}

int AIManager::getCurrentAiIndex() const
{
	return std::distance(ai_order_.begin(), std::find(ai_order_.begin(), ai_order_.end(), current_ai_name_));
}

// ========== 获取所有AI信息 ==========
std::vector<std::string> AIManager::getAllAINames() const {
    return ai_order_;
}

AIPersonality AIManager::getPersonality(const std::string& name) const {
    auto it = ai_components_.find(name);
    if (it != ai_components_.end()) {
        return it->second->personality;
    }
    return AIPersonality();
}

// ========== 委托给当前AI的方法 ==========
std::string AIManager::ask(const std::string& question) {
    auto ai = getCurrentAI();
    if (!ai) {
        return "错误: 没有选中的AI";
    }

    // 调用AI
    std::string answer = ai->ask(question);

    return answer;
}

void AIManager::clearHistory() {
    for (auto& [name, components] : ai_components_) {
        components->ai->clearHistory();
    }
}

void AIManager::setApiKey(const std::string& api_key_ai) {
    api_key_ai_ = api_key_ai;
    for (auto& [name, components] : ai_components_) {
        components->ai->setApiKey(api_key_ai);
    }
}

// ========== 回调设置 ==========
void AIManager::setOnAIChanged(std::function<void(const std::string&)> callback) {
    on_ai_changed_ = callback;
}

void AIManager::setOnStateChanged(std::function<void(PetState)> callback) {
    on_state_changed_ = callback;
}