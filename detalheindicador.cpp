#include "detalheindicador.h"
#include "ui_detalheindicador.h"

DetalheIndicador::DetalheIndicador(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetalheIndicador)
{
    ui->setupUi(this);
    QStringList header;
    header << "Prestadora" << "UF" << "Conexão" << "Medições Válidas" <<
              "Índice 39" << "Índice 40" << "Índice 41" << "Índice 42";
    ui->tableWidget_detalhe->setColumnCount(8);
    ui->tableWidget_detalhe->setHorizontalHeaderLabels(header);

}

DetalheIndicador::~DetalheIndicador()
{
    delete ui;
}

void DetalheIndicador::atualizarTabelaIndice(const QString& prestadora, const QString& uf, const vecQStr& vecIndicadores)
{
    QStringList strCsv;
    int colPrest = 0, colUF = 2, colWanmode = 3, colMedVal = 4, colColVal = 5,
            colInd_39 = 6, colInd_40 = 7, colInd_41 = 8, colInd_42 = 9;
    int linha = 0;

    ui->tableWidget_detalhe->clearContents();
    ui->tableWidget_detalhe->setRowCount(0);

    for(auto itQStr = vecIndicadores.begin() + 1; itQStr != vecIndicadores.end(); ++itQStr)
    {
        strCsv = (*itQStr).split(";");

        if(strCsv[colPrest] == prestadora)
            if(strCsv[colUF] == uf)
            {
                ui->tableWidget_detalhe->setRowCount(linha + 1);
                ui->tableWidget_detalhe->setItem(linha, 0, new QTableWidgetItem(strCsv[colPrest]));
                ui->tableWidget_detalhe->setItem(linha, 1, new QTableWidgetItem(strCsv[colUF]));
                ui->tableWidget_detalhe->setItem(linha, 2, new QTableWidgetItem(strCsv[colWanmode]));
                ui->tableWidget_detalhe->setItem(linha, 3, new QTableWidgetItem(strCsv[colMedVal]));
                ui->tableWidget_detalhe->setItem(linha, 4, new QTableWidgetItem(strCsv[colInd_39]));
                ui->tableWidget_detalhe->setItem(linha, 5, new QTableWidgetItem(strCsv[colInd_40]));
                ui->tableWidget_detalhe->setItem(linha, 6, new QTableWidgetItem(strCsv[colInd_41]));
                ui->tableWidget_detalhe->setItem(linha, 7, new QTableWidgetItem(strCsv[colInd_42]));

                ++linha;
            }

    }
}
