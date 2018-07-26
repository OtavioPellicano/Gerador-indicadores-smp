#include "Medicao.h"


Medicao::Medicao(const QString &medicao, const tipoPrestadora &tipo, const QString &sep)
    :mSep(sep)
{
    QStringList strCsv = medicao.split(this->mSep);

    if(tipo == SMP)
    {
        this->mDataHora = strCsv[17];
        this->mDeviceId = strCsv[3];
        this->mSpeedDown = strCsv[40];
        this->mSpeedUp = strCsv[39];
        this->mDownStream = strCsv[1];
        this->mUpStream = strCsv[2];

        this->mUf = strCsv[0];
        this->mDescarte = strCsv[57];
        this->mWanMode = strCsv[22];

        this->mPrestadora = prestadoraFromDeviceId(strCsv[3]);

    }
    else
    {
        this->mDataHora = strCsv[11];
        this->mDeviceId = strCsv[0];
        this->mSpeedDown = strCsv[25];
        this->mSpeedUp = strCsv[24];
        this->mDownStream = "ERRO";
        this->mUpStream = "ERRO";

        this->mUf = strCsv[27];
        this->mDescarte = strCsv[26];
        this->mWanMode = "WIFI";

        this->mPrestadora = prestadoraFromDeviceId(strCsv[0]);

    }

}

QString Medicao::medicaoValida() const
{
    if(this->mDescarte.isEmpty())
    {
        return this->mDataHora % this->mSep % this->mDeviceId % this->mSep % this->mSpeedDown % this->mSep %
                this->mSpeedUp % this->mSep % this->mDownStream % this->mSep % this->mUpStream % this->mSep % this->mPrestadora;
    }
    else
    {
        return "";
    }

}

QString Medicao::medicao() const
{
    return this->mDataHora % this->mSep % this->mDeviceId % this->mSep % this->mSpeedDown % this->mSep %
            this->mSpeedUp % this->mSep % this->mDownStream % this->mSep % this->mUpStream % this->mSep % this->mPrestadora;

}

QString Medicao::uf() const
{
    return this->mUf;
}

QString Medicao::wanMode() const
{
    return this->mWanMode;
}

QString Medicao::prestadoraFromDeviceId(const QString &deviceId)
{
    char codigo = deviceId.split('-')[0].toInt();
    QString prest;

    //SKY e VIVO nao tem codigo
    switch (codigo) {
    case 4:
        prest = "ALGAR";
        break;
    case 14:
        prest = "CLARO";
        break;
    case 16:
        prest = "NEXTEL";
        break;
    case 11:
        prest = "OI";
        break;
    case 103:
        prest = "PORTO_CONECTA";
        break;
    case 8:
        prest = "SERCOMTEL";
        break;
    case 13:
        prest = "TIM";
        break;
    case 3:
        prest = "CABO";
        break;
    default:
        prest = "VIVO";
        break;
    }

    return prest;
}

QString Medicao::descarte() const
{
    return mDescarte;
}

//void Medicao::dataHoraSplit(const QString &dataHora, QString &data, QString &hora)
//{
//    std::string
//}

QString Medicao::prestadora() const
{
    return mPrestadora;
}
