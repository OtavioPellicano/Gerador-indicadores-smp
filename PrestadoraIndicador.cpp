#include "PrestadoraIndicador.h"


PrestadoraIndicador::PrestadoraIndicador(const tipoPrestadora &tipo, const QString &sep)
    :mTipo(tipo), mSep(sep)
{

}

QString PrestadoraIndicador::nomePrestadora() const
{
    return mNomePrestadora;
}

void PrestadoraIndicador::setNomePrestadora(const QString &nomePrestadora)
{
    mNomePrestadora = nomePrestadora;
}

tipoPrestadora PrestadoraIndicador::tipo() const
{
    return mTipo;
}

void PrestadoraIndicador::setTipo(const tipoPrestadora &tipo)
{
    mTipo = tipo;
}

void PrestadoraIndicador::gerarMetaSmp(map<QString, map<QString, vector<tuple<bool, bool, double, double> > > > &mapUfMeta, const map<QString, map<QString, set<QString, less<QString> > > > &mapUfMedicao, map<QString, map<QString, map<QString, size_t> > >& /*mMapUfWanColetorMedicao*/)
{
    double down, up, downStream, upStream;
    size_t cont;
    QString uf, wanMode;

    pair<vector<double>, vector<double>> pairVecDownUp;
//    vector<QString> vecColetor;
//    vecColetor.reserve(7);

    pairVecDownUp.first.reserve(7);
    pairVecDownUp.second.reserve(7);
    mapUfMeta.clear();

    QStringList strCsv;
    for(auto itMap1 = mapUfMedicao.begin(); itMap1 != mapUfMedicao.end(); ++itMap1)
    {
        uf = itMap1->first;
        for(auto itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2)
        {
            wanMode = itMap2->first;
            cont = 0;
            pairVecDownUp.first.clear();
            pairVecDownUp.first.reserve(7);
            pairVecDownUp.second.clear();
            pairVecDownUp.second.reserve(7);
            for(auto itVec = itMap2->second.begin(); itVec != itMap2->second.end(); ++itVec)
            {
                strCsv = (*itVec).split(sep());
                down = strCsv[2].toDouble();
                up = strCsv[3].toDouble();

//                vecColetor.push_back(strCsv[1]);
                pairVecDownUp.first.push_back(down);
                pairVecDownUp.second.push_back(up);

                if((++cont % 7) == 0)
                {
                    downStream = strCsv[4].toDouble();
                    upStream = strCsv[5].toDouble();
                    down = limiteVelocidade(mediana(pairVecDownUp.first), downStream);
                    up = limiteVelocidade(mediana(pairVecDownUp.second), upStream);
                    if(quarentena(down, downStream))
                    {
                        mapUfMeta[uf][wanMode].push_back(tuple<bool, bool, double, double>(meta(down, downStream), meta(up, upStream),
                                                                                           down/downStream, up/upStream));

                        //carregando o map de quantidade de coletores e mediçoes validas
//                        for(auto itVecCol = vecColetor.begin(); itVecCol != vecColetor.end(); ++itVecCol)
//                        {
//                            ++mMapUfWanColetorMedicao[uf][wanMode][*itVecCol];
//                        }

                    }

                    pairVecDownUp.first.clear();
                    pairVecDownUp.first.reserve(7);
                    pairVecDownUp.second.clear();
                    pairVecDownUp.second.reserve(7);
//                    vecColetor.clear();
//                    vecColetor.reserve(7);
                }
            }
        }
    }

}

void PrestadoraIndicador::gerarColetoresMedicoesPorUf(map<QString, map<QString, size_t> > &mapUfColetor, map<QString, map<QString, size_t> > &mapUfQntMedicao, map<QString, map<QString, double> > &mapUfErro, const map<QString, map<QString, set<QString, less<QString> > > > &mapUfMedicao)
{
    QString uf, wanMode;
    QStringList strCsv;
    set<QString> setColetores;
    size_t qnt;

    for(auto itMap1 = mapUfMedicao.begin(); itMap1 != mapUfMedicao.end(); ++itMap1)
    {
        uf = itMap1->first;
        setColetores.clear();
        for(auto itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2)
        {
            wanMode = itMap2->first;
            qnt = 0;
            for(auto itSet = itMap2->second.begin(); itSet != itMap2->second.end(); ++itSet)
            {
                strCsv = (*itSet).split(sep());
                setColetores.insert(strCsv[1]);
                ++qnt;
            }
            mapUfQntMedicao[uf][wanMode] = qnt;
        }
        mapUfColetor[uf]["3G"] = setColetores.size();
        mapUfColetor[uf]["4G"] = setColetores.size();

        mapUfErro[uf]["3G"] = erroEstatistico(setColetores.size());
        mapUfErro[uf]["4G"] = erroEstatistico(setColetores.size());

    }
}

