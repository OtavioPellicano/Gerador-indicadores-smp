#ifndef PRESTADORAINDICADOR_H
#define PRESTADORAINDICADOR_H

#include <QStringList>
#include <QStringBuilder>
#include <map>
#include <set>
#include <vector>
#include <functional>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <QDir>
#include <fstream>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <iostream>

#include "Medicao.h"


using namespace std;

//enum tipoPrestadora{SMP, SCM};

class PrestadoraIndicador
{
public:

    PrestadoraIndicador(const tipoPrestadora &tipo, const QString &sep = ";");

    QString sep() const;
    void setSep(const QString &sep);

    bool carregarMedicoes(const QDir &dirIn);

    bool carregarSalvarMedicoes(const vector<QDir> &vecDirIn, const QDir &dirOut);

    bool salvarMedicoes(const QString &fullPath);

    bool salvarMedicoes(const QDir &dirOut);

    vector<QString> indicadores();

private:

    void addMedicao(const QString &medicao);

    QString nomePrestadora() const;
    void setNomePrestadora(const QString &nomePrestadora);

    tipoPrestadora tipo() const;
    void setTipo(const tipoPrestadora &tipo);

    void gerarMetaSmp(map<QString, map<QString, vector<tuple<bool, bool, double, double>>>> &mapUfMeta,
                       const map<QString, map<QString, set<QString, less<QString>>>> &mapUfMedicao);

    void gerarColetoresMedicoesPorUf(map<QString, map<QString, size_t>> &mapUfColetor,
                             map<QString, map<QString, size_t>> &mapUfQntMedicao,
                             map<QString, map<QString, double>> &mapUfErro,
                             const map<QString, map<QString, set<QString, less<QString>>>> &mapUfMedicao);

    void gerarIndiceSmp(map<QString, map<QString, tuple<size_t, size_t, double, double> > > &mapUfIndiceSmp,
                        const map<QString, map<QString, vector<tuple<bool, bool, double, double>>>> &mapUfMeta);

    void gerarIndicadorSmp(map<QString, map<QString, tuple<double, double, double> > > &mapUfIndicadorSmp,
                           const map<QString, map<QString, tuple<size_t, size_t, double, double> > > &mapUfIndiceSmp);

    bool quarentena(const double &down, const double &downStream);

    double limiteVelocidade(const double &mediana, const double &medianaStream);

    bool meta(const double &mediana, const double &medianaStream);

    double mediana(vector<double> &meds);

    double erroEstatistico(const size_t &n);

    vector<QString> informacaoEstruturada();

    void clearAll();

private:
    QString mNomePrestadora;
    tipoPrestadora mTipo;

    bool mPrestadoraAtualizada = false;

    bool mPrimeiroSalvamento = true;

    QString mSep;

    map<QString, map<QString, set<QString, less<QString>>>> mMapUfMedicao;

    //map quantidade de coletores por UF
    map<QString, map<QString, size_t>> mMapUfQntColetor;

    //map quantidade de medicao por UF por wanmode
    map<QString, map<QString, size_t>> mMapUfQntMedicao;

    //map erro por uf
    map<QString, map<QString, double>> mMapUfErro;

    //map com metaDown, metaUp, quocienteDown e quocienteUp, respectivamente
    map<QString, map<QString, vector<tuple<bool, bool, double, double>>>> mMapUfMeta;

    //map com indice_39, indice_40, indice_41 e indice_42, respectivamente
    map<QString, map<QString, tuple<size_t, size_t, double, double> > > mMapUfIndiceSmp;

    //map com indicador smp_4, smp_5_down, smp_5_up. indicador por UF!
    map<QString, map<QString, tuple<double, double, double> > > mMapUfIndicadorSmp;

};

#endif // PRESTADORAINDICADOR_H
