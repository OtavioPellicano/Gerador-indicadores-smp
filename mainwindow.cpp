#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget_processado->setColumnCount(7);
    QStringList header;
    header << "Prestadora" << "UF" << "SMP 10" << "SMP 11 Down" << "SMP 11 Up" << "Erro Estatístico" << "Medições Válidas";
    ui->tableWidget_processado->setHorizontalHeaderLabels(header);
    ui->tableWidget_processado->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_processado->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lineEdit_diretorio_origem->setEnabled(false);
    ui->tableWidget_processado->horizontalHeader()->setStretchLastSection(true);

    ui->actionRecuperar_UF->setEnabled(false);
    ui->actionRecuperar_Velocidade_Contratada->setEnabled(false);

    ui->actionProcessar->setEnabled(false);
    ui->actionSalvar->setEnabled(false);


    connect(ui->tableWidget_processado, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(cellSelected(int,int)));

    connect(ui->tableWidget_processado->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(on_headerDoubleClicked(int)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable()
{
    QStringList strCsv;
    int colPrest = 0, colUF = 2, colSmp10 = 10, colSmp11_down = 11, colSmp11_up = 12, colErro = 13, colMedVal = 4;
    int linha = 0;

    std::vector<QString> ufsAtualizadas;
    ufsAtualizadas.reserve((mVecIndicadores.size()/2) + 1);

    std::map<QString, long> mapMedVal;

    //for que agrupa as medidas validas (3G + 4G), separado po UF
    for(auto itQStr = mVecIndicadores.begin() + 1; itQStr != mVecIndicadores.end(); ++itQStr)
    {
        strCsv = (*itQStr).split(";");

        mapMedVal[strCsv[colUF]] += strCsv[colMedVal].toLong();
    }

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
            ui->tableWidget_processado->setItem(linha, 6, new QTableWidgetItem(QString::number(mapMedVal[strCsv[colUF]])));
            ufsAtualizadas.push_back(strCsv[colUF]);
            ++linha;
        }

    }

    atualizarCelulas();

}

QString MainWindow::formatarPorcentagem(const QString &qstr)
{
    double med = qstr.toDouble();

    med *= 100;

    return QString("%1%").arg(med);

}

void MainWindow::atualizarCelulas()
{
    double smp_10, smp_11_down, smp_11_up, erro_est;

    long medVal;

    mPA = new PlanoAmostral(this);
    vecQStrPair vecPaPrestadoraUF;
    vecPaPrestadoraUF = mPA->vecPrestadoraUf();
    std::pair<QString, QString> prestadoraUf;

    for(int linha = 0 ; linha < ui->tableWidget_processado->rowCount(); ++linha)
    {
        smp_10 = ui->tableWidget_processado->item(linha, 2)->text().split("%")[0].toDouble();
        smp_11_down = ui->tableWidget_processado->item(linha, 3)->text().split("%")[0].toDouble();
        smp_11_up = ui->tableWidget_processado->item(linha, 4)->text().split("%")[0].toDouble();
        erro_est = ui->tableWidget_processado->item(linha, 5)->text().split("%")[0].toDouble();
        medVal = ui->tableWidget_processado->item(linha, 6)->text().toLong();

        prestadoraUf = std::pair<QString, QString>(
                    ui->tableWidget_processado->item(linha, 0)->text(),
                    ui->tableWidget_processado->item(linha, 1)->text());
        if(std::find(vecPaPrestadoraUF.begin(), vecPaPrestadoraUF.end(), prestadoraUf) == vecPaPrestadoraUF.end())
        {
            ui->tableWidget_processado->item(linha, 0)->setBackground(Qt::darkMagenta);
            ui->tableWidget_processado->item(linha, 1)->setBackground(Qt::darkMagenta);
        }


        if(erro_est > 7 || medVal < 2430)
        {
            ui->tableWidget_processado->item(linha, 2)->setBackground(Qt::gray);
            ui->tableWidget_processado->item(linha, 3)->setBackground(Qt::gray);
            ui->tableWidget_processado->item(linha, 4)->setBackground(Qt::gray);
            continue;
        }

        if(smp_10 >= 95)
        {
            ui->tableWidget_processado->item(linha, 2)->setBackground(Qt::green);
        }
        else
        {
            ui->tableWidget_processado->item(linha, 2)->setBackground(Qt::red);
        }

        if(smp_11_down >= 80)
        {
            ui->tableWidget_processado->item(linha, 3)->setBackground(Qt::green);
        }
        else
        {
            ui->tableWidget_processado->item(linha, 3)->setBackground(Qt::red);
        }

        if(smp_11_up >= 80)
        {
            ui->tableWidget_processado->item(linha, 4)->setBackground(Qt::green);
        }
        else
        {
            ui->tableWidget_processado->item(linha, 4)->setBackground(Qt::red);
        }

    }

    delete mPA;
}

