#ifndef IDENTIFICACAODAUF_H
#define IDENTIFICACAODAUF_H

#include "validade.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <QDir>
#include <QStringBuilder>
#include <QStringList>
#include <fstream>


using namespace std;

namespace opmm {

enum tacLac {LAC, TAC};
enum pesquisa{BASE_CGI, TAC_LAC};

class IdentificacaoDaUf : public Validade
{
public:
    IdentificacaoDaUf();
    virtual ~IdentificacaoDaUf();
    IdentificacaoDaUf(const string &cgi, const string &medicaoLacTac, const tacLac &tipoMedicao,
                      const pesquisa &tipoPesquisa = TAC_LAC,
                      const unordered_map<string, string> &mapCgiUf = unordered_map<string, string>());

    string uf(const unordered_map<string, string> &mapCgiUf = unordered_map<string, string>());

    void setup(const string &cgi, const string &medicaoLacTac,
               const tacLac &tipoMedicao, const pesquisa &tipoPesquisa = TAC_LAC,
               const unordered_map<string, string> &mapCgiUf = unordered_map<string, string>());



private:

    enum prestadora {ALGAR, CLARO, NEXTEL, OI, PORTO, SERCOMTEL, TIM, VIVO, NONE};

    string mcc() const;
    void setMcc(const string &mcc);

    string mnc() const;
    void setMnc(const string &mnc);

    string cgi() const;
    void setCgi(const string &cgi);

    string medicaoLacTac() const;
    void setMedicaoLacTac(const string &medicaoLacTac);

    tacLac tipoMedicao() const;
    void setTipoMedicao(const tacLac &tipoMedicao);

    prestadora rede() const{return mRede;}
    bool setRede(const string &mnc);

    string cn();
    bool setCn(const string &medicaoLacTac,
               const unordered_map<string, string> &mapCgiUf = unordered_map<string, string>());

    bool setUf(const string &cn);


    void destrincharCgi();

    //bool carregarBaseCgi();




private:
    string mMcc;    //codigo Pais (Brasil 724)
    string mMnc;    //codigo prestadora
    string mCgi;
    string mMedicaoLacTac;
    tacLac mTipoMedicao;
    layout mTipoLayout;

    string mCn;
    string mUf = "$PI$";

    prestadora mRede;

    unordered_map<string, string>::const_iterator itFind;
//    unordered_map<string, string> mMapCgiUf;

    pesquisa mTipoPesquisa;

//    bool mFlagBaseCgiCarregada = false;



    typedef unordered_map<string, prestadora> mapStrPrestadora;
    mapStrPrestadora mMapMncRede = {
        {"24", OI},
        {"16", OI},
        {"30", OI},
        {"31", OI},
        {"12", CLARO},
        {"38", CLARO},
        {"05", CLARO},
        {"32", ALGAR},
        {"34", ALGAR},
        {"33", ALGAR},
        {"07", ALGAR},
        {"39", NEXTEL},
        {"00", NEXTEL},
        {"01", VIVO},
        {"19", VIVO},
        {"23", VIVO},
        {"11", VIVO},
        {"10", VIVO},
        {"06", VIVO},
        {"54", PORTO},
        {"08", TIM},
        {"03", TIM},
        {"02", TIM},
        {"04", TIM},
        {"15", SERCOMTEL},
    };

    unordered_map<string, string> mMapCnUf = {
        {"11", "SP"},
        {"12", "SP"},
        {"13", "SP"},
        {"14", "SP"},
        {"15", "SP"},
        {"16", "SP"},
        {"17", "SP"},
        {"18", "SP"},
        {"19", "SP"},
        {"21", "RJ"},
        {"22", "RJ"},
        {"24", "RJ"},
        {"27", "ES"},
        {"28", "ES"},
        {"31", "MG"},
        {"32", "MG"},
        {"33", "MG"},
        {"34", "MG"},
        {"35", "MG"},
        {"37", "MG"},
        {"38", "MG"},
        {"41", "PR"},
        {"42", "PR"},
        {"43", "PR"},
        {"44", "PR"},
        {"45", "PR"},
        {"46", "PR"},
        {"47", "SC"},
        {"48", "SC"},
        {"49", "SC"},
        {"51", "RS"},
        {"53", "RS"},
        {"54", "RS"},
        {"55", "RS"},
        {"61", "DF"},
        {"62", "GO"},
        {"63", "TO"},
        {"64", "GO"},
        {"65", "MT"},
        {"66", "MT"},
        {"67", "MS"},
        {"68", "AC"},
        {"69", "RO"},
        {"71", "BA"},
        {"73", "BA"},
        {"74", "BA"},
        {"75", "BA"},
        {"77", "BA"},
        {"79", "SE"},
        {"81", "PE"},
        {"82", "AL"},
        {"83", "PB"},
        {"84", "RN"},
        {"85", "CE"},
        {"86", "PI"},
        {"87", "PE"},
        {"88", "CE"},
        {"89", "PI"},
        {"91", "PA"},
        {"92", "AM"},
        {"93", "PA"},
        {"94", "PA"},
        {"95", "RR"},
        {"96", "AP"},
        {"97", "AM"},
        {"98", "MA"},
        {"99", "MA"}
    };



};

}


#endif // IDENTIFICACAODAUF_H
