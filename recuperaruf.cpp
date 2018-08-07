#include "recuperaruf.h"

RecuperarUF::RecuperarUF()
{

}

bool RecuperarUF::recuperar3G(const QDir &dirOrigem)
{
    QString cgi;/*55*/
    QString lac;
    QString uf;/*0*/
    QString wanMode;/*22*/

    QStringList strCsv;
    std::string str;
    std::vector<QString> vecMedUfRec;

    std::ifstream arq;
//    std::ofstream arqOut;
    QStringList arqs = dirOrigem.entryList(QStringList("*.csv"));

    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        arq.open(dirOrigem.absoluteFilePath(*itQStr).toStdString());
        if(arq.is_open())
        {
            while(std::getline(arq, str))
            {
                strCsv = QString::fromStdString(str).split(";");

                cgi = strCsv[55];
                uf = strCsv[0];
                wanMode = strCsv[22];


                if(wanMode == "3G")
                {
                    ++mTotalMed3G;
                    if(uf.isEmpty())
                    {
                        ++mTotalMedUfBranco3G;
                        //verificando CGI Valido
                        if(cgi.size() <= LIMITE_SUPERIOR_CGI && cgi.simplified() >= LIMITE_INFERIOR_CGI)
                        {
                            lac = cgi.mid(5,5);

                            opmm::IdentificacaoDaUf identUf;
                            identUf.setup(cgi.toStdString(), lac.toStdString(), opmm::LAC);
                            uf = QString::fromStdString(identUf.uf());

                            if(uf == "$PI$")
                            {
                                ++mTotalMedSemRecupUf3G;
                                uf = "";
                            }

                            strCsv[0] = uf;
                        }
                        else
                        {
                            ++mTotalMedSemRecupUf3G;
                        }
                    }
                }


                try {
                    vecMedUfRec.push_back(strCsv.join(";"));
                } catch (...) {
                    descarregarMedicoes(vecMedUfRec, dirOrigem.absoluteFilePath(*itQStr));
                    vecMedUfRec.push_back(strCsv.join(";"));
                }

            }

            arq.close();

        }
        else
        {
            qDebug() << "Erro recuperacao UF 3G (leitura): " << *itQStr;
            return false;
        }

        if(!vecMedUfRec.empty())
        {
            descarregarMedicoes(vecMedUfRec, dirOrigem.absoluteFilePath(*itQStr));
        }

    }

    mVecHistSalvamento.clear();
    return true;

}

bool RecuperarUF::recuperar4G(const QDir &dirOrigem, const QDir &dirRawdataBruto)
{
    std::map<QString, QString> mapChaveTacOuUF;

    if(!carregarMapChaveTacOuUF(mapChaveTacOuUF, dirRawdataBruto))
    {
        qDebug() << "Erro ao carregar o mapChaveTacOuUF";
        return false;
    }


    if(this->mTipoSolucao == AXIROS)
    {
        //recuperar Axiros
        if(!recuperar4GAxiros(dirOrigem, mapChaveTacOuUF))
        {
            qDebug() << "Erro ao recuperar TAC Axiros";
            return false;
        }

    }
    else if(this->mTipoSolucao == NETMETRIC)
    {
        //recuperar NETMETRIC
        if(!recuperar4GNetmetric(dirOrigem, mapChaveTacOuUF))
        {
            qDebug() << "Erro ao recuperar location Netmetric";
            return false;
        }
    }
    else
    {
        qDebug() << "Erro ao escolher solucao";
        return false;
    }



    return true;
}

bool RecuperarUF::descarregarMedicoes(std::vector<QString> &vecMed, const QString &fullPath)
{
    std::ofstream arq;
    size_t tam;

    if(std::find(mVecHistSalvamento.begin(), mVecHistSalvamento.end(), fullPath) != mVecHistSalvamento.end())
    {
        arq.open(fullPath.toStdString(), std::ios_base::app);
    }
    else
    {
        mVecHistSalvamento.push_back(fullPath);
        arq.open(fullPath.toStdString());
    }

    if(arq.is_open())
    {
        vecMed.shrink_to_fit();
        tam = vecMed.size();
        for(auto itVec = vecMed.begin(); itVec != vecMed.end(); ++itVec)
        {
            arq << (*itVec).toStdString() << std::endl;
        }
        arq.close();
    }
    else
    {
        return false;
    }

    vecMed.clear();
    vecMed.reserve(tam);

    return true;

}

