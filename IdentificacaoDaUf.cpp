#include "IdentificacaoDaUf.h"

namespace opmm {

IdentificacaoDaUf::IdentificacaoDaUf()
{

}

IdentificacaoDaUf::~IdentificacaoDaUf()
{
    mMapCnUf.clear();
    mMapMncRede.clear();
}

IdentificacaoDaUf::IdentificacaoDaUf(const string &cgi, const string &medicaoLacTac, const tacLac &tipoMedicao, const pesquisa &tipoPesquisa, const unordered_map<string, string> &mapCgiUf)
{
    setup(cgi, medicaoLacTac, tipoMedicao, tipoPesquisa, mapCgiUf);
}

string IdentificacaoDaUf::cgi() const
{
    return mCgi;
}

void IdentificacaoDaUf::setCgi(const string &cgi)
{
    mCgi = cgi;
}

string IdentificacaoDaUf::medicaoLacTac() const
{
    return mMedicaoLacTac;
}

void IdentificacaoDaUf::setMedicaoLacTac(const string &medicaoLacTac)
{
    mMedicaoLacTac = medicaoLacTac;
}

tacLac IdentificacaoDaUf::tipoMedicao() const
{
    return mTipoMedicao;
}

void IdentificacaoDaUf::setTipoMedicao(const tacLac &tipoMedicao)
{
    mTipoMedicao = tipoMedicao;
}

void IdentificacaoDaUf::destrincharCgi()
{
    setMcc(string(cgi(),0,2));
    setMnc(string(cgi(),3,2));
}


void IdentificacaoDaUf::setup(const string &cgi, const string &medicaoLacTac,
                              const tacLac &tipoMedicao, const pesquisa &tipoPesquisa,
                              const unordered_map<string, string> &mapCgiUf)
{

    mCgi = cgi;
    mMedicaoLacTac = medicaoLacTac;
    mTipoMedicao = tipoMedicao;
    mTipoPesquisa = tipoPesquisa;

    mUf = "$PI$";

    if(tipoPesquisa == TAC_LAC)
    {
        if(medicaoLacTac.size() != 5)
        {
            setMedicaoValida(false);
            return;
        }

        //Validando o CGI
        if(tipoMedicao == LAC)
        {
            if(mCgi.size() > 15)
            {
                setMedicaoValida(false);
                return;
            }
        }

        //Validando o E-CGI
        if(tipoMedicao == TAC)
        {
            if(mCgi.size() > 13)
            {
                setMedicaoValida(false);
                return;
            }
        }

        destrincharCgi();

        if(!setRede(mMnc))
        {
            setMedicaoValida(false);
            return;
        }

        if(!setCn(mMedicaoLacTac, mapCgiUf))
        {
            setMedicaoValida(false);
            return;
        }

        if(!setUf(mCn))
        {
            setMedicaoValida(false);
            return;
        }
    }



    setMedicaoValida(true);

}

string IdentificacaoDaUf::uf(const unordered_map<string, string> &mapCgiUf)
{

    if(mTipoPesquisa == TAC_LAC)
    {
        return mUf;
    }
    else
    {
        itFind = mapCgiUf.find(mCgi);
        if(itFind != mapCgiUf.end())
        {
            return itFind->second;
        }
        else
        {
            return "$PI$";
        }

    }


}

bool IdentificacaoDaUf::setUf(const string &cn)
{
    if(mMapCnUf.find(cn) != mMapCnUf.end())
    {
        mUf = mMapCnUf[cn];
        return true;
    }
    else
    {
        mUf = "$PI$";
        return false;
    }

}

string IdentificacaoDaUf::cn()
{
    return mCn;
}

bool IdentificacaoDaUf::setCn(const string &medicaoLacTac, const unordered_map<string, string> &mapCgiUf)
{

    if(tipoMedicao() == LAC)
    {
        switch (rede()) {
        case ALGAR:
        case CLARO:
        case VIVO:
        case PORTO:
        case TIM:
        case SERCOMTEL:
            mCn = string(medicaoLacTac,3,2);   //xxxcn
            break;
        case NEXTEL:
            mCn = string(medicaoLacTac,1,2);   //xcnxx
            break;
        case NONE:
            mCn = "-1";
            return false;
            break;
        case OI:
            itFind = mapCgiUf.find(mCgi);
            if(itFind != mapCgiUf.end())
            {
                mCn = itFind->second;
            }
            else
            {
                mCn = "-1";
                return false;
            }

            break;
        default:
            mCn = "-1";
            cerr << "LAC - OPERADORA NAO IMPLEMENTADA! CGI: " << mCgi << endl;
            return false;
            break;
        }
    }
    else    //TAC
    {
        switch (rede()) {
        case CLARO:
        case ALGAR:
        case VIVO:                              //Tenho que verificar isso!!
            mCn = string(medicaoLacTac,3,2);   //xxxcn
            break;
        case OI:
        case NEXTEL:
        case PORTO:
        case TIM:
            mCn = string(medicaoLacTac,1,2);   //xcnxx
            break;
        case SERCOMTEL: //NAO TEM 4G
            mCn = "-1";
            return false;
            break;
        case NONE:
            mCn = "-1";
            return false;
            break;
//        case VIVO:
//            //Preciso pegar da coluna "location"
//            break;
        default:
            mCn = "-1";
            cerr << "TAC - OPERADORA NAO IMPLEMENTADA! e-CGI: " << mCgi <<endl;
            return false;
            break;
        }

    }

    return true;
}



bool IdentificacaoDaUf::setRede(const string &mnc)
{

    if(mMapMncRede.find(mnc) != mMapMncRede.end())
    {
        mRede = mMapMncRede[mnc];
        return true;
    }
    else
    {
        mRede = NONE;
        return false;
    }

}

string IdentificacaoDaUf::mnc() const
{
    return mMnc;
}

void IdentificacaoDaUf::setMnc(const string &mnc)
{
    mMnc = mnc;
}

string IdentificacaoDaUf::mcc() const
{
    return mMcc;
}

void IdentificacaoDaUf::setMcc(const string &mcc)
{
    mMcc = mcc;
}


}


