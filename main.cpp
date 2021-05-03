#include <QApplication>
#include <string>

#include "window.h"

#pragma comment (lib, "opengl32.lib")

int main(int argc, char *argv[])
{
    std::string s;
    for (int i = 0; i < argc; i++)
        {
            if (!strcmp(argv[i], "-p") && (i + 1 < argc))
            {
                s = argv[i + 1];
            }
        }

    QApplication app(argc, argv);
    Window window(nullptr, s.c_str());
    window.show();
    return app.exec();
}
