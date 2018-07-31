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

    void on_tableWidget_desativadas_cellDoubleClicked(int row, int column);

    void on_tableWidget_ativadas_cellDoubleClicked(int row, int column);

private:
    void carregarMapCodigoDescarteDoArquivo();
    void atualizarTabela(QTableWidget& tabela, const mapQStr& mapAtivDes);
    void carregarMapsCodigoDescarte(const QTableWidget& tabela, mapQStr& mapAtivDes);
    bool salvarRegras(const mapQStr& mapAtivo, const mapQStr& mapInativo);

private:
    Ui::RegrasDescarte *ui;

    QTableWidget* mTabelaRegAtivas;
    QTableWidget* mTabelaRegInativas;

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
        {"14","Paridade Download e Upload"},
        {"15","Quarentena"},
        {"16","Ping back divergente"},
        {"17","Stolen/changed probe"},
        {"19","Exceção de Infraestrutura"},
        {"20","3 medições/dia - 3 medições válidas"},
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
        {"35","Modelo do hardware/software não compatível"},
        {"36","Intensidade do sinal"},
        {"40","Medição sem coletor"}
    };


};

#endif // REGRASDESCARTE_H
