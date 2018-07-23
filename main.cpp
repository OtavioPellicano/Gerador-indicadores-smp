#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/img/mainico.png"));
    w.setWindowTitle("Gerador de Indicadores SMP");

    w.showMaximized();

    return a.exec();
}