void PrestadoraIndicador::gerarColetoresMedicoesErroPorUf(map<QString, map<QString, size_t> > &mapUfQntColetor, map<QString, map<QString, size_t> > &mapUfQntMedicao, map<QString, map<QString, double> > &mapUfErro, const map<QString, map<QString, map<QString, size_t> > > &mMapUfWanColetorMedicao)
{
    QString uf;
    QString wanMode;
    set<QString> sColetor;
    for(auto itMap1 = mMapUfWanColetorMedicao.begin(); itMap1 != mMapUfWanColetorMedicao.end(); ++itMap1)
    {
        uf = itMap1->first;
        sColetor.clear();
        for(auto itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2)
        {
            wanMode = itMap2->first;

            for(auto itMap3 = itMap2->second.begin(); itMap3 != itMap2->second.end(); ++itMap3)
            {
                sColetor.insert(itMap3->first);
                mapUfQntMedicao[uf][wanMode] += itMap3->second;
            }
        }

        mapUfQntColetor[uf]["3G"] = sColetor.size();
        mapUfQntColetor[uf]["4G"] = sColetor.size();

        mapUfErro[uf]["3G"] = erroEstatistico(sColetor.size());
        mapUfErro[uf]["4G"] = erroEstatistico(sColetor.size());

    }
}

void PrestadoraIndicador::gerarIndiceSmp(map<QString, map<QString, tuple<size_t, size_t, double, double> > > &mapUfIndiceSmp, const map<QString, map<QString, vector<tuple<bool, bool, double, double> > > > &mapUfMeta)
{
    QString uf, wanMode;

    size_t indice_39, indice_40;
    double indice_41, indice_42;

    bool metaDown, metaUp;
    double quocienteDown, quocienteUp;

    for(auto itMap1 = mapUfMeta.begin(); itMap1 != mapUfMeta.end(); ++itMap1)
    {
        uf = itMap1->first;
        for(auto itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2)
        {
            wanMode = itMap2->first;
            indice_40 = itMap2->second.size();
            indice_41 = 0;
            indice_42 = 0;
            indice_39 = 0;
            for(auto itVec = itMap2->second.begin(); itVec != itMap2->second.end(); ++itVec)
            {
                metaDown = std::get<0>(*itVec);
                metaUp = std::get<1>(*itVec);
                quocienteDown = std::get<2>(*itVec);
                quocienteUp = std::get<3>(*itVec);

                if(metaDown && metaUp)
                {
                    ++indice_39;
                }

                indice_41 += quocienteDown;
                indice_42 += quocienteUp;
            }

            mapUfIndiceSmp[uf][wanMode] = tuple<size_t, size_t, double, double>(
                        indice_39, indice_40, indice_41, indice_42);

        }

    }

}

void PrestadoraIndicador::gerarIndicadorSmp(map<QString, map<QString, tuple<double, double, double> > > &mapUfIndicadorSmp, const map<QString, map<QString, tuple<size_t, size_t, double, double> > > &mapUfIndiceSmp)
{
    QString uf;

    double smp_4, smp_5_down, smp_5_up;

    size_t indice_39, indice_40;
    double indice_41, indice_42;

    //O Indicador é por UF!!!!
    for(auto itMap1 = mapUfIndiceSmp.begin(); itMap1 != mapUfIndiceSmp.end(); ++itMap1)
    {
        uf = itMap1->first;
        indice_39 = 0;
        indice_40 = 0;
        indice_41 = 0;
        indice_42 = 0;
        for(auto itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2)
        {

            indice_39 += std::get<0>(itMap2->second);
            indice_40 += std::get<1>(itMap2->second);
            indice_41 += std::get<2>(itMap2->second);
            indice_42 += std::get<3>(itMap2->second);

        }

        smp_4 = double(indice_39) / double(indice_40);
        smp_5_down = indice_41 / double(indice_40);
        smp_5_up = indice_42 / double(indice_40);

        mapUfIndicadorSmp[uf]["3G"] = tuple<double, double, double>(smp_4, smp_5_down, smp_5_up);
        mapUfIndicadorSmp[uf]["4G"] = tuple<double, double, double>(smp_4, smp_5_down, smp_5_up);


    }

}

