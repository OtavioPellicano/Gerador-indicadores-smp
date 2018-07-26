#include "planoamostral.h"
#include "ui_planoamostral.h"

PlanoAmostral::PlanoAmostral(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanoAmostral)
{
    ui->setupUi(this);
    QStringList header;
    header << "Prestadora" << "UF";
    ui->tableWidget_pa->setColumnCount(2);
    ui->tableWidget_pa->setHorizontalHeaderLabels(header);

    ui->pushButton_aplicar->setEnabled(false);

    ui->tableWidget_pa->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_pa->setSelectionBehavior(QAbstractItemView::SelectRows);

    vecQStrPair vecTemp;

    carregarTabelaAtualDoArquivo(vecTemp);
    atualizarTabelaPA(vecTemp);

    mVecPrestadoraUf = vecTemp;
}

PlanoAmostral::~PlanoAmostral()
{
    delete ui;
}

bool PlanoAmostral::validarPrest(const QString &qstrPrest)
{
    if(std::find(mVecPrestStd.begin(), mVecPrestStd.end(), qstrPrest) != mVecPrestStd.end())
        return true;
    else
        return false;
}

bool PlanoAmostral::validarUf(const QString &qstrUf)
{
    if(std::find(mVecUfStd.begin(), mVecUfStd.end(), qstrUf) != mVecUfStd.end())
        return true;
    else
        return false;
}

bool PlanoAmostral::validarPA(const QString &paFile)
{
    std::ifstream arq(paFile.toStdString());
    vecQStrPair vecPrestadoraUf;
    mVecErroPrest.clear();
    mVecErroUf.clear();

    if(arq.is_open())
    {
        QStringList strCsv;
        std::string str;
        for(size_t i = 0; std::getline(arq, str); ++i)
        {
            //Retirando o cabeçalho
            if(i == 0)
                continue;

            strCsv = QString::fromStdString(str).split((";"));
            vecPrestadoraUf.push_back(std::pair<QString, QString>(strCsv[0], strCsv[1]));

            if(!validarPrest(strCsv[0]))
            {
                if(std::find(mVecErroPrest.begin(), mVecErroPrest.end(),strCsv[0]) == mVecErroPrest.end())
                    mVecErroPrest.push_back(strCsv[0]);
            }

            if(!validarUf(strCsv[1]))
            {
                if(std::find(mVecErroUf.begin(), mVecErroUf.end(),strCsv[1]) == mVecErroUf.end())
                mVecErroUf.push_back(strCsv[1]);
            }
        }

        arq.close();
    }
    else
    {
        QMessageBox::critical(this, tr("Importar Plano Amostral"), tr("Erro ao abrir o arquivo!"), QMessageBox::Ok);
        return false;
    }

    if(mVecErroPrest.empty() && mVecErroUf.empty())
    {
        mVecPrestadoraUf = vecPrestadoraUf;

        QMessageBox::information(this, tr("Importar Plano Amostral"), tr("Plano Amostral importado!"), QMessageBox::Ok);

        return true;
    }
    else
    {
        QMessageBox::critical(this, tr("Importar Plano Amostral"), tr("Erro de formato!"), QMessageBox::Ok);
        return false;
    }

}

void PlanoAmostral::atualizarTabelaPA(const vecQStrPair& vecPrestadoraUf)
{
    ui->tableWidget_pa->clearContents();

    size_t linha = 0;

    for(auto itVec = vecPrestadoraUf.begin(); itVec != vecPrestadoraUf.end(); ++itVec)
    {
        ui->tableWidget_pa->setRowCount(linha + 1);
        ui->tableWidget_pa->setItem(linha, 0, new QTableWidgetItem(itVec->first));
        ui->tableWidget_pa->setItem(linha, 1, new QTableWidgetItem(itVec->second));

        ++linha;
    }

}

void PlanoAmostral::carregarTabelaAtualDoArquivo(vecQStrPair &vecPrestadoraUf)
{
    vecPrestadoraUf.clear();
    QDir dir;
    std::ifstream arq((dir.absolutePath() + "/PA/" + finaNamePA).toStdString());
    std::string str;
    QStringList strCsv;

    if(arq.is_open())
    {
        for(size_t i = 0; std::getline(arq,str); ++i)
        {
            if(i == 0)
                continue;
            strCsv = QString::fromStdString(str).split(";");
            vecPrestadoraUf.push_back(std::pair<QString, QString>(strCsv[0], strCsv[1]));
        }
    }
    else
    {
        qDebug() << "nao existe arquivo de PA";
    }

}

