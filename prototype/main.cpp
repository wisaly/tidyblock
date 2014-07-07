#include "widget.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle("Fusion");

    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    Widget w;
    w.show();

    return a.exec();
}
