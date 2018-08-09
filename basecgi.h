#ifndef BASECGI_H
#define BASECGI_H

#include <QDir>
#include <QFileDialog>
#include <string>
#include <unordered_map>
#include <QStringList>
#include <fstream>
#include <QDebug>

typedef std::unordered_map<std::string, std::string> uMapStr;

class BaseCGI
{
public:
    BaseCGI();

    bool fileDialogBaseCGI(const QString &fileNameCgi);
    bool carregarBaseCgi(const QString &fullPathCGI);
    bool salvarBaseCgi();

    void carregarMapCgiUf();
    uMapStr &unMapCgiUf();

private:

    bool validarBaseCgi(const QString& fullPathCGI);

private:
    uMapStr mUnMapCgiUf;

    const QString mFileNamePA = "cgi_smp.csv";
    const QString mNomeDirRef = "data_gismp";

};

#endif // BASECGI_H
