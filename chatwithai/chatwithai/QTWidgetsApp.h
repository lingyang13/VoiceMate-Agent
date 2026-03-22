#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

#include "AudioRecorder.h"
#include "WhisperASR.h"
#include "PythonTTS.h"
#include "AIManager.h"
#include "AIAgent.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <io.h>
#include <cstdlib>
#include <mmsystem.h>  // 用于PlaySound函数

class QTWidgetsApp : public QMainWindow
{
    Q_OBJECT

public:
    QTWidgetsApp(QWidget* parent = nullptr);
    ~QTWidgetsApp();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

protected:
    // 右键菜单事件
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    // 录音按钮点击事件
    void onMicrophoneButtonClicked();
    // 文本输入按钮点击事件
    void onTextInputButtonClicked();
    // 切换AI事件
    void onSwitchAiButtonClicked();
	// 发送文本消息事件
	void onSendTextButtonClicked();
    // 选择电脑管家AI事件
	void onComputerManagerAiButtonClicked();


    // 使用聊天AI
	void UseChatAI(const std::string& user_input);
	// 使用电脑管家AI
	void UseComputerManagerAI(const std::string& user_input);

private:
    //ui对象
    Ui::MainWindow ui;

    //用于拖动窗口
    QPoint m_dragPosition;

	// 初始化 录音器
    AudioRecorder recorder;

    // 初始化 语音识别
    WhisperASR asr;

    // 初始化 TTS
    PythonTTS tts;

	// 初始化 AI 管理器
    AIManager ai_manager;

	//是否选中电脑管家AI
	bool isComputerManagerAISelected = false;

	// 初始化 电脑管家AI
	AIAgent ai_computerManager;

};
