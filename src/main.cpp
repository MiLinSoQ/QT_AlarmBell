#include "AlarmBell.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlarmBell *bell = new AlarmBell();
    bell->show();

    return a.exec();
}
