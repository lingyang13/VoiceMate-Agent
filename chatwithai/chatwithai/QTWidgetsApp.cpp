#include "QTWidgetsApp.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QFile>
#include <QLabel>
#include <QMenu>
#include <CommonUtils.h>
#include <thread>

#pragma comment(lib, "winmm.lib")  // 链接winmm库

QTWidgetsApp::QTWidgetsApp(QWidget* parent)
	: QMainWindow(parent), 
    ai_manager("Enter your Deepseek ApiKey"),  //传入Deepseek ApiKey
    tts("Enter your qwen3-tts-flash ApiKey"),         //传入qwen3-tts-flash ApiKey
	ai_computerManager("Enter your Deepseek ApiKey")  ////传入Deepseek ApiKey
{
    ui.setupUi(this);

    // 模型路径：上一级目录的models文件夹
    std::string modelPath = u8"../models/ggml-base.bin";
    if (!asr.loadModel(modelPath)) {
        modelPath = u8"models/ggml-base.bin";
        if (!asr.loadModel(modelPath)) {
            std::cerr << "无法加载语音识别模型！" << std::endl;
            std::cerr << "请确保文件存在: " << modelPath << std::endl;
            std::cin.ignore(); //停留
            return ;
        }
    }
    std::cout << u8"   ✓ 语音识别就绪\n" << std::endl;

    // 初始化录音
    std::cout << u8"2. 初始化录音器..." << std::endl;
    AudioRecorder recorder;
    std::cout << u8"   ✓ 录音器就绪\n" << std::endl;

    // 设置窗口大小
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFixedSize((ai_manager.getCurrentPetImage(Normal).size().width() / 8)+20, (ai_manager.getCurrentPetImage(Normal).size().height() / 8) + 40 + 140);  //+..是为了给其他功能控件腾出空间

    // 设置窗口属性
    setWindowTitle("ChatNG");
    move(100, 100);

    setWindowFlags(Qt::FramelessWindowHint);  // 无边框
    setAttribute(Qt::WA_TranslucentBackground); // 透明背景

	findChild<QTextEdit*>("TestInputBox")->setVisible(false);  // 默认隐藏文本输入框
	findChild<QPushButton*>("SentTextButton")->setVisible(false);  // 默认隐藏发送请求文本按钮

	findChild<QPushButton*> ("ChangeAIButton")->setToolTip("切换AI角色");;  // 按钮提示词
	findChild<QPushButton*>("TextInputButton")->setToolTip("选择文本输入");;  // 按钮提示词
	findChild<QPushButton*>("MicrophoneButton")->setToolTip("语音输入");;  // 按钮提示词
	findChild<QPushButton*>("ComputerManagerAIButton")->setToolTip("切换到电脑管家AI");;  // 按钮提示词
	findChild<QPushButton*>("SentTextButton")->setToolTip("发送文本消息");;  // 按钮提示词


    // 录音按钮事件绑定
    QPushButton* micBtn = findChild<QPushButton*>("MicrophoneButton");
    if (micBtn) {
        connect(micBtn, &QPushButton::clicked,
            this, &QTWidgetsApp::onMicrophoneButtonClicked);
    }
    // 文本输入按钮事件绑定
    QPushButton* textInputBtn = findChild<QPushButton*>("TextInputButton");
    if (textInputBtn) {
        connect(textInputBtn, &QPushButton::clicked,
            this, &QTWidgetsApp::onTextInputButtonClicked);
    }
	// 切换AI按钮事件绑定
    QPushButton* changeAiBtn = findChild<QPushButton*>("ChangeAIButton");
    if (changeAiBtn)
    {
        connect(changeAiBtn, &QPushButton::clicked,
			this, &QTWidgetsApp::onSwitchAiButtonClicked);
    }
	// 发送文本按钮事件绑定
	QPushButton* sendTextBtn = findChild<QPushButton*>("SentTextButton");
    if (sendTextBtn)
    {
        connect(sendTextBtn, &QPushButton::clicked,
            this, &QTWidgetsApp::onSendTextButtonClicked);
	}
	// 选择电脑管家AI按钮事件绑定
	QPushButton* computerManagerAiBtn = findChild<QPushButton*>("ComputerManagerAIButton");
    if (computerManagerAiBtn) {
        connect(computerManagerAiBtn, &QPushButton::clicked,
            this,&QTWidgetsApp::onComputerManagerAiButtonClicked);
    }

}

