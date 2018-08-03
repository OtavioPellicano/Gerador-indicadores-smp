#ifndef RECUPERARUF_H
#define RECUPERARUF_H

#include <QStringList>
#include <QDir>
#include <fstream>
#include <QDebug>
#include <vector>
#include "IdentificacaoDaUf.h"
#include <algorithm>

class RecuperarUF
{
private:
    enum solucao{AXIROS, NETMETRIC, NONE};
public:
    RecuperarUF();

    bool recuperar3G(const QDir& dirOrigem);
    bool recuperar4G(const QDir& dirOrigem, const QDir& dirRawdataBruto);

    QString totalMedRecup3G() const;
    QString totalMedRecupInicial3G() const;

private:
    bool descarregarMedicoes(std::vector<QString>& vecMed, const QString& fullPath);
    bool carregarMapChaveTac(std::map<QString, QString>& mapChaveTac, const QDir& dirRawdataBruto);
    solucao validarCabecalhoSolucao(const QStringList &strCsv);


private:

    const int LIMITE_SUPERIOR_CGI = 15;
    const int LIMITE_INFERIOR_CGI = 10;
    const int LIMITE_SUPERIOR_ECGI = 13;

    std::vector<QString> mVecHistSalvamento;

    long mTotalMed3G = 0;
    long mTotalMedSemRecupUf3G = 0;
    long mTotalMedUfBranco3G = 0;

};

#endif // RECUPERARUF_H