void MainWindow::cellSelected(const int &row, const int& /*col*/)
{

    mDetInd = new DetalheIndicador(this);

    mDetInd->setWindowTitle("Detalhe do Indicador");

    mDetInd->atualizarTabelaIndice(ui->tableWidget_processado->item(row, 0)->text(),
                                   ui->tableWidget_processado->item(row, 1)->text(),
                                   mVecIndicadores);

    mDetInd->exec();
}


void MainWindow::on_actionBuscarDiretorio_triggered()
{
    QFileDialog diretorioDialog(this);

    diretorioDialog.setNameFilter(tr("Text files (*.csv)"));
    diretorioDialog.setViewMode(QFileDialog::Detail);
    QString path = diretorioDialog.getExistingDirectory(this, tr("Diretório de Origem"), "/home");

    qDebug() << path;

    if(path != "")
    {
        ui->lineEdit_diretorio_origem->setText(path);
        ui->actionProcessar->setEnabled(true);
        ui->actionRecuperar_Velocidade_Contratada->setEnabled(true);
        ui->actionRecuperar_UF->setEnabled(true);
    }
}

void MainWindow::on_actionSalvar_triggered()
{
    if(mVecIndicadores.empty())
    {
        QMessageBox::critical(this, tr("Processamento"), QString("Processe o arquivo antes de exportar!"), QMessageBox::Ok);
        return;
    }

    QFileDialog diretorioDialog(this);
    diretorioDialog.setWindowModality(Qt::WindowModal);
    diretorioDialog.setAcceptMode(QFileDialog::AcceptSave);

    ofstream arqOut;

    if(diretorioDialog.exec() != QDialog::Accepted)
    {
        qDebug() << "false";
        return;
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

void MainWindow::on_actionProcessar_triggered()
{
    if(ui->lineEdit_diretorio_origem->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Processamento"), QString("Selecione um diretório válido!"), QMessageBox::Ok);
        return;
    }

    ui->statusbar->showMessage("Processando...", 1);

    mDirIn.cd(ui->lineEdit_diretorio_origem->text());

    mPrestInd = new PrestadoraIndicador(SMP);

    mRegDescarte = new RegrasDescarte(this);

    if(mRegDescarte->mapCodigoDescricaoInativo().empty())
    {

        mPrestInd->carregarMedicoes(mDirIn);
    }
    else
    {
        mPrestInd->carregarMedicoes(mDirIn, mRegDescarte->mapCodigoDescricaoInativo());
    }

    mVecIndicadores = mPrestInd->indicadores();

    updateTable();

    if(!mVecIndicadores.empty())
        ui->actionSalvar->setEnabled(true);

    QMessageBox::information(this, tr("Processamento"), QString("Processamento concluido!"), QMessageBox::Ok);

    delete mPrestInd;
    delete mRegDescarte;
}

void MainWindow::on_actionSair_triggered()
{
    this->close();
}

void MainWindow::on_actionDesenvolvedor_triggered()
{
    QMessageBox::about(this, tr("Desenvolvedor Gerador de Indicadores SMP"),
                       tr("<p>O <b>Gerador de Indicadores SMP</b> é um software desenvolvido "
                          "para simulação dos índices e indicadores do <b>SMP</b> através do rawdata diário ou mensal.</p>"

                          "<p>Desenvolvido com <b>Qt (C++)</b> por <b>Eng. Otavio Pellicano Moreira de Mello</b>, o software "
                          "apresenta os índices, indicadores, erro estatístico e validade operacional separados por célula e "
                          "tipo de conexão (3G ou 4G).</p>"

                          "<p>Legenda:</p>"

                          "<p>"
                              "<ul>"
                                  "<li><b>Meta:</b> 40% da Velocidade de Referência (Down e Up)</li>"
                                  "<li><b>Quociente:</b> Mediana por Velocidade de Referência</li>"
                                  "<li><b>Velocidade de Refência:</b> Velocidade Contratada</li>"
                              "</ul>"
                          "</p>"

                          "<p>"
                              "<ul>"
                                  "<li><b>Índice 039:</b> Quantidade de Medianas que atingiram a Meta</li>"
                                  "<li><b>Índice 040:</b> Quantidade total de Medianas</li>"
                                  "<li><b>Índice 041:</b> Somatório dos Quocientes Down</li>"
                                  "<li><b>Índice 042:</b> Somatório dos Quocientes Up</li>"
                              "</ul>"
                          "</p>"

                          "<p>"
                              "<ul>"
                                  "<li>"
                                      "<b>SMP 10 = (Índice 039)/(Índice 040)</b>"
                                  "</li>"
                                  "<li>"
                                      "<b>SMP 11 Down = (Índice 041)/(Índice 040)</b>"
                                  "</li>"
                                  "<li>"
                                      "<b>SMP 11 Up = (Índice 042)/(Índice 040)</b>"
                                  "</li>"
                              "</ul>"
                          "</p>"));
}

void MainWindow::on_actionPlano_Amostral_triggered()
{
    mPA = new PlanoAmostral(this);

    mPA->setWindowTitle("Plano Amostral");

    mPA->exec();

    delete mPA;

}

void MainWindow::on_actionRegras_de_Descarte_triggered()
{
    mRegDescarte = new RegrasDescarte(this);

    mRegDescarte->setWindowTitle("Regras de Descarte");

    mRegDescarte->exec();

    delete mRegDescarte;
}

void MainWindow::on_actionRecuperar_Velocidade_Contratada_triggered()
{
    QMessageBox msg(this);
    msg.setWindowTitle(tr("Recuperação de Velocidade Contratada"));
    msg.setText("Recuperando Velocidade Contratada...");
    msg.setStandardButtons(0);  //removendo todos os botões
    msg.show();
    QTest::qWait(1);    //Esperando a janela abrir

    mRecVelCont = new RecuperarVelocidadeContratada(QDir(ui->lineEdit_diretorio_origem->text()));

    if(mRecVelCont->processar())
    {
        msg.close();
        QMessageBox::information(this, tr("Recuperação da Velocidade Contratada"), QString("Velocidade Contratada recuperada!"), QMessageBox::Ok);
    }
    else
    {
        msg.close();
        QMessageBox::critical(this, tr("Recuperação da Velocidade Contratada"), QString("Erro ao tentar recuperar Velocidade Contratada!"), QMessageBox::Ok);
    }

    delete mRecVelCont;
}

void MainWindow::on_headerDoubleClicked(const int &col)
{
    if(col != 6)
        ui->tableWidget_processado->sortByColumn(col);
}

void MainWindow::on_actionRecuperar_UF_triggered()
{
    mRecUf = new RecuperarUfDialog(QDir(ui->lineEdit_diretorio_origem->text()) , this);

    mRecUf->setWindowTitle("Recuperar UF");

    mRecUf->exec();

    delete mRecUf;

}
