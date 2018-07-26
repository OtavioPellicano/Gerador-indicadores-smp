#ifndef REGRASDESCARTE_H
#define REGRASDESCARTE_H

#include <QDialog>
#include <QStringList>
#include <map>
#include <QDir>
#include <fstream>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>

typedef std::map<QString, QString> mapQStr;

namespace Ui {
class RegrasDescarte;
}

class RegrasDescarte : public QDialog
{
    Q_OBJECT

public:
    explicit RegrasDescarte(QWidget *parent = 0);
    ~RegrasDescarte();

    mapQStr mapCodigoDescricaoAtivo() const;

    mapQStr mapCodigoDescricaoInativo() const;

private slots:
    void on_pushButton_aplicar_clicked();

private:
    void carregarMapCodigoDescarteDoArquivo();
    void atualizarTabela(QTableWidget& tabela, const mapQStr& mapAtivDes);
    void carregarMapsCodigoDescarte(const QTableWidget& tabela, mapQStr& mapAtivDes);
    bool salvarRegras(const mapQStr& mapAtivo, const mapQStr& mapInativo);

private:
    Ui::RegrasDescarte *ui;

    const QString mFileNamePA = "rd_smp.csv";
    const QString mNomeDirRef = "data_gismp";

    mapQStr mMapCodigoDescricaoAtivo;
    mapQStr mMapCodigoDescricaoInativo;

    mapQStr mMapCodigoDescricao = {
        {"00","Validação dos campos esperados"},
        {"01","Duplicidades"},
        {"02","Medição com falha - Down ou Up"},
        {"03","Medição com falha - Latência"},
        {"04","Medição com falha - Disponibilidade"},
        {"05","Medição com falha - Jitter"},
        {"06","PMT"},
        {"07","Alteração de endereço"},
        {"08","Alteração de plano"},
        {"09","Limitação por franquia"},
        {"10","Suspensão de Serviço"},
        {"11","Cancelamento de Plano"},
        {"12","ISP com menos de 180 dias de operação"},
        {"14","Paridade Download e Upload"},
        {"15","Quarentena"},
        {"16","Ping back divergente"},
        {"17","Stolen/changed probe"},
        {"18","Medições de voluntário/assinante não validado ou não encontrado"},
        {"19","Exceção de Infraestrutura"},
        {"20","3 medições/dia - 3 medições válidas"},
        {"21","Voluntário não válido"},
        {"22","Envio de Rawdata fora do prazo"},
        {"23","Validação dos campos esperados"},
        {"24","PTT não cadastrado"},
        {"25","Avaliação da CPU"},
        {"26","Memória Insuficiente"},
        {"27","Medições autônomas"},
        {"28","Avaliação do status"},
        {"29","Tipo de Conexão"},
        {"30","Troca de Tecnologia"},
        {"31","CellId não encontrado"},
        {"32","Alteração de plano"},
        {"33","UF sem plano amostral"},
        {"34","Velocidade contrata acima de 100 Mbps"},
        {"35","Modelo do hardware/software não compatível"},
        {"36","Intensidade do sinal"},
        {"37","CEP Error"},
        {"38","Município sem Mesorregião usado no PA"},
        {"39","Invalid CEP/UF/GMT for ISP that send the data in raw"},
        {"40","Medição sem coletor"}
    };


};

#endif // REGRASDESCARTE_H
