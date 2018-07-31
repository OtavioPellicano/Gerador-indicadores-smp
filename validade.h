#ifndef VALIDADE_H
#define VALIDADE_H

namespace opmm {

enum layout {ABR = 1, TGR, NETMETRICS = 3, SEMIGLOBE, HDM};

class Validade
{
public:
    Validade();
    virtual ~Validade(){}
    virtual bool medicaoValida() const;

protected:
    virtual void setMedicaoValida(const bool &medicaoValida);

private:
    bool mMedicaoValida;
};


}


#endif // VALIDADE_H
