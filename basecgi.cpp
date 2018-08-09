#include "basecgi.h"

BaseCGI::BaseCGI()
{

}

bool BaseCGI::fileDialogBaseCGI(const QString& fileNameCgi)
{
    if(!validarBaseCgi(fileNameCgi))
    {
        return false;
    }

    return true;

}

bool BaseCGI::carregarBaseCgi(const QString& fullPathCGI)
{
    std::ifstream arq(fullPathCGI.toStdString());
    std::string str;
    QStringList strCsv;
    //cgi[3]
    //uf[10]

    if(arq.is_open())
    {
        mUnMapCgiUf.clear();
        for(size_t i = 0; std::getline(arq, str); ++i)
        {
            if(i == 0)
                continue;

            strCsv = QString::fromStdString(str).split(";");

            if(strCsv.size() < 11)
                continue;

            if(strCsv[3].isEmpty() || strCsv[10].isEmpty())
                continue;

            try {
                mUnMapCgiUf[strCsv[3].toStdString()] = strCsv[10].toStdString();
            } catch (...) {
                mUnMapCgiUf.clear();
                return false;
            }
        }

        arq.close();
    }
    else
    {
        qDebug() << "Erro ao carregar Base CGI: " << fullPathCGI;
        return false;
    }

    return true;

}

bool BaseCGI::validarBaseCgi(const QString &fullPathCGI)
{
    std::ifstream arq(fullPathCGI.toStdString());
    std::string str;
    QStringList strCsv;

    if(arq.is_open())
    {
        std::getline(arq, str);
        arq.close();
        strCsv = QString::fromStdString(str).split(";");
        if(strCsv.size() != 3)
        {
            return false;
        }

        if(strCsv[0] != "H" || strCsv[1] != "B" || !strCsv[2].isEmpty())
        {
            return false;
        }

    }
    else
    {
        qDebug() << "Erro ao ler base CGI: " << fullPathCGI;
    }

    return true;
}

bool BaseCGI::salvarBaseCgi()
{
    QDir dir;
    dir.mkdir(mNomeDirRef);
    dir.cd(mNomeDirRef);

    std::ofstream arq((dir.absolutePath() + "/" + mFileNamePA).toStdString());

    if(arq.is_open())
    {
        arq << "cgi;UF" << std::endl;
        for(auto itMap = mUnMapCgiUf.begin(); itMap != mUnMapCgiUf.end(); ++itMap)
        {
            arq << itMap->first << ";" << itMap->second << std::endl;
        }
    }
    else
    {
        qDebug() << "Impossivel salvar CGI";
        return false;
    }

    return true;
}

void BaseCGI::carregarMapCgiUf()
{
    mUnMapCgiUf.clear();
    QDir dir;
    dir.mkdir(mNomeDirRef);
    dir.cd(mNomeDirRef);

    std::ifstream arq((dir.absolutePath() + "/" + mFileNamePA).toStdString());

    QStringList strCsv;
    std::string str;

    if(arq.is_open())
    {
        for(size_t i = 0; std::getline(arq, str); ++i)
        {
            if(i == 0)
                continue;

            strCsv = QString::fromStdString(str).split(";");

            mUnMapCgiUf[strCsv[0].toStdString()] = strCsv[1].toStdString();
        }
        arq.close();
    }
    else
    {
        qDebug() << "Impossivel abrir o arquivo CGI";
    }


}

uMapStr &BaseCGI::unMapCgiUf()
{
    return mUnMapCgiUf;
}
