#include "validade.h"

namespace opmm {


Validade::Validade()
{

}

bool Validade::medicaoValida() const
{
    return mMedicaoValida;
}

void Validade::setMedicaoValida(const bool &medicaoValida)
{
    mMedicaoValida = medicaoValida;
}


}
