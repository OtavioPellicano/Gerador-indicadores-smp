#include "recuperarufdialog.h"
#include "ui_recuperarufdialog.h"

RecuperarUfDialog::RecuperarUfDialog(const QDir &dirOrigem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecuperarUfDialog),
    mDirOrigem(dirOrigem)
{
    ui->setupUi(this);

    ui->lineEdit_diretorio_rawdata_bruto->setEnabled(false);
    ui->pushButton_buscar->setEnabled(false);

//    ui->checkBox_4g->setEnabled(false);
}

RecuperarUfDialog::~RecuperarUfDialog()
{
    delete ui;
}

void RecuperarUfDialog::on_checkBox_4g_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->pushButton_buscar->setEnabled(true);
    }
    else
    {
        ui->pushButton_buscar->setEnabled(false);
    }
}

void RecuperarUfDialog::on_pushButton_recuperar_uf_clicked()
{

    if(!ui->checkBox_3g->isChecked() && !ui->checkBox_4g->isChecked())
    {
        QMessageBox::warning(this, tr("Recuperação de UF"), tr("Selecione a Conexão!"), QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_diretorio_rawdata_bruto->text().isEmpty() && ui->checkBox_4g->isChecked())
    {
        QMessageBox::warning(this, tr("Recuperação de UF"), tr("Selecione o diretório do rawdata bruto (antes de passar pelo motor)"), QMessageBox::Ok);
        return;
    }

    QMessageBox msg(this);
    msg.setWindowTitle(tr("Recuperação de UF"));
    msg.setText("Recuperando  UF...");
    msg.setStandardButtons(0);  //removendo todos os botões
    msg.show();
    QTest::qWait(1);    //Esperando a janela abrir

    mRecUF = new RecuperarUF;
    if(ui->checkBox_3g->isChecked())
    {
        mRecUF->recuperar3G(mDirOrigem);
    }

    if(ui->checkBox_4g->isChecked())
    {
        mRecUF->recuperar4G(mDirOrigem, mDirRawdataBruto);

        //Tenho que fazer para o 4G
    }


    msg.close();
    QMessageBox::information(this, tr("Recuperação de UF"), QString("Medições com UF:\n3G Pré-processamento: %1\n3G Pós-processamento: %2"
                                                                    "\n4G Pré-processamento: %3\n4G Pós-processamento: %4"
                                                                    "\n3G + 4G Pré-processamento: %5\n3G + 4G Pós-processamento: %6").arg(mRecUF->totalMedRecupInicial3G()).arg(mRecUF->totalMedRecup3G()).arg(mRecUF->totalMedRecupInicial4G()).arg(mRecUF->totalMedRecup4G()).arg(mRecUF->totalMedRecupInicial3Ge4G()).arg(mRecUF->totalMedRecup3Ge4G()), QMessageBox::Ok);

    delete mRecUF;
}

void RecuperarUfDialog::on_pushButton_buscar_clicked()
{
    QFileDialog diretorioDialog(this);

    diretorioDialog.setNameFilter(tr("Text files (*.csv)"));
    diretorioDialog.setViewMode(QFileDialog::Detail);
    QString path = diretorioDialog.getExistingDirectory(this, tr("Diretório de Origem"));

    qDebug() << path;

    if(path != "")
    {
        ui->lineEdit_diretorio_rawdata_bruto->setText(path);
        mDirRawdataBruto.cd(ui->lineEdit_diretorio_rawdata_bruto->text());
    }

}
