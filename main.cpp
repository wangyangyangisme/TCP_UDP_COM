#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif
#include <QTextCodec>
#include "mainwindow.h"
#include "mystyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif

    QApplication::addLibraryPath("./plugins");
    MainWindow w;
    QApplication::setStyle(new myStyle);
    w.show();

    return a.exec();
}
