#include "view.h"
#include <string>
#include <QApplication>

#pragma comment (lib, "opengl32.lib")

int main(int argc, char *argv[])
{
    std::string s;

    QApplication a(argc, argv);
    View w;
    QSurfaceFormat format;

    for (int i = 0; i < argc; i++)
        {
            if (!strcmp(argv[i], "-p") && (i + 1 < argc))
            {
                s = argv[i + 1];
            }
        }

    QSurfaceFormat::setDefaultFormat(format);

    w.setFormat(format);
    w.LoadData(s.c_str());
    w.show();
    return a.exec();
}
