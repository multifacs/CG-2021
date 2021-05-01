#include "view.h"

#include <QApplication>

#pragma comment (lib, "opengl32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    QSurfaceFormat format;

    //format.setDepthBufferSize(24);
    //format.setVersion(3, 5);
    //format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    w.setFormat(format);
    w.LoadData("FOURDIX-1.bin");
    w.show();
    return a.exec();
}
