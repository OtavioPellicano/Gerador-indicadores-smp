#ifndef DETALHEINDICADOR_H
#define DETALHEINDICADOR_H

#include <QDialog>

namespace Ui {
class DetalheIndicador;
}

class DetalheIndicador : public QDialog
{
    Q_OBJECT

public:
    explicit DetalheIndicador(QWidget *parent = 0);
    ~DetalheIndicador();

private:
    Ui::DetalheIndicador *ui;
};

#endif // DETALHEINDICADOR_H
