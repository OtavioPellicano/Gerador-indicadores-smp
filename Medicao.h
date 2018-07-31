#ifndef MEDICAO_H
#define MEDICAO_H

#include <QStringList>
#include <QStringBuilder>
#include <string>

enum tipoPrestadora{SMP, SCM};

class Medicao
{
public:
    Medicao(const QString &medicao, const tipoPrestadora &tipo, const QString &sep);

    QString medicaoValida() const;

    QString medicao() const;

    QString uf() const;

    QString wanMode() const;

    QString prestadora() const;

    QString descarte() const;

    QString downStream() const;

    QString upStream() const;

private:
    QString prestadoraFromDeviceId(const QString &deviceId);

    void dataHoraSplit(const QString& dataHora, QString& data, QString& hora);

private:
    QString mSep;

    QString mDataHora;
    QString mDeviceId;
    QString mWanMode;
    QString mSpeedDown;
    QString mSpeedUp;
    QString mDownStream;
    QString mUpStream;
    QString mUf;
    QString mDescarte;
    QString mPrestadora;
    QString mData;
    QString mHora;

    tipoPrestadora mTipo;

};

#endif // MEDICAO_H
