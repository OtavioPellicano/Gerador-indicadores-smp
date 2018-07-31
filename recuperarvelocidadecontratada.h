#ifndef RECUPERARVELOCIDADECONTRATADA_H
#define RECUPERARVELOCIDADECONTRATADA_H

#include <QDir>
#include <QStringList>
#include <fstream>
#include <map>
#include <QDebug>
#include "Medicao.h"
#include <vector>

/**
 * @brief The RecuperarVelocidadeContratada class
 * Recupera a velocidade (3G e 4G)
 */
class RecuperarVelocidadeContratada
{
public:
    RecuperarVelocidadeContratada(const QDir& dirIn);

    bool processar();
private:

    std::map<QString, std::pair<QString, QString>> velocidadeReferencia(const QString& fileName);

private:
    QDir mDirIn;




};

#endif // RECUPERARVELOCIDADECONTRATADA_H
