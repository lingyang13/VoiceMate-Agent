/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *MicrophoneButton;
    QPushButton *TextInputButton;
    QPushButton *ChangeAIButton;
    QTextEdit *TestInputBox;
    QPushButton *SentTextButton;
    QPushButton *ComputerManagerAIButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setToolTipDuration(-1);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MicrophoneButton = new QPushButton(centralwidget);
        MicrophoneButton->setObjectName("MicrophoneButton");
        MicrophoneButton->setGeometry(QRect(57, 284, 40, 40));
        MicrophoneButton->setSizeIncrement(QSize(0, 0));
        MicrophoneButton->setBaseSize(QSize(0, 0));
        QFont font;
        font.setBold(false);
        MicrophoneButton->setFont(font);
        MicrophoneButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: #000000;\n"
"color: white;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("img/button/Button_1.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MicrophoneButton->setIcon(icon);
        MicrophoneButton->setIconSize(QSize(40, 40));
        MicrophoneButton->setCheckable(false);
        MicrophoneButton->setAutoRepeatInterval(100);
        MicrophoneButton->setAutoDefault(false);
        MicrophoneButton->setFlat(false);
        TextInputButton = new QPushButton(centralwidget);
        TextInputButton->setObjectName("TextInputButton");
        TextInputButton->setGeometry(QRect(10, 284, 40, 40));
        TextInputButton->setSizeIncrement(QSize(0, 0));
        TextInputButton->setBaseSize(QSize(0, 0));
        QFont font1;
        TextInputButton->setFont(font1);
        TextInputButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: #000000;\n"
"color: white;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("img/button/Button_3.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        TextInputButton->setIcon(icon1);
        TextInputButton->setIconSize(QSize(40, 40));
        TextInputButton->setCheckable(false);
        TextInputButton->setAutoRepeatInterval(100);
        TextInputButton->setAutoDefault(false);
        TextInputButton->setFlat(false);
        ChangeAIButton = new QPushButton(centralwidget);
        ChangeAIButton->setObjectName("ChangeAIButton");
        ChangeAIButton->setGeometry(QRect(104, 284, 40, 40));
        ChangeAIButton->setSizeIncrement(QSize(0, 0));
        ChangeAIButton->setBaseSize(QSize(0, 0));
        ChangeAIButton->setFont(font1);
        ChangeAIButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: #000000;\n"
"color: white;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("img/button/Button_5.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ChangeAIButton->setIcon(icon2);
        ChangeAIButton->setIconSize(QSize(40, 40));
        ChangeAIButton->setCheckable(false);
        ChangeAIButton->setAutoRepeatInterval(100);
        ChangeAIButton->setAutoDefault(false);
        ChangeAIButton->setFlat(false);
        TestInputBox = new QTextEdit(centralwidget);
        TestInputBox->setObjectName("TestInputBox");
        TestInputBox->setEnabled(true);
        TestInputBox->setGeometry(QRect(0, 330, 191, 71));
        SentTextButton = new QPushButton(centralwidget);
        SentTextButton->setObjectName("SentTextButton");
        SentTextButton->setGeometry(QRect(10, 410, 40, 40));
        SentTextButton->setSizeIncrement(QSize(0, 0));
        SentTextButton->setBaseSize(QSize(0, 0));
        SentTextButton->setFont(font1);
        SentTextButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: #000000;\n"
"color: white;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("img/button/Button_7.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        SentTextButton->setIcon(icon3);
        SentTextButton->setIconSize(QSize(40, 40));
        SentTextButton->setCheckable(false);
        SentTextButton->setAutoRepeatInterval(100);
        SentTextButton->setAutoDefault(false);
        SentTextButton->setFlat(false);
        ComputerManagerAIButton = new QPushButton(centralwidget);
        ComputerManagerAIButton->setObjectName("ComputerManagerAIButton");
        ComputerManagerAIButton->setGeometry(QRect(151, 284, 40, 40));
        ComputerManagerAIButton->setSizeIncrement(QSize(0, 0));
        ComputerManagerAIButton->setBaseSize(QSize(0, 0));
        ComputerManagerAIButton->setFont(font1);
        ComputerManagerAIButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: #000000;\n"
"color: white;"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("img/button/Button_9.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ComputerManagerAIButton->setIcon(icon4);
        ComputerManagerAIButton->setIconSize(QSize(40, 40));
        ComputerManagerAIButton->setCheckable(false);
        ComputerManagerAIButton->setAutoRepeatInterval(100);
        ComputerManagerAIButton->setAutoDefault(false);
        ComputerManagerAIButton->setFlat(false);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        MicrophoneButton->setDefault(false);
        TextInputButton->setDefault(false);
        ChangeAIButton->setDefault(false);
        SentTextButton->setDefault(false);
        ComputerManagerAIButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
#if QT_CONFIG(tooltip)
        MainWindow->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        MicrophoneButton->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        MicrophoneButton->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        MicrophoneButton->setText(QString());
        TextInputButton->setText(QString());
        ChangeAIButton->setText(QString());
        SentTextButton->setText(QString());
        ComputerManagerAIButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