bool PrestadoraIndicador::quarentena(const double &down, const double &downStream)
{
    return down/downStream >= .1 ? true : false;
}

double PrestadoraIndicador::limiteVelocidade(const double &mediana, const double &medianaStream)
{
    return mediana/medianaStream > 1 ? medianaStream : mediana;
}

bool PrestadoraIndicador::meta(const double &mediana, const double &medianaStream)
{
    return mediana/medianaStream >= .4 ? true : false;
}

/**
 * @brief PrestadoraIndicador::mediana
 * @param meds
 * @return
 * Retornar a mediana de um conjunto de valores
 */
double PrestadoraIndicador::mediana(vector<double> &meds)
{
    std::sort(meds.begin(), meds.end());
    size_t tam = meds.size();

    if(tam%2 == 0)
    {
        if(tam == 2)
            return (meds[0] + meds[1])/2;
        return (meds[tam/2] + meds[tam/2 - 1])/2;
    }
    else
    {
        return meds[tam/2];
    }

}

double PrestadoraIndicador::mediana(vector<double> &meds, size_t pos[])
{
    size_t tam = meds.size();

    std::map<size_t, double> mapPosMeds;
    for(size_t i = 0; i < meds.size(); ++i)
    {
        mapPosMeds[i] = meds[i];
    }

    std::map<double, size_t> mapMedsPos;
    for(auto itMap = mapPosMeds.begin(); itMap != mapPosMeds.end(); ++itMap)
    {
        mapMedsPos.insert({itMap->second, itMap->first});
    }

    if(tam%2 == 0)
    {
        if(tam == 2)
        {
            pos[0] = 0;
            pos[1] = 1;
            return (mapPosMeds[pos[0]] + mapPosMeds[pos[1]])/2;
        }

        pos[0] = mapMedsPos[tam/2];
        pos[1] = mapMedsPos[tam/2 - 1];

        return (mapPosMeds[*pos] + mapPosMeds[pos[1]])/2;
    }
    else
    {
        pos[0] = mapMedsPos[tam/2];
        pos[1] = pos[0];
        return mapPosMeds[*pos];
    }



}

double PrestadoraIndicador::erroEstatistico(const size_t &n)
{
    double num;
    num = pow(1.64485362695147, 2)*.95*(1 - 0.95);

    return sqrt(num/double(n));
}

