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
#include <QTest>
#include "detalheindicador.h"
#include "planoamostral.h"
#include "regrasdescarte.h"
#include "recuperarvelocidadecontratada.h"
#include "recuperarufdialog.h"

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

    void on_actionDesenvolvedor_triggered();

    void on_actionPlano_Amostral_triggered();

    void on_actionRegras_de_Descarte_triggered();

    void on_actionRecuperar_Velocidade_Contratada_triggered();

    void on_headerDoubleClicked(const int& col);

    void on_actionRecuperar_UF_triggered();

private:
    Ui::MainWindow *ui;

    QDir mDirIn;
    PrestadoraIndicador *mPrestInd;

    std::vector<QString> mVecIndicadores;

    DetalheIndicador* mDetInd;

    PlanoAmostral* mPA;

    RegrasDescarte* mRegDescarte;

    RecuperarVelocidadeContratada* mRecVelCont;

    RecuperarUfDialog* mRecUf;

};

#endif // MAINWINDOW_H
