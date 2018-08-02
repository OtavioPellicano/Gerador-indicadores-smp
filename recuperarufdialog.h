#ifndef RECUPERARUFDIALOG_H
#define RECUPERARUFDIALOG_H

#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include <QTest>
#include "recuperaruf.h"

namespace Ui {
class RecuperarUfDialog;
}

class RecuperarUfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecuperarUfDialog(const QDir& dirOrigem, QWidget *parent = 0);
    ~RecuperarUfDialog();

private slots:
    void on_checkBox_4g_stateChanged(int arg1);

    void on_pushButton_recuperar_uf_clicked();

private:
    Ui::RecuperarUfDialog *ui;
    QDir mDirOrigem;

    RecuperarUF* mRecUF;
};

#endif // RECUPERARUFDIALOG_H
