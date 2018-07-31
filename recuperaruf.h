#ifndef RECUPERARUF_H
#define RECUPERARUF_H

#include <QStringList>
#include <QDir>
#include "IdentificacaoDaUf.h"

class RecuperarUF
{
public:
    RecuperarUF(const QDir& dirIn);

private:
    QDir mDirIn;
};

#endif // RECUPERARUF_H