vector<QString> PrestadoraIndicador::informacaoEstruturada()
{
    map<QString, map<QString, tuple<double, double, double>>> &mapUfIndicadorSmp = this->mMapUfIndicadorSmp;
    map<QString, map<QString, tuple<size_t, size_t, double, double>>> &mapUfIndiceSmp = this->mMapUfIndiceSmp;
    map<QString, map<QString, size_t>> &mapUfQntColetor = this->mMapUfQntColetor;
    map<QString, map<QString, size_t>> &mapUfQntMedicao = this->mMapUfQntMedicao;
    map<QString, map<QString, double>> &mapUfErro = this->mMapUfErro;

    vector<QString> vecSaida;
    vecSaida.reserve(55);

    QString cabecalho = "Prestadora" % sep() % "Tipo" % sep() % "UF" % sep() % "wanmode" % sep() % "medicoes_validas" % sep() %
            "coletores_validos" % sep() % "indice_39" % sep() % "indice_40" % sep() % "indice_41" % sep() %
            "indice_42" % sep() % "smp_10" % sep() % "smp_11_down" % sep() % "smp_11_up" % sep() % "erro_estatistico" % sep() %
            "flag_meta_smp_10" % sep() % "flag_meta_smp_11_down" % sep() % "flag_meta_smp_11_up" % sep() %
            "flag_val_op" % sep() % "flag_val_es" % sep() % "flag_res";

    if(mPrimeiroSalvamento)
        vecSaida.push_back(cabecalho);

    QString uf, wanMode;
    QString indice_39, indice_40, indice_41, indice_42;
    QString smp_10, smp_11_down, smp_11_up;
    QString qntColetores, qntMedicoes;
    QString erro;
    QString prestadora = this->nomePrestadora();
    QString tipo = (this->mTipo == SMP ? "SMP" : "SCM");
    QString dado;
    char flag_val_op, flag_val_es, flag_meta_10, flag_meta_11_down, flag_meta_11_up;
    QString flag_res;

    for(auto itMap1 = mapUfIndicadorSmp.begin(); itMap1 != mapUfIndicadorSmp.end(); ++itMap1)
    {
        uf = itMap1->first;
        for(auto itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2)
        {
            wanMode = itMap2->first;

            indice_39 = QString::number(std::get<0>(mapUfIndiceSmp[uf][wanMode]));
            indice_40 = QString::number(std::get<1>(mapUfIndiceSmp[uf][wanMode]));
            indice_41 = QString::number(std::get<2>(mapUfIndiceSmp[uf][wanMode]));
            indice_42 = QString::number(std::get<3>(mapUfIndiceSmp[uf][wanMode]));

            smp_10 = QString::number(std::get<0>(itMap2->second));
            smp_11_down = QString::number(std::get<1>(itMap2->second));
            smp_11_up = QString::number(std::get<2>(itMap2->second));

            qntColetores = QString::number(mapUfQntColetor[uf][wanMode]);
            qntMedicoes = QString::number(mapUfQntMedicao[uf][wanMode]);

            erro = QString::number(mapUfErro[uf][wanMode]);

            if((mapUfQntMedicao[uf]["3G"] + mapUfQntMedicao[uf]["4G"]) >= 2430)
            {
                flag_val_op = 1;
            }
            else
            {
                flag_val_op = 0;
            }

            if(mapUfErro[uf][wanMode] < 0.07)
            {
                flag_val_es = 1;
            }
            else
            {
                flag_val_es = 0;
            }

            if(std::get<0>(itMap2->second) >= .95)
            {
                flag_meta_10 = 1;
            }
            else
            {
                flag_meta_10 = 0;
            }

            if(std::get<1>(itMap2->second) >= .8)
            {
                flag_meta_11_down = 1;
            }
            else
            {
                flag_meta_11_down = 0;
            }

            if(std::get<2>(itMap2->second) >= .8)
            {
                flag_meta_11_up = 1;
            }
            else
            {
                flag_meta_11_up = 0;
            }

            if(!flag_val_op)
            {
                flag_res = "erro_val_op";
            }
            else
            {
                if(!flag_val_es)
                {
                    flag_res = "erro_val_es";
                }
                else
                {
                    flag_res = "ind_valido";
                }
            }

            dado = prestadora % sep() % tipo % sep() % uf % sep() % wanMode % sep() % qntMedicoes % sep() %
                        qntColetores % sep() % indice_39 % sep() % indice_40 % sep() % indice_41 % sep() %
                        indice_42 % sep() % smp_10 % sep() % smp_11_down % sep() % smp_11_up % sep() % erro % sep() %
                        QString::number(flag_meta_10) % sep() % QString::number(flag_meta_11_down) % sep() % QString::number(flag_meta_11_up) % sep() %
                        QString::number(flag_val_op) % sep() % QString::number(flag_val_es) % sep() % flag_res;

            vecSaida.push_back(dado);
        }

    }

    vecSaida.shrink_to_fit();

    return vecSaida;

}

void PrestadoraIndicador::clearAll()
{
    mMapUfMedicao.clear();

    //map quantidade de coletores por UF
    mMapUfQntColetor.clear();

    //map quantidade de medicao por UF por wanmode
    mMapUfQntMedicao.clear();

    //map erro por uf
    mMapUfErro.clear();

    //map com metaDown, metaUp, quocienteDown e quocienteUp, respectivamente
    mMapUfMeta.clear();

    //map com indice_39, indice_40, indice_41 e indice_42, respectivamente
    mMapUfIndiceSmp.clear();

    //map com indicador smp_4, smp_5_down, smp_5_up. indicador por UF!
    mMapUfIndicadorSmp.clear();

    //map[uf][wanMode][coletor] = qnt
    mMapUfWanColetorMedicao.clear();

}

QString PrestadoraIndicador::sep() const
{
    return mSep;
}

void PrestadoraIndicador::setSep(const QString &sep)
{
    mSep = sep;
}

/**
 * @brief PrestadoraIndicador::carregarMedicao
 * @param medicao
 * separo as medições da prestadora por uf e wanmode
 */
void PrestadoraIndicador::addMedicao(const QString &medicao)
{

    Medicao med(medicao, tipo(), sep());

    if(!med.medicaoValida().isEmpty())
    {
        this->mMapUfMedicao[med.uf()][med.wanMode()].insert(med.medicaoValida());
        if(!this->mPrestadoraAtualizada)
        {
            this->mPrestadoraAtualizada = true;
            this->mNomePrestadora = med.prestadora();
        }
    }

}

