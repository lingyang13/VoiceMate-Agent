#include "QTConfigWindowApp.h"
#include "DataSave.h"
#include <iostream>

QTConfigWindowApp::QTConfigWindowApp(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Exit按钮
    exitBtn = findChild<QPushButton*>("ExitButton");
    if (exitBtn) {
        connect(exitBtn, &QPushButton::clicked,
            this, &QTConfigWindowApp::onExitButtonClicked);
    }

    // APIKey设置选项按钮
    apisetBtn = findChild<QPushButton*>("APIKeySetButton");
    if (apisetBtn) {
        connect(apisetBtn, &QPushButton::clicked,
            this, &QTConfigWindowApp::onAPISetButtonClicked);
    }

    // AI设置选项按钮事件
    aisetBtn = findChild<QPushButton*>("AISetButton");
    if (aisetBtn) {
        connect(aisetBtn, &QPushButton::clicked,
            this, &QTConfigWindowApp::onAISetButtonClicked);
    }



    // APIKey设置窗口
    apisetFrame = findChild<QFrame*>("ApiSetFrame");
    // AI设置窗口
    aisetFrame = findChild<QFrame*>("AISetFrame");

    //初始化按钮状态
    switch (currentOption) {
    case api_set:
        apisetBtn->setEnabled(false);
        aisetFrame->setVisible(false);
        break;
    case ai_set:
        aisetBtn->setEnabled(false);
        apisetBtn->setEnabled(false);
        break;
    }

    // saveAPI按钮
    saveAPIButton = findChild<QPushButton*>("SaveAPIButton");
    if (saveAPIButton) {
        connect(saveAPIButton, &QPushButton::clicked,
            this, &QTConfigWindowApp::onSaveAPIButtonClicked);
    }
    // saveAI按钮
    saveAIButton = findChild<QPushButton*>("SaveAIButton");
    if (saveAIButton) {
        connect(saveAIButton, &QPushButton::clicked,
            this, &QTConfigWindowApp::onSaveAIButtonClicked);
    }

    ui.DeepseekAPIKey_textEdit->setPlainText(QString::fromStdString(DataSave::Get().GetDeepSeekApiKey()));
    ui.Qwen3ttsflashAPIKey_textEdit->setPlainText(QString::fromStdString(DataSave::Get().GetQwen3TtsApiKey()));

    //init文本框
    ui.ChatRole_TimeoutDuration_textEdit->setPlainText(QString::number(DataSave::Get().GetChatRoleTimeout()));
    ui.ChatRole_TokenRestrictions_textEdit->setPlainText(QString::number(DataSave::Get().GetChatRoleTokenLimit()));
    ui.AIBoss_TimeoutDuration_textEdit->setPlainText(QString::number(DataSave::Get().GetAIBossTimeout()));
    ui.AIBoss_TokenRestriction_textEdit->setPlainText(QString::number(DataSave::Get().GetAIBossTokenLimit()));
    ui.AIManager_TimeoutDuration_textEdit->setPlainText(QString::number(DataSave::Get().GetAIManagerTimeout()));
    ui.AIManager_TokenRestriction_textEdit->setPlainText(QString::number(DataSave::Get().GetAIManagerTokenLimit()));
    ui.AISecurityOfficer_TimeoutDuration_textEdit->setPlainText(QString::number(DataSave::Get().GetAISecurityOfficerTimeout()));
    ui.AISecurityOfficer_TokenRestriction_textEdit->setPlainText(QString::number(DataSave::Get().GetAISecurityOfficerTokenLimit()));

}

void QTConfigWindowApp::onAPISetButtonClicked()
{
    apisetBtn->setEnabled(false);
    aisetBtn->setEnabled(true);

    aisetFrame->setVisible(false);
    apisetFrame->setVisible(true);
}

void QTConfigWindowApp::onAISetButtonClicked()
{
    aisetBtn->setEnabled(false);
    apisetBtn->setEnabled(true);

    apisetFrame->setVisible(false);
    aisetFrame->setVisible(true);
}

void QTConfigWindowApp::onSaveAPIButtonClicked()
{
    DataSave::Get().SetDeepSeekApiKey(ui.DeepseekAPIKey_textEdit->toPlainText().toStdString());
    DataSave::Get().SetQwen3TtsApiKey(ui.Qwen3ttsflashAPIKey_textEdit->toPlainText().toStdString());
}

void QTConfigWindowApp::onSaveAIButtonClicked()
{
    DataSave::Get().SetChatRoleTimeout(std::stol(ui.ChatRole_TimeoutDuration_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetChatRoleTokenLimit(std::stol(ui.ChatRole_TokenRestrictions_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetAIBossTimeout(std::stol(ui.AIBoss_TimeoutDuration_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetAIBossTokenLimit(std::stol(ui.AIBoss_TokenRestriction_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetAIManagerTimeout(std::stol(ui.AIManager_TimeoutDuration_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetAIManagerTokenLimit(std::stol(ui.AIManager_TokenRestriction_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetAISecurityOfficerTimeout(std::stol(ui.AISecurityOfficer_TimeoutDuration_textEdit->toPlainText().toStdString()));
    DataSave::Get().SetAISecurityOfficerTokenLimit(std::stol(ui.AISecurityOfficer_TokenRestriction_textEdit->toPlainText().toStdString()));
}

void QTConfigWindowApp::onExitButtonClicked() {
    qApp->quit();
}