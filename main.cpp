#include <QApplication>

#include "window.h"

#pragma comment (lib, "opengl32.lib")

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