QTWidgetsApp::~QTWidgetsApp() {}

// 绘制窗口内容
void QTWidgetsApp::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // 设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

	if (!isComputerManagerAISelected) {  //是否选中电脑管家AI（特殊AI模式）
        //获取桌宠状态
        int petStateIndex = ai_manager.getCurrentState();

        // 绘制桌宠图片
        int petImageWidth = ai_manager.getCurrentPetImage(ai_manager.getCurrentState()).size().width() / 8; // 将图片缩小为原来的1/8
        int petImageHeight = ai_manager.getCurrentPetImage(ai_manager.getCurrentState()).size().height() / 8; // 将图片缩小为原来的1/8

        painter.drawPixmap(0, 0, petImageWidth, petImageHeight, ai_manager.getCurrentPetImage(ai_manager.getCurrentState()));
    }
    else {
        // 绘制电脑管家AI界面
        QPixmap computerManagerImage("img/PlagueDoctor.png");
		painter.drawPixmap(0, 0, computerManagerImage.width() / 8, computerManagerImage.height() / 8, computerManagerImage);
    }
}

// 处理鼠标按下事件，记录拖动位置
void QTWidgetsApp::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

// 处理鼠标移动事件，根据拖动位置移动窗口
void QTWidgetsApp::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void QTWidgetsApp::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction("exit", qApp, &QApplication::quit);
    menu.exec(event->globalPos());
}



// 处理录音按钮点击事件
void QTWidgetsApp::onMicrophoneButtonClicked()
{
    QPushButton* micBtn = findChild<QPushButton*>("MicrophoneButton");
    if (!micBtn) return;

    static bool isActive = false;
    isActive = !isActive;

    if (isActive) {
        
        if (recorder.startRecording()) {  //开始录音
            // 按下状态：显示Button_2.png
            micBtn->setIcon(QIcon("img/button/Button_2.png"));
        }
    }
    else {
        
		if (recorder.stopRecording()) {  //停止录音
            // 正常状态：显示Button_1.png
            micBtn->setIcon(QIcon("img/button/Button_1.png"));
			// 获取录音数据
            auto audioData = recorder.getRecordedData();
			// 语音识别
            std::string text = asr.transcribe(audioData);
            // 保存录音文件
            recorder.saveToFile("last_recording.wav");


            // 获取AI回复
			if (!isComputerManagerAISelected) {  //如果没有选中电脑管家AI，就进行正常的问答流程
                ai_manager.setCurrentState(Thinking); // 切换到思考状态
                repaint(); // 重绘
                std::thread t1(std::bind(&QTWidgetsApp::UseChatAI, this, text));
                t1.detach();
            }

            else {  //如果选中电脑管家AI，就执行特定的命令解析流程
                std::thread t2(std::bind(&QTWidgetsApp::UseComputerManagerAI, this, text));
                t2.detach();
            }
            
        }
    }
}

//处理文本输入按钮点击事件
void QTWidgetsApp::onTextInputButtonClicked()
{
    QPushButton* textInputBtn = findChild<QPushButton*>("TextInputButton");
    if (!textInputBtn) return;

    static bool isActive = false;
    isActive = !isActive;

    if (isActive) {
        // 按下状态：显示Button_4.png
        textInputBtn->setIcon(QIcon("img/button/Button_4.png"));

        findChild<QTextEdit*>("TestInputBox")->setVisible(true);
		findChild<QPushButton*>("SentTextButton")->setVisible(true);
    }
    else {
        // 正常状态：显示Button_3.png
        textInputBtn->setIcon(QIcon("img/button/Button_3.png"));

		findChild<QTextEdit*>("TestInputBox")->setVisible(false);
		findChild<QPushButton*>("SentTextButton")->setVisible(false);
    }
}

