#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include "PrestadoraIndicador.h"
#include <QMessageBox>
#include <vector>
#include <QTableWidget>
#include <QStringList>
#include <algorithm>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    void updateTable();
    QString formatarPorcentagem(const QString& qstr);

private slots:
    void on_pushButton_buscar_clicked();

    void on_pushButton_processar_clicked();

    void on_pushButton_exportar_clicked();

private:
    Ui::Dialog *ui;

    QDir mDirIn;
    PrestadoraIndicador *mPrestInd;

    std::vector<QString> mVecIndicadores;

};

#endif // DIALOG_H
