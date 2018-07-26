#include "regrasdescarte.h"
#include "ui_regrasdescarte.h"

RegrasDescarte::RegrasDescarte(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegrasDescarte)
{
    ui->setupUi(this);

    QStringList header;
    header << "Código" << "Descrição";
    ui->tableWidget_ativadas->setColumnCount(2);
    ui->tableWidget_desativadas->setColumnCount(2);
    ui->tableWidget_ativadas->setHorizontalHeaderLabels(header);
    ui->tableWidget_desativadas->setHorizontalHeaderLabels(header);
    ui->tableWidget_ativadas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_ativadas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_desativadas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_desativadas->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget_ativadas->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_desativadas->horizontalHeader()->setStretchLastSection(true);

    carregarMapCodigoDescarteDoArquivo();
    if(mMapCodigoDescricaoAtivo.empty() && mMapCodigoDescricaoInativo.empty())
    {
        atualizarTabela(*ui->tableWidget_ativadas, mMapCodigoDescricao);
    }
    else
    {
        atualizarTabela(*ui->tableWidget_ativadas, mMapCodigoDescricaoAtivo);
        atualizarTabela(*ui->tableWidget_desativadas, mMapCodigoDescricaoInativo);
    }

}

RegrasDescarte::~RegrasDescarte()
{
    delete ui;
}

void RegrasDescarte::carregarMapCodigoDescarteDoArquivo()
{

    mMapCodigoDescricaoAtivo.clear();
    mMapCodigoDescricaoInativo.clear();

    QDir dir;
    std::ifstream arq((dir.absolutePath() + "/" + mNomeDirRef + "/" + mFileNamePA).toStdString());
    std::string str;
    QStringList strCsv;

    if(arq.is_open())
    {
        for(size_t i = 0; std::getline(arq,str); ++i)
        {
            if(i == 0)
                continue;
            strCsv = QString::fromStdString(str).split(";");

            if(strCsv[2] == "1")
            {
                mMapCodigoDescricaoAtivo[strCsv[0]] = strCsv[1];
            }
            else if(strCsv[2] == "0")
            {
                mMapCodigoDescricaoInativo[strCsv[0]] = strCsv[1];
            }
        }
    }
    else
    {
        qDebug() << "nao existe arquivo de codigo de descarte";
    }
}

void RegrasDescarte::atualizarTabela(QTableWidget &tabela, const mapQStr &mapAtivDes)
{
    tabela.clearContents();
    size_t linha = 0;

    for(auto itMap = mapAtivDes.begin(); itMap != mapAtivDes.end(); ++itMap)
    {
        tabela.setRowCount(linha + 1);
        tabela.setItem(linha, 0, new QTableWidgetItem(itMap->first));
        tabela.setItem(linha, 1, new QTableWidgetItem(itMap->second));
        ++linha;
    }

}

void RegrasDescarte::carregarMapsCodigoDescarte(const QTableWidget &tabela, mapQStr &mapAtivDes)
{
    mapAtivDes.clear();
    for(int i = 0; i < tabela.rowCount(); ++i)
    {
        mapAtivDes[tabela.item(i,0)->text()] = tabela.item(i,1)->text();
    }
}

bool RegrasDescarte::salvarRegras(const mapQStr &mapAtivo, const mapQStr &mapInativo)
{
    QDir dir;
    dir.mkdir(mNomeDirRef);
    dir.cd(mNomeDirRef);

    std::ofstream arq((dir.absolutePath() + "/" + mFileNamePA).toStdString());

    if(arq.is_open())
    {
        arq << "Codigo;Decricao;ativo" << std::endl;


        for(auto itMap = mapAtivo.begin(); itMap != mapAtivo.end(); ++itMap)
        {
            arq << itMap->first.toStdString() << ";" << itMap->second.toStdString() << ";1" << std::endl;
        }

        for(auto itMap = mapInativo.begin(); itMap != mapInativo.end(); ++itMap)
        {
            arq << itMap->first.toStdString() << ";" << itMap->second.toStdString() << ";0" << std::endl;
        }
    }
    else
    {
        qDebug() << "Impossivel salvar Regras!";
        return false;
    }

    return true;



}

mapQStr RegrasDescarte::mapCodigoDescricaoInativo() const
{
    return mMapCodigoDescricaoInativo;
}

mapQStr RegrasDescarte::mapCodigoDescricaoAtivo() const
{
    return mMapCodigoDescricaoAtivo;
}

void RegrasDescarte::on_pushButton_aplicar_clicked()
{
    carregarMapsCodigoDescarte(*ui->tableWidget_ativadas, mMapCodigoDescricaoAtivo);
    carregarMapsCodigoDescarte(*ui->tableWidget_desativadas, mMapCodigoDescricaoInativo);

    if(salvarRegras(mMapCodigoDescricaoAtivo, mMapCodigoDescricaoInativo))
    {
        QMessageBox::information(this, tr("Regras de Descarte"), tr("Regras de Descarte aplicadas!"), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, tr("Regras de Descarte"), tr("Erro ao aplicar as Regras de Descarte!\nRegras não aplicadas!"), QMessageBox::Ok);
    }
}
