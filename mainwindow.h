#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include "PrestadoraIndicador.h"
#include <QMessageBox>
#include <vector>
#include <QTableWidget>
#include <QStringList>
#include <algorithm>
#include "detalheindicador.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void updateTable();
    QString formatarPorcentagem(const QString& qstr);

    void atualizarCelulas();


private slots:

    void cellSelected(const int& row, const int&);

    void on_actionBuscarDiretorio_triggered();

    void on_actionSalvar_triggered();

    void on_actionProcessar_triggered();

    void on_actionSair_triggered();

private:
    Ui::MainWindow *ui;

    QDir mDirIn;
    PrestadoraIndicador *mPrestInd;

    std::vector<QString> mVecIndicadores;

    DetalheIndicador* mDetInd;
};

#endif // MAINWINDOW_H
