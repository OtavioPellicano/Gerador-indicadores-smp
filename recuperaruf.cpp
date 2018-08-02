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

QString RecuperarUF::totalMedRecup3G() const
{
    return QString("%1%").arg((float(mTotalMed3G - mTotalMedSemRecupUf3G)/float(mTotalMed3G))*100.0);
}

QString RecuperarUF::totalMedRecupInicial3G() const
{
    return QString("%1%").arg((float(mTotalMed3G - mTotalMedUfBranco3G)/float(mTotalMed3G))*100.0);
}

