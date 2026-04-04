#include <QtWidgets/QApplication>
#include "QTWidgetsApp.h"

#pragma comment(lib, "winmm.lib")  // 链接winmm库

int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);

    app.setApplicationName("VMAgent");
    app.setOrganizationName("LingYang13");
    app.setApplicationDisplayName("TablePet - 3.0");

    QTWidgetsApp window;
    window.show();

    return app.exec();
}