void PrestadoraIndicador::addMedicao(const QString& medicao, const map<QString, QString>& mapRegInaCodDes)
{
    Medicao med(medicao, tipo(), sep());

    if(!med.medicaoValida(mapRegInaCodDes).isEmpty())
    {
        this->mMapUfMedicao[med.uf()][med.wanMode()].insert(med.medicaoValida(mapRegInaCodDes));
        if(!this->mPrestadoraAtualizada)
        {
            this->mPrestadoraAtualizada = true;
            this->mNomePrestadora = med.prestadora();
        }
    }
}


bool PrestadoraIndicador::carregarMedicoes(const QDir& dirIn, const map<QString, QString> &mapRegInaCodDes)
{
    QStringList arqs = dirIn.entryList(QStringList("*.csv"));
    string str;
    QString qstr;

    ifstream arq;
    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        arq.open(dirIn.absoluteFilePath(*itQStr).toStdString());

        if(arq.is_open())
        {
            while (getline(arq, str)) {
                qstr = QString::fromStdString(str);
                this->addMedicao(qstr, mapRegInaCodDes);
            }
            arq.close();
        }
        else
        {
            qDebug() << "Erro ao abrir o arquivo de origem: " << *itQStr;
            return false;
        }

    }

    return true;

}

bool PrestadoraIndicador::carregarMedicoes(const QDir &dirIn)
{
    QStringList arqs = dirIn.entryList(QStringList("*.csv"));
    string str;
    QString qstr;

    ifstream arq;
    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        arq.open(dirIn.absoluteFilePath(*itQStr).toStdString());

        if(arq.is_open())
        {
            while (getline(arq, str)) {
                qstr = QString::fromStdString(str);
                this->addMedicao(qstr);
            }
            arq.close();
        }
        else
        {
            qDebug() << "Erro ao abrir o arquivo de origem: " << *itQStr;
            return false;
        }

    }

    return true;

}

bool PrestadoraIndicador::carregarSalvarMedicoes(const vector<QDir> &vecDirIn, const QDir &dirOut)
{
    for(auto itDir = vecDirIn.begin(); itDir != vecDirIn.end(); ++itDir)
    {
        clearAll();
        this->mPrestadoraAtualizada = false;
        if(!carregarMedicoes(*itDir))
            return false;
        salvarMedicoes(dirOut);
    }
    return true;
}

bool PrestadoraIndicador::salvarMedicoes(const QString &fullPath)
{
    vector<QString> vecResumo(indicadores());
//    bool mPrimeiroSalvamento = true;

    ofstream arq;

    if(mPrimeiroSalvamento)
    {
        arq.open(fullPath.toStdString());
        mPrimeiroSalvamento = false;
    }
    else
    {
        arq.open(fullPath.toStdString(), ios_base::app);
    }



    if(arq.is_open())
    {
        for(auto itQStr = vecResumo.begin(); itQStr != vecResumo.end(); ++itQStr)
        {
            arq << (*itQStr).toStdString() << endl;
        }
        arq.close();
    }
    else
    {
        qDebug() << "Erro ao abrir o arquivo de origem: " << fullPath;
        return false;
    }

    return true;
}

bool PrestadoraIndicador::salvarMedicoes(const QDir &dirOut)
{
//    QString arqNomeSaida = this->nomePrestadora() % "_resumo.csv";
    QString arqNomeSaida = "indices_indicadores_smp.csv";
    if(salvarMedicoes(dirOut.absoluteFilePath(arqNomeSaida)))
        return true;

    return false;
}

/**
 * @brief PrestadoraIndicador::indicadores
 * @return
 * retorna os indices e indicadores da prestadora por uf por wanmode
 */
vector<QString> PrestadoraIndicador::indicadores()
{
    gerarMetaSmp(this->mMapUfMeta, this->mMapUfMedicao, this->mMapUfWanColetorMedicao);
    gerarColetoresMedicoesPorUf(this->mMapUfQntColetor,this->mMapUfQntMedicao, this->mMapUfErro, this->mMapUfMedicao);
//    gerarColetoresMedicoesErroPorUf(this->mMapUfQntColetor, this->mMapUfQntMedicao, this->mMapUfErro, this->mMapUfWanColetorMedicao);
    this->mMapUfMedicao.clear();
    this->mMapUfWanColetorMedicao.clear();

    gerarIndiceSmp(this->mMapUfIndiceSmp, this->mMapUfMeta);
    this->mMapUfMeta.clear();

    gerarIndicadorSmp(this->mMapUfIndicadorSmp, this->mMapUfIndiceSmp);

    return informacaoEstruturada();
}