bool RecuperarUF::carregarMapChaveTacOuUF(std::map<QString, QString> &mapChaveTacOuUF, const QDir &dirRawdataBruto)
{
    QStringList strCsv;
    solucao tipoSolucao;
    std::string str;
    size_t qntTipoSolucao = 0;

    QStringList arqs = dirRawdataBruto.entryList(QStringList("*.csv"));

    std::ifstream arq;

    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        arq.open(dirRawdataBruto.absoluteFilePath(*itQStr).toStdString());

        if(arq.is_open())
        {
            for(size_t i = 0; std::getline(arq, str); ++i)
            {
                strCsv = QString::fromStdString(str).split(";");

                if(i == 0)
                {
                    tipoSolucao = validarCabecalhoSolucao(strCsv);

                    if(this->mTipoSolucao != tipoSolucao)
                    {
                        if(++qntTipoSolucao > 1)
                        {
                            qDebug() << "Mais de uma solucao!";
                            return false;
                        }
                        this->mTipoSolucao = tipoSolucao;
                    }

                }

                switch (tipoSolucao) {
                case AXIROS:
                    carregarMapChaveTac(arq, mapChaveTacOuUF);
                    break;
                case NETMETRIC:
                    carregarMapChaveUF(arq, mapChaveTacOuUF);
                    break;
                default:
                    qDebug() << "Rawdata bruto não identificado: " << *itQStr;
                    return false;
                    break;
                }

            }
            arq.close();
        }
        else
        {
            qDebug() << "Erro ao ler arq rawdata bruto: " << *itQStr;
            return false;
        }

    }

    return true;
}

RecuperarUF::solucao RecuperarUF::validarCabecalhoSolucao(const QStringList& strCsv)
{
    if(strCsv[0] == "deviceid"
            && strCsv[1] == "timestamp"
            && strCsv[18] == "tac")
    {
        return AXIROS;
    }
    else if(strCsv[0] == "deviceID"
            && strCsv[16] == "dateTime"
            && strCsv[3] == "location")
    {
        return NETMETRIC;
    }
    else
    {
        return NONE;
    }
}

bool RecuperarUF::carregarMapChaveTac(ifstream &arq, std::map<QString, QString> &mapChaveTac)
{
    std::string str;
    QString deviceId;//[0] => split("-")[1]
    QString dateTime;//[1] => mid(0, 19)
    QString tac;//[18]
    QString chave;
    QStringList strCsv;

    while(std::getline(arq, str))
    {
        strCsv = QString::fromStdString(str).split(";");

        deviceId = strCsv[0].split("-")[1];
        dateTime = strCsv[1].mid(0,19);
        tac = strCsv[18];
        chave = deviceId % dateTime;

        if(tac.size() == 5)
        {
            try {
                mapChaveTac[chave] = tac;
            } catch (...) {
                qDebug() << "Erro ao carregar o mapChaveTac";
                return false;
            }
        }

    }

    return true;
}

bool RecuperarUF::carregarMapChaveUF(ifstream &arq, std::map<QString, QString> &mapChaveUF)
{
    std::string str;
    QString deviceId;//[0]
    QString dateTime;//[16]
    QString location;//[3]
    QString chave;
    QStringList strCsv;

    while(std::getline(arq, str))
    {
        strCsv = QString::fromStdString(str).split((";"));

        deviceId = strCsv[0];
        dateTime = strCsv[16];
        location = strCsv[3];
        chave = deviceId % dateTime;

        if(!location.isEmpty())
        {
            try {
                mapChaveUF[chave] = location;
             } catch (...) {
                qDebug() << "Erro ao carregar o mapChaveUF";
                return false;
            }
        }

    }

    return true;
}

