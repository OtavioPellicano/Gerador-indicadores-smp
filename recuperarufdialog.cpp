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

    ui->checkBox_4g->setEnabled(false);
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
        //Tenho que fazer para o 4G
    }


    msg.close();
    QMessageBox::information(this, tr("Recuperação de UF"), QString("Medições com UF:\n3G Pré-processamento: %1\n3G Pós-processamento: %2").arg(mRecUF->totalMedRecupInicial3G()).arg(mRecUF->totalMedRecup3G()), QMessageBox::Ok);

    delete mRecUF;
}
