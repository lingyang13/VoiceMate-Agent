#pragma once
#include <QMainWindow>
#include "ui_ConfigWindow.h"

enum optionType {
    api_set,ai_set
};

class QTConfigWindowApp : public QMainWindow
{
	Q_OBJECT

public:
    QTConfigWindowApp(QWidget* parent = nullptr);
    ~QTConfigWindowApp() = default;
private slots:
    // Exit按钮点击事件
    void onExitButtonClicked();
    // APISet按钮点击事件
    void onAPISetButtonClicked();
    // AISet按钮点击事件
    void onAISetButtonClicked();
    // saveAPI按钮点击事件
    void onSaveAPIButtonClicked();
    // saveAI按钮点击事件
    void onSaveAIButtonClicked();

private:
    //ui对象
    Ui::ConfigWindow ui;

    //当前选择选项
    optionType currentOption = api_set;

private:
    //侧栏UI
    QPushButton* exitBtn;
    QPushButton* apisetBtn;
    QPushButton* aisetBtn;
    //右侧页面
    QFrame* apisetFrame;
    QFrame* aisetFrame;
    
    //保存按钮
    QPushButton* saveAPIButton;
    QPushButton* saveAIButton;
};