bool RecuperarUF::recuperar4GAxiros(const QDir &dirOrigem, std::map<QString, QString> &mapChaveTac)
{
    QStringList arqs = dirOrigem.entryList(QStringList("*.csv"));
    std::string str;
    QStringList strCsv;
    QString ecgi;/*55*/
    QString tac;
    QString uf;/*0*/
    QString wanMode;/*22*/
    QString deviceId;//3
    QString dateTime;//17
    QString chave;


    ifstream arq;
    std::vector<QString> vecSaida;

    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        arq.open(dirOrigem.absoluteFilePath(*itQStr).toStdString());

        if(arq.is_open())
        {
            while(std::getline(arq, str))
            {
                strCsv = QString::fromStdString(str).split(";");

                wanMode = strCsv[22];
                uf = strCsv[0];

                if(wanMode == "4G")
                {
                    ++mTotalMed4G;
                    if(uf.isEmpty())
                    {
                        ++mTotalMedUfBranco4G;
                        if(!strCsv[3].isEmpty())
                        {
                            deviceId = strCsv[3].split("-")[1];
                            dateTime = strCsv[17];
                            chave = deviceId % dateTime;
                            ecgi = strCsv[55];

                            auto itTac = mapChaveTac.find(chave);
                            if(itTac != mapChaveTac.end())
                            {
                                tac = itTac->second;
                                mapChaveTac.erase(itTac);

                                opmm::IdentificacaoDaUf identUf;
                                identUf.setup(ecgi.toStdString(), tac.toStdString(), opmm::TAC);
                                uf = QString::fromStdString(identUf.uf());

                                if(uf == "$PI$")
                                {
                                    ++mTotalMedSemRecupUf4G;
                                    uf = "";
                                }

                                strCsv[0] = uf;


                            }
                            else
                            {
                                ++mTotalMedSemRecupUf4G;
                            }

                        }
                        else
                        {
                            ++mTotalMedSemRecupUf4G;
                        }
                    }
                }


                try {
                    vecSaida.push_back(strCsv.join(";"));
                } catch (...) {
                    descarregarMedicoes(vecSaida, *itQStr);
                    vecSaida.push_back(strCsv.join(";"));
                }

            }
            arq.close();

            if(!vecSaida.empty())
            {
                descarregarMedicoes(vecSaida, *itQStr);
            }
        }
        else
        {
            qDebug() << "Erro ao abrir: " << *itQStr;
            return false;
        }
    }


    return true;

}

bool RecuperarUF::recuperar4GNetmetric(const QDir &dirOrigem, std::map<QString, QString> &mapChaveUF)
{
    QStringList arqs = dirOrigem.entryList(QStringList("*.csv"));
    std::string str;
    QStringList strCsv;
    QString ecgi;/*55*/
    QString uf;/*0*/
    QString wanMode;/*22*/
    QString deviceId;//3
    QString dateTime;//17
    QString chave;


    ifstream arq;
    std::vector<QString> vecSaida;

    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        arq.open(dirOrigem.absoluteFilePath(*itQStr).toStdString());

        if(arq.is_open())
        {
            while(std::getline(arq, str))
            {
                strCsv = QString::fromStdString(str).split(";");

                wanMode = strCsv[22];
                uf = strCsv[0];

                if(wanMode == "4G")
                {
                    ++mTotalMed4G;
                    if(uf.isEmpty())
                    {
                        ++mTotalMedUfBranco4G;
                        if(!strCsv[3].isEmpty())
                        {
//                            deviceId = strCsv[3].split("-")[1];
                            deviceId = strCsv[3];   //Enquanto a alteração do codigo da prestadora não estiver ativo
                            dateTime = strCsv[17];
                            chave = deviceId % dateTime;
                            ecgi = strCsv[55];

                            auto itUf = mapChaveUF.find(chave);
                            if(itUf != mapChaveUF.end())
                            {
                                uf = itUf->second;
                                mapChaveUF.erase(itUf);
                                strCsv[0] = uf;
                            }
                            else
                            {
                                ++mTotalMedSemRecupUf4G;
                            }

                        }
                        else
                        {
                            ++mTotalMedSemRecupUf4G;
                        }
                    }
                }


                try {
                    vecSaida.push_back(strCsv.join(";"));
                } catch (...) {
                    descarregarMedicoes(vecSaida, *itQStr);
                    vecSaida.push_back(strCsv.join(";"));
                }

            }
            arq.close();

            if(!vecSaida.empty())
            {
                descarregarMedicoes(vecSaida, *itQStr);
            }
        }
        else
        {
            qDebug() << "Erro ao abrir: " << *itQStr;
            return false;
        }
    }

    return true;
}

QString RecuperarUF::totalMedRecup3G() const
{
    return QString("%1%").arg((float(mTotalMed3G - mTotalMedSemRecupUf3G)/float(mTotalMed3G))*100.0);
}

QString RecuperarUF::totalMedRecupInicial3G() const
{
    return QString("%1%").arg((float(mTotalMed3G - mTotalMedUfBranco3G)/float(mTotalMed3G))*100.0);
}

QString RecuperarUF::totalMedRecup4G() const
{
    return QString("%1%").arg((float(mTotalMed4G - mTotalMedSemRecupUf4G)/float(mTotalMed4G))*100.0);
}

QString RecuperarUF::totalMedRecupInicial4G() const
{
    return QString("%1%").arg((float(mTotalMed4G - mTotalMedUfBranco4G)/float(mTotalMed4G))*100.0);
}

