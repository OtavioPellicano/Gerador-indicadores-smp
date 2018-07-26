#ifndef PLANOAMOSTRAL_H
#define PLANOAMOSTRAL_H

#include <QDialog>
#include <vector>
#include <QString>
#include <algorithm>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <fstream>
#include <QDebug>
#include <QStringBuilder>
#include <QDir>

typedef std::vector<QString> vecQStr;

typedef std::vector<std::pair<QString, QString>> vecQStrPair;

namespace Ui {
class PlanoAmostral;
}

class PlanoAmostral : public QDialog
{
    Q_OBJECT

public:
    explicit PlanoAmostral(QWidget *parent = 0);
    ~PlanoAmostral();

    vecQStrPair vecPrestadoraUf() const;

private:
    bool validarPrest(const QString& qstrPrest);
    bool validarUf(const QString& qstrUf);
    bool validarPA(const QString& paFile);
    void atualizarTabelaPA(const vecQStrPair &vecPrestadoraUf);
    void carregarTabelaAtualDoArquivo(vecQStrPair& vecPrestadoraUf);
    QString messagemBoxForaFormato();
    void carregarTabelaAtual(vecQStrPair &vecUfPrestadora);
    bool salvarPA(const vecQStrPair &vecPrestadoraUf);

private slots:

    void on_pushButton_importar_clicked();

    void on_pushButton_aplicar_clicked();

private:
    Ui::PlanoAmostral *ui;

    vecQStrPair mVecPrestadoraUf;

    vecQStr mVecErroPrest;
    vecQStr mVecErroUf;

    const QString finaNamePA = "pa_smp.csv";


    vecQStr mVecPrestStd = {
        "ALGAR",
        "CLARO",
        "NEXTEL",
        "OI",
        "PORTO_CONECTA",
        "SERCOMTEL",
        "TIM",
        "VIVO"
    };

    vecQStr mVecUfStd = {
        "AC",
        "AL",
        "AM",
        "AP",
        "BA",
        "CE",
        "DF",
        "ES",
        "GO",
        "MA",
        "MG",
        "MS",
        "MT",
        "PA",
        "PB",
        "PE",
        "PI",
        "PR",
        "RJ",
        "RN",
        "RO",
        "RR",
        "RS",
        "SC",
        "SE",
        "SP",
        "TO"
    };
};

#endif // PLANOAMOSTRAL_H
