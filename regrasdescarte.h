#ifndef REGRASDESCARTE_H
#define REGRASDESCARTE_H

#include <QDialog>

namespace Ui {
class RegrasDescarte;
}

class RegrasDescarte : public QDialog
{
    Q_OBJECT

public:
    explicit RegrasDescarte(QWidget *parent = 0);
    ~RegrasDescarte();

private:
    Ui::RegrasDescarte *ui;
};

#endif // REGRASDESCARTE_H
