#include "recuperarvelocidadecontratada.h"

RecuperarVelocidadeContratada::RecuperarVelocidadeContratada(const QDir &dirIn)
    :mDirIn(dirIn)
{

}

bool RecuperarVelocidadeContratada::processar()
{
    QStringList arqs = mDirIn.entryList(QStringList("*.csv"));

    QString wanMode, downStream, upStream;
    QStringList strCsv;
    std::ifstream arqIn;
    std::ofstream arqOut;
    std::string str;
    std::vector<QString> vecMed;

    //Tecnologia velocidade de referência down e up
    std::map<QString, std::pair<QString, QString>> mapTecnologiaVelocidadeRef;

    for(auto itQStr = arqs.begin(); itQStr != arqs.end(); ++itQStr)
    {
        mapTecnologiaVelocidadeRef = velocidadeReferencia(*itQStr);

        vecMed.clear(); //limpando o vetor

        arqIn.open(mDirIn.absoluteFilePath(*itQStr).toStdString());
        if(arqIn.is_open())
        {
            while(std::getline(arqIn, str))
            {
                strCsv = QString::fromStdString(str).split(";");
                wanMode = strCsv[22];
                downStream = strCsv[1];
                upStream = strCsv[2];

                if(wanMode == "3G" || wanMode == "4G")
                {
                    if(downStream == "")
                    {
                        strCsv[1] = mapTecnologiaVelocidadeRef[wanMode].first;
                    }
                    if(upStream == "")
                    {
                        strCsv[2] = mapTecnologiaVelocidadeRef[wanMode].second;
                    }
                }

                vecMed.push_back(strCsv.join(";"));

            }
            arqIn.close();
        }
        else
        {
            return false;
        }

        arqOut.open(mDirIn.absoluteFilePath(*itQStr).toStdString());

        if(arqOut.is_open())
        {
            for(auto itVec = vecMed.begin(); itVec != vecMed.end(); ++itVec)
            {
                arqOut << (*itVec).toStdString() << std::endl;
            }
            arqOut.close();
        }
        else
        {
            return false;
        }
    }

    return true;
}

std::map<QString, std::pair<QString, QString>> RecuperarVelocidadeContratada::velocidadeReferencia(const QString &fileName)
{

    QString wanMode, downStream, upStream;
    QStringList strCsv;
    std::ifstream arqIn;
    std::string str;

    //Tecnologia velocidade de referência down e up
    std::map<QString, std::pair<QString, QString>> mapTecnologiaVelocidadeRef;

    arqIn.open(mDirIn.absoluteFilePath(fileName).toStdString());
    if(arqIn.is_open())
    {
        while(std::getline(arqIn, str))
        {
            strCsv = QString::fromStdString(str).split(";");
            wanMode = strCsv[22];
            downStream = strCsv[1];
            upStream = strCsv[2];

            if(wanMode == "3G" || wanMode == "4G")
            {

                if(!downStream.isEmpty() && !upStream.isEmpty())
                {
                    mapTecnologiaVelocidadeRef[wanMode] = std::pair<QString, QString>(downStream, upStream);

                    if(mapTecnologiaVelocidadeRef.size() == 2)
                        break;

                }
            }
        }
        arqIn.close();
    }
    else
    {
        qDebug() << "Erro ao coletar a referencia de down e up: " << fileName;
    }

    return mapTecnologiaVelocidadeRef;

}