QString PlanoAmostral::messagemBoxForaFormato()
{
    QString msgAnt;
    QString msgMeiuca;
    QString msgPos;
    msgAnt = "<p>Itens fora do formato:</p><p>Prestadora</p>"
            "<ui>";
    if(mVecErroPrest.size() > 5)
    {
        msgMeiuca = "<li>-> Mais de 5 itens</li>";
    }
    else
    {
        for(auto itVecP = mVecErroPrest.begin(); itVecP != mVecErroPrest.end(); ++itVecP)
        {
            msgMeiuca += "<li>-> " % *itVecP % "</li>";
        }
    }



    msgPos = msgMeiuca % "</ui>"
                "<p>UF</p>"
                "<ui>";

    msgAnt = msgAnt % msgPos;
    msgMeiuca.clear();

    if(mVecErroUf.size() > 5)
    {
        msgMeiuca = "<li>-> Mais de 5 itens</li>";
    }
    else
    {
        for(auto itVec = mVecErroUf.begin(); itVec != mVecErroUf.end(); ++itVec)
        {
            msgMeiuca += "<li>-> " % *itVec % "</li>";
        }
    }

    msgPos = msgMeiuca % "</ui>";


    return msgAnt % msgPos;

}

void PlanoAmostral::carregarTabelaAtual(vecQStrPair& vecUfPrestadora)
{

    vecUfPrestadora.clear();
    vecUfPrestadora.reserve(ui->tableWidget_pa->rowCount());

    for(auto i = 0; i < ui->tableWidget_pa->rowCount(); ++i)
    {
        vecUfPrestadora.push_back(std::pair<QString, QString>(ui->tableWidget_pa->item(i,0)->text(), ui->tableWidget_pa->item(i,1)->text()));
    }

}

bool PlanoAmostral::salvarPA(const vecQStrPair& vecPrestadoraUf)
{
    QDir dirPA;
    dirPA.mkdir("PA");
    dirPA.cd("PA");

    std::ofstream arq((dirPA.absolutePath() + "/" + finaNamePA).toStdString());

    if(arq.is_open())
    {
        arq << "Prestadora;UF" << std::endl;
        for(auto itVec = vecPrestadoraUf.begin(); itVec != vecPrestadoraUf.end(); ++itVec)
        {
            arq << itVec->first.toStdString() << ";" << itVec->second.toStdString() << std::endl;
        }
    }
    else
    {
        qDebug() << "Impossivel salvar PA";
        return false;
    }

    return true;
}

void PlanoAmostral::on_pushButton_importar_clicked()
{
    QFileDialog fileName(this);

    QString path = fileName.getOpenFileName(this, tr("Diretório de Origem"), "/home",
                                            tr("Text files(*.csv)"));

    qDebug() << path;

    if(path != "")
    {
        if(validarPA(path))
        {
            atualizarTabelaPA(mVecPrestadoraUf);
            ui->pushButton_aplicar->setEnabled(true);
        }
        else
        {
            QMessageBox::critical(this, tr("Importar Plano Amostral"), tr(messagemBoxForaFormato().toUtf8()),QMessageBox::Ok);
        }
    }

}

void PlanoAmostral::on_pushButton_aplicar_clicked()
{
    vecQStrPair vecPrestadoraUf;
    carregarTabelaAtual(vecPrestadoraUf);

    if(vecPrestadoraUf.empty())
    {
        QMessageBox::critical(this, tr("Importar Plano Amostral"), tr("Tabela vazia!"), QMessageBox::Ok);
    }
    else
    {
        if(salvarPA(vecPrestadoraUf))
        {
            QMessageBox::information(this, tr("Importar Plano Amostral"), tr("Plano amostral salvo com sucesso!"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this, tr("Importar Plano Amostral"), tr("Erro ao salvar o Plano Amostral!\nTabela não salva!"), QMessageBox::Ok);
        }
    }
}

vecQStrPair PlanoAmostral::vecPrestadoraUf() const
{
    return mVecPrestadoraUf;
}

