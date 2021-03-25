#include <QCoreApplication>
#include <string>

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
}
