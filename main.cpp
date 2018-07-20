#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowIcon(QIcon(":/img/mainico.png"));
    w.setWindowTitle("Gerador de Medições SMP");

    w.show();

    return a.exec();
}
