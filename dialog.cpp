#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->tableWidget_processado->setColumnCount(6);
    QStringList header;
    header << "Prestadora" << "UF" << "SMP 10" << "SMP 11 Down" << "SMP 11 Up" << "Erro Estatístico";
    ui->tableWidget_processado->setHorizontalHeaderLabels(header);
    ui->tableWidget_processado->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_processado->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateTable()
{
    QStringList strCsv;
    int colPrest = 0, colUF = 2, colSmp10 = 10, colSmp11_down = 11, colSmp11_up = 12, colErro = 13;
    int linha = 0;

    std::vector<QString> ufsAtualizadas;
    ufsAtualizadas.reserve((mVecIndicadores.size()/2) + 1);

    ui->tableWidget_processado->clearContents();
    ui->tableWidget_processado->setRowCount(0);

    for(auto itQStr = mVecIndicadores.begin() + 1; itQStr != mVecIndicadores.end(); ++itQStr)
    {
        strCsv = (*itQStr).split(";");

        if(std::find(ufsAtualizadas.begin(), ufsAtualizadas.end(), strCsv[colUF]) == ufsAtualizadas.end())
        {

            ui->tableWidget_processado->setRowCount(linha + 1);
            ui->tableWidget_processado->setItem(linha, 0, new QTableWidgetItem(strCsv[colPrest]));
            ui->tableWidget_processado->setItem(linha, 1, new QTableWidgetItem(strCsv[colUF]));
            ui->tableWidget_processado->setItem(linha, 2, new QTableWidgetItem(formatarPorcentagem(strCsv[colSmp10])));
            ui->tableWidget_processado->setItem(linha, 3, new QTableWidgetItem(formatarPorcentagem(strCsv[colSmp11_down])));
            ui->tableWidget_processado->setItem(linha, 4, new QTableWidgetItem(formatarPorcentagem(strCsv[colSmp11_up])));
            ui->tableWidget_processado->setItem(linha, 5, new QTableWidgetItem(formatarPorcentagem(strCsv[colErro])));
            ufsAtualizadas.push_back(strCsv[colUF]);
            ++linha;

        }

    }
}

QString Dialog::formatarPorcentagem(const QString &qstr)
{
    double med = qstr.toDouble();

    med *= 100;

    return QString("%1%").arg(med);

}

void Dialog::on_pushButton_buscar_clicked()
{
    QFileDialog diretorioDialog(this);

    QString path = diretorioDialog.getExistingDirectory(this, tr("Diretório de Origem"));

    qDebug() << path;

    if(path != "")
    {
        ui->lineEdit_diretorio_origem->setText(path);
    }
}

void Dialog::on_pushButton_processar_clicked()
{

    if(ui->lineEdit_diretorio_origem->text().isEmpty())
        return;

    mDirIn.cd(ui->lineEdit_diretorio_origem->text());

    mPrestInd = new PrestadoraIndicador(SMP);
    mPrestInd->carregarMedicoes(mDirIn);
    mVecIndicadores = mPrestInd->indicadores();

    updateTable();

    QMessageBox::information(this, tr("Processamento"), QString("Processamento concluido!"), QMessageBox::Ok);

    delete mPrestInd;
}

void Dialog::on_pushButton_exportar_clicked()
{
    if(mVecIndicadores.empty())
    {
        QMessageBox::critical(this, tr("Processamento"), QString("Processe o arquivo, arrombado!"), QMessageBox::Ok);
        return;
    }

    QFileDialog diretorioDialog(this);
    diretorioDialog.setWindowModality(Qt::WindowModal);
    diretorioDialog.setAcceptMode(QFileDialog::AcceptSave);

    ofstream arqOut;

    if(diretorioDialog.exec() != QDialog::Accepted)
    {
        qDebug() << "false";
    }
    else
    {
        arqOut.open(diretorioDialog.selectedFiles()[0].toStdString());
        if(arqOut.is_open())
        {
            for(auto itQStr = mVecIndicadores.begin(); itQStr != mVecIndicadores.end(); ++itQStr)
            {
                arqOut << (*itQStr).toStdString() << std::endl;
            }
            arqOut.close();
        }
        else
        {
            qDebug() << "Erro ao abrir arquivo destino";

        }
    }


    QMessageBox::information(this, tr("Processamento"), QString("Arquivo exportado no caminho:\n%1").arg(diretorioDialog.selectedFiles()[0]), QMessageBox::Ok);


}