//处理切换AI按钮点击事件
void QTWidgetsApp::onSwitchAiButtonClicked()
{
    if (ai_manager.getCurrentAiIndex() < ai_manager.getAICount()-1) {
        ai_manager.switchAI(ai_manager.getCurrentAiIndex()+1);
    }
	else if (ai_manager.getCurrentAiIndex() >= ai_manager.getAICount() - 1) { //如果超过了最后一个AI，就切回第一个
		ai_manager.switchAI(0);   
    }
	repaint();
}

//处理发送文本按钮点击事件
void QTWidgetsApp::onSendTextButtonClicked()
{
    QTextEdit* textInputBox = findChild<QTextEdit*>("TestInputBox");
    if (!textInputBox) return;
    std::string question = textInputBox->toPlainText().toStdString();
    if (question.empty()) return;

    // 获取AI回复
    if (!isComputerManagerAISelected) {  //如果没有选中电脑管家AI，就进行正常的问答流程
        ai_manager.setCurrentState(Thinking); // 切换到思考状态
        repaint(); // 重绘
        std::thread t1(std::bind(&QTWidgetsApp::UseChatAI, this, question));
        t1.detach();
    }
    
    else {  //如果选中电脑管家AI，就执行特定的命令解析流程
		std::thread t2(std::bind(&QTWidgetsApp::UseComputerManagerAI, this, question));
		t2.detach();
    }
	textInputBox->clear();  // 清空输入框
}

void QTWidgetsApp::onComputerManagerAiButtonClicked()
{
    QPushButton* computerManagerAiBtn = findChild<QPushButton*>("ComputerManagerAIButton");
    if (!computerManagerAiBtn) return;

    static bool isActive = false;
    isActive = !isActive;

    if (isActive) {
        // 按下状态
        computerManagerAiBtn->setIcon(QIcon("img/button/Button_10.png"));
		isComputerManagerAISelected = true;
		repaint();
    }
    else {
        // 正常状态
        computerManagerAiBtn->setIcon(QIcon("img/button/Button_9.png"));
		isComputerManagerAISelected = false;
		repaint();
    }
}

void QTWidgetsApp::UseChatAI(const std::string& user_input)
{
    std::string answer = ai_manager.ask(user_input);
    // 语音合成回复(TTS)
    std::string audioFile = tts.synthesize(CommonUtils::UTF8ToString(answer), ai_manager.getCurrentTTSVoice(), u8"qwen3-tts-flash");
    ai_manager.setCurrentState(Saying); // 切换回正常状态
    repaint(); // 重绘
    // 播放生成的音频文件
    CommonUtils::playAudioSilent2(audioFile);
    // 清理临时文件
    CommonUtils::CleanTemporaryFiles();
    // 回复完成，切换回正常状态
    ai_manager.setCurrentState(Normal); // 切换回正常状态
    repaint(); // 重绘
}

void QTWidgetsApp::UseComputerManagerAI(const std::string& user_input)
{
    std::string response = ai_computerManager.askToDo(user_input);  //让电脑管家AI解析命令并执行，返回最终结果(无论成败)
    // 语音合成回复(TTS)
    std::string audioFile = tts.synthesize(CommonUtils::UTF8ToString(response), ai_manager.getCurrentTTSVoice(), u8"qwen3-tts-flash");
    // 播放生成的音频文件
    CommonUtils::playAudioSilent2(audioFile);
    // 清理临时文件
    CommonUtils::CleanTemporaryFiles();
}
