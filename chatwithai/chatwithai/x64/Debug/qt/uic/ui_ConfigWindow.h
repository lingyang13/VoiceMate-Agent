/********************************************************************************
** Form generated from reading UI file 'ConfigWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGWINDOW_H
#define UI_CONFIGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigWindow
{
public:
    QWidget *centralwidget;
    QPushButton *ExitButton;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *APIKeySetButton;
    QPushButton *AISetButton;
    QMdiArea *mdiArea;
    QMdiArea *mdiArea_2;
    QMdiArea *mdiArea_3;
    QMdiArea *mdiArea_4;
    QFrame *ApiSetFrame;
    QTextEdit *DeepseekAPIKey_textEdit;
    QLabel *Tips_label;
    QTextEdit *Qwen3ttsflashAPIKey_textEdit;
    QLabel *Tips_label_2;
    QPushButton *SaveAPIButton;
    QFrame *AISetFrame;
    QTextEdit *ChatRole_TimeoutDuration_textEdit;
    QLabel *Tips_label_3;
    QMdiArea *mdiArea_5;
    QLabel *Tips_label_4;
    QLabel *Tips_label_5;
    QTextEdit *ChatRole_TokenRestrictions_textEdit;
    QLabel *Tips_label_6;
    QMdiArea *mdiArea_6;
    QLabel *Tips_label_7;
    QLabel *Tips_label_8;
    QLabel *Tips_label_9;
    QTextEdit *AIBoss_TimeoutDuration_textEdit;
    QTextEdit *AIManager_TimeoutDuration_textEdit;
    QTextEdit *AISecurityOfficer_TimeoutDuration_textEdit;
    QTextEdit *AIBoss_TokenRestriction_textEdit;
    QTextEdit *AIManager_TokenRestriction_textEdit;
    QTextEdit *AISecurityOfficer_TokenRestriction_textEdit;
    QLabel *Tips_label_10;
    QLabel *Tips_label_11;
    QLabel *Tips_label_12;
    QMdiArea *mdiArea_7;
    QMdiArea *mdiArea_8;
    QMdiArea *mdiArea_9;
    QPushButton *SaveAIButton;

    void setupUi(QMainWindow *ConfigWindow)
    {
        if (ConfigWindow->objectName().isEmpty())
            ConfigWindow->setObjectName("ConfigWindow");
        ConfigWindow->resize(900, 500);
        centralwidget = new QWidget(ConfigWindow);
        centralwidget->setObjectName("centralwidget");
        ExitButton = new QPushButton(centralwidget);
        ExitButton->setObjectName("ExitButton");
        ExitButton->setGeometry(QRect(10, 440, 161, 41));
        QFont font;
        font.setPointSize(11);
        ExitButton->setFont(font);
        ExitButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 0, 161, 101));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        APIKeySetButton = new QPushButton(verticalLayoutWidget);
        APIKeySetButton->setObjectName("APIKeySetButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(APIKeySetButton->sizePolicy().hasHeightForWidth());
        APIKeySetButton->setSizePolicy(sizePolicy);
        APIKeySetButton->setMinimumSize(QSize(0, 40));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Yu Gothic UI")});
        font1.setPointSize(12);
        font1.setBold(true);
        APIKeySetButton->setFont(font1);
        APIKeySetButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(APIKeySetButton);

        AISetButton = new QPushButton(verticalLayoutWidget);
        AISetButton->setObjectName("AISetButton");
        AISetButton->setMinimumSize(QSize(0, 40));
        AISetButton->setFont(font1);
        AISetButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(AISetButton);

        mdiArea = new QMdiArea(centralwidget);
        mdiArea->setObjectName("mdiArea");
        mdiArea->setGeometry(QRect(0, 0, 181, 511));
        QBrush brush(QColor(66, 66, 66, 255));
        brush.setStyle(Qt::BrushStyle::FDiagPattern);
        mdiArea->setBackground(brush);
        mdiArea_2 = new QMdiArea(centralwidget);
        mdiArea_2->setObjectName("mdiArea_2");
        mdiArea_2->setGeometry(QRect(0, 0, 181, 511));
        QBrush brush1(QColor(61, 61, 61, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        mdiArea_2->setBackground(brush1);
        mdiArea_3 = new QMdiArea(centralwidget);
        mdiArea_3->setObjectName("mdiArea_3");
        mdiArea_3->setGeometry(QRect(180, 0, 721, 501));
        mdiArea_3->setAutoFillBackground(true);
        QBrush brush2(QColor(93, 93, 93, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        mdiArea_3->setBackground(brush2);
        mdiArea_3->setTabsClosable(false);
        mdiArea_3->setTabShape(QTabWidget::TabShape::Rounded);
        mdiArea_4 = new QMdiArea(centralwidget);
        mdiArea_4->setObjectName("mdiArea_4");
        mdiArea_4->setGeometry(QRect(180, 0, 2, 501));
        QBrush brush3(QColor(212, 212, 212, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        mdiArea_4->setBackground(brush3);
        ApiSetFrame = new QFrame(centralwidget);
        ApiSetFrame->setObjectName("ApiSetFrame");
        ApiSetFrame->setEnabled(true);
        ApiSetFrame->setGeometry(QRect(230, 40, 611, 151));
        ApiSetFrame->setStyleSheet(QString::fromUtf8(""));
        ApiSetFrame->setFrameShape(QFrame::Shape::NoFrame);
        ApiSetFrame->setFrameShadow(QFrame::Shadow::Plain);
        DeepseekAPIKey_textEdit = new QTextEdit(ApiSetFrame);
        DeepseekAPIKey_textEdit->setObjectName("DeepseekAPIKey_textEdit");
        DeepseekAPIKey_textEdit->setGeometry(QRect(200, 5, 391, 31));
        DeepseekAPIKey_textEdit->setStyleSheet(QString::fromUtf8(""));
        DeepseekAPIKey_textEdit->setFrameShape(QFrame::Shape::HLine);
        DeepseekAPIKey_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        Tips_label = new QLabel(ApiSetFrame);
        Tips_label->setObjectName("Tips_label");
        Tips_label->setGeometry(QRect(10, 10, 161, 21));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Yu Gothic UI")});
        font2.setPointSize(13);
        font2.setBold(false);
        font2.setItalic(false);
        Tips_label->setFont(font2);
        Tips_label->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        Qwen3ttsflashAPIKey_textEdit = new QTextEdit(ApiSetFrame);
        Qwen3ttsflashAPIKey_textEdit->setObjectName("Qwen3ttsflashAPIKey_textEdit");
        Qwen3ttsflashAPIKey_textEdit->setGeometry(QRect(200, 55, 391, 31));
        Qwen3ttsflashAPIKey_textEdit->setStyleSheet(QString::fromUtf8("border-color: rgba(255, 255, 255, 0);"));
        Qwen3ttsflashAPIKey_textEdit->setFrameShape(QFrame::Shape::VLine);
        Qwen3ttsflashAPIKey_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        Tips_label_2 = new QLabel(ApiSetFrame);
        Tips_label_2->setObjectName("Tips_label_2");
        Tips_label_2->setGeometry(QRect(10, 60, 191, 21));
        Tips_label_2->setFont(font2);
        Tips_label_2->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        SaveAPIButton = new QPushButton(ApiSetFrame);
        SaveAPIButton->setObjectName("SaveAPIButton");
        SaveAPIButton->setGeometry(QRect(460, 100, 131, 31));
        SaveAPIButton->setFont(font1);
        AISetFrame = new QFrame(centralwidget);
        AISetFrame->setObjectName("AISetFrame");
        AISetFrame->setEnabled(true);
        AISetFrame->setGeometry(QRect(230, 20, 631, 471));
        AISetFrame->setStyleSheet(QString::fromUtf8(""));
        AISetFrame->setFrameShape(QFrame::Shape::NoFrame);
        AISetFrame->setFrameShadow(QFrame::Shadow::Plain);
        ChatRole_TimeoutDuration_textEdit = new QTextEdit(AISetFrame);
        ChatRole_TimeoutDuration_textEdit->setObjectName("ChatRole_TimeoutDuration_textEdit");
        ChatRole_TimeoutDuration_textEdit->setGeometry(QRect(230, 55, 61, 31));
        ChatRole_TimeoutDuration_textEdit->setStyleSheet(QString::fromUtf8(""));
        ChatRole_TimeoutDuration_textEdit->setFrameShape(QFrame::Shape::HLine);
        ChatRole_TimeoutDuration_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        Tips_label_3 = new QLabel(AISetFrame);
        Tips_label_3->setObjectName("Tips_label_3");
        Tips_label_3->setGeometry(QRect(10, 10, 131, 21));
        Tips_label_3->setFont(font2);
        Tips_label_3->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        mdiArea_5 = new QMdiArea(AISetFrame);
        mdiArea_5->setObjectName("mdiArea_5");
        mdiArea_5->setGeometry(QRect(0, 40, 611, 2));
        QBrush brush4(QColor(255, 255, 255, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        mdiArea_5->setBackground(brush4);
        Tips_label_4 = new QLabel(AISetFrame);
        Tips_label_4->setObjectName("Tips_label_4");
        Tips_label_4->setGeometry(QRect(10, 60, 231, 21));
        Tips_label_4->setFont(font2);
        Tips_label_4->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        Tips_label_5 = new QLabel(AISetFrame);
        Tips_label_5->setObjectName("Tips_label_5");
        Tips_label_5->setGeometry(QRect(10, 100, 211, 21));
        Tips_label_5->setFont(font2);
        Tips_label_5->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        ChatRole_TokenRestrictions_textEdit = new QTextEdit(AISetFrame);
        ChatRole_TokenRestrictions_textEdit->setObjectName("ChatRole_TokenRestrictions_textEdit");
        ChatRole_TokenRestrictions_textEdit->setGeometry(QRect(230, 95, 61, 31));
        ChatRole_TokenRestrictions_textEdit->setStyleSheet(QString::fromUtf8(""));
        ChatRole_TokenRestrictions_textEdit->setFrameShape(QFrame::Shape::HLine);
        ChatRole_TokenRestrictions_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        Tips_label_6 = new QLabel(AISetFrame);
        Tips_label_6->setObjectName("Tips_label_6");
        Tips_label_6->setGeometry(QRect(10, 180, 131, 21));
        Tips_label_6->setFont(font2);
        Tips_label_6->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        mdiArea_6 = new QMdiArea(AISetFrame);
        mdiArea_6->setObjectName("mdiArea_6");
        mdiArea_6->setGeometry(QRect(0, 210, 611, 2));
        mdiArea_6->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        mdiArea_6->setBackground(brush4);
        Tips_label_7 = new QLabel(AISetFrame);
        Tips_label_7->setObjectName("Tips_label_7");
        Tips_label_7->setGeometry(QRect(30, 230, 51, 21));
        Tips_label_7->setFont(font2);
        Tips_label_7->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        Tips_label_8 = new QLabel(AISetFrame);
        Tips_label_8->setObjectName("Tips_label_8");
        Tips_label_8->setGeometry(QRect(200, 230, 131, 21));
        Tips_label_8->setFont(font2);
        Tips_label_8->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        Tips_label_9 = new QLabel(AISetFrame);
        Tips_label_9->setObjectName("Tips_label_9");
        Tips_label_9->setGeometry(QRect(430, 230, 131, 21));
        Tips_label_9->setFont(font2);
        Tips_label_9->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        AIBoss_TimeoutDuration_textEdit = new QTextEdit(AISetFrame);
        AIBoss_TimeoutDuration_textEdit->setObjectName("AIBoss_TimeoutDuration_textEdit");
        AIBoss_TimeoutDuration_textEdit->setGeometry(QRect(210, 270, 111, 31));
        AIBoss_TimeoutDuration_textEdit->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        AIBoss_TimeoutDuration_textEdit->setStyleSheet(QString::fromUtf8(""));
        AIBoss_TimeoutDuration_textEdit->setFrameShape(QFrame::Shape::HLine);
        AIBoss_TimeoutDuration_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        AIManager_TimeoutDuration_textEdit = new QTextEdit(AISetFrame);
        AIManager_TimeoutDuration_textEdit->setObjectName("AIManager_TimeoutDuration_textEdit");
        AIManager_TimeoutDuration_textEdit->setGeometry(QRect(210, 320, 111, 31));
        AIManager_TimeoutDuration_textEdit->setStyleSheet(QString::fromUtf8(""));
        AIManager_TimeoutDuration_textEdit->setFrameShape(QFrame::Shape::HLine);
        AIManager_TimeoutDuration_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        AISecurityOfficer_TimeoutDuration_textEdit = new QTextEdit(AISetFrame);
        AISecurityOfficer_TimeoutDuration_textEdit->setObjectName("AISecurityOfficer_TimeoutDuration_textEdit");
        AISecurityOfficer_TimeoutDuration_textEdit->setGeometry(QRect(210, 370, 111, 31));
        AISecurityOfficer_TimeoutDuration_textEdit->setStyleSheet(QString::fromUtf8(""));
        AISecurityOfficer_TimeoutDuration_textEdit->setFrameShape(QFrame::Shape::HLine);
        AISecurityOfficer_TimeoutDuration_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        AIBoss_TokenRestriction_textEdit = new QTextEdit(AISetFrame);
        AIBoss_TokenRestriction_textEdit->setObjectName("AIBoss_TokenRestriction_textEdit");
        AIBoss_TokenRestriction_textEdit->setGeometry(QRect(440, 270, 111, 31));
        AIBoss_TokenRestriction_textEdit->setStyleSheet(QString::fromUtf8(""));
        AIBoss_TokenRestriction_textEdit->setFrameShape(QFrame::Shape::HLine);
        AIBoss_TokenRestriction_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        AIManager_TokenRestriction_textEdit = new QTextEdit(AISetFrame);
        AIManager_TokenRestriction_textEdit->setObjectName("AIManager_TokenRestriction_textEdit");
        AIManager_TokenRestriction_textEdit->setGeometry(QRect(440, 320, 111, 31));
        AIManager_TokenRestriction_textEdit->setStyleSheet(QString::fromUtf8(""));
        AIManager_TokenRestriction_textEdit->setFrameShape(QFrame::Shape::HLine);
        AIManager_TokenRestriction_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        AISecurityOfficer_TokenRestriction_textEdit = new QTextEdit(AISetFrame);
        AISecurityOfficer_TokenRestriction_textEdit->setObjectName("AISecurityOfficer_TokenRestriction_textEdit");
        AISecurityOfficer_TokenRestriction_textEdit->setGeometry(QRect(440, 370, 111, 31));
        AISecurityOfficer_TokenRestriction_textEdit->setStyleSheet(QString::fromUtf8(""));
        AISecurityOfficer_TokenRestriction_textEdit->setFrameShape(QFrame::Shape::HLine);
        AISecurityOfficer_TokenRestriction_textEdit->setFrameShadow(QFrame::Shadow::Plain);
        Tips_label_10 = new QLabel(AISetFrame);
        Tips_label_10->setObjectName("Tips_label_10");
        Tips_label_10->setGeometry(QRect(30, 270, 141, 31));
        Tips_label_10->setFont(font2);
        Tips_label_10->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        Tips_label_11 = new QLabel(AISetFrame);
        Tips_label_11->setObjectName("Tips_label_11");
        Tips_label_11->setGeometry(QRect(30, 320, 141, 31));
        Tips_label_11->setFont(font2);
        Tips_label_11->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        Tips_label_12 = new QLabel(AISetFrame);
        Tips_label_12->setObjectName("Tips_label_12");
        Tips_label_12->setGeometry(QRect(30, 370, 141, 31));
        Tips_label_12->setFont(font2);
        Tips_label_12->setStyleSheet(QString::fromUtf8("font: 13pt \"Yu Gothic UI\";\n"
"color: rgb(255, 255, 255);"));
        mdiArea_7 = new QMdiArea(AISetFrame);
        mdiArea_7->setObjectName("mdiArea_7");
        mdiArea_7->setGeometry(QRect(20, 260, 570, 1));
        mdiArea_7->setStyleSheet(QString::fromUtf8(""));
        mdiArea_7->setBackground(brush4);
        mdiArea_8 = new QMdiArea(AISetFrame);
        mdiArea_8->setObjectName("mdiArea_8");
        mdiArea_8->setGeometry(QRect(20, 310, 570, 1));
        mdiArea_8->setBackground(brush4);
        mdiArea_9 = new QMdiArea(AISetFrame);
        mdiArea_9->setObjectName("mdiArea_9");
        mdiArea_9->setGeometry(QRect(20, 360, 570, 1));
        mdiArea_9->setBackground(brush4);
        SaveAIButton = new QPushButton(AISetFrame);
        SaveAIButton->setObjectName("SaveAIButton");
        SaveAIButton->setGeometry(QRect(490, 430, 131, 31));
        SaveAIButton->setFont(font1);
        ConfigWindow->setCentralWidget(centralwidget);
        mdiArea_3->raise();
        mdiArea_2->raise();
        mdiArea->raise();
        ExitButton->raise();
        verticalLayoutWidget->raise();
        mdiArea_4->raise();
        AISetFrame->raise();
        ApiSetFrame->raise();

        retranslateUi(ConfigWindow);

        QMetaObject::connectSlotsByName(ConfigWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ConfigWindow)
    {
        ConfigWindow->setWindowTitle(QCoreApplication::translate("ConfigWindow", "AppConfig", nullptr));
        ExitButton->setText(QCoreApplication::translate("ConfigWindow", "EXIT", nullptr));
        APIKeySetButton->setText(QCoreApplication::translate("ConfigWindow", "APIKey Setting", nullptr));
        AISetButton->setText(QCoreApplication::translate("ConfigWindow", "AI Setting", nullptr));
        Tips_label->setText(QCoreApplication::translate("ConfigWindow", "DeepSeek  APIKey \357\274\232", nullptr));
        Tips_label_2->setText(QCoreApplication::translate("ConfigWindow", "qwen3-tts-flashAPIKey \357\274\232", nullptr));
        SaveAPIButton->setText(QCoreApplication::translate("ConfigWindow", "Save", nullptr));
        Tips_label_3->setText(QCoreApplication::translate("ConfigWindow", "Chat role Setup", nullptr));
        Tips_label_4->setText(QCoreApplication::translate("ConfigWindow", "Network timeout duration \357\274\232", nullptr));
        Tips_label_5->setText(QCoreApplication::translate("ConfigWindow", "Token restrictions \357\274\232", nullptr));
        Tips_label_6->setText(QCoreApplication::translate("ConfigWindow", "AIAgent Setup", nullptr));
        Tips_label_7->setText(QCoreApplication::translate("ConfigWindow", "AIRole", nullptr));
        Tips_label_8->setText(QCoreApplication::translate("ConfigWindow", "TimeoutDuration", nullptr));
        Tips_label_9->setText(QCoreApplication::translate("ConfigWindow", "TokenRestrictions", nullptr));
        Tips_label_10->setText(QCoreApplication::translate("ConfigWindow", "AIBoss", nullptr));
        Tips_label_11->setText(QCoreApplication::translate("ConfigWindow", "AIManager", nullptr));
        Tips_label_12->setText(QCoreApplication::translate("ConfigWindow", "AISecurityOfficer", nullptr));
        SaveAIButton->setText(QCoreApplication::translate("ConfigWindow", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigWindow: public Ui_ConfigWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGWINDOW_H
