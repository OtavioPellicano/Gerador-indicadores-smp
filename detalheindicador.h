#ifndef DETALHEINDICADOR_H
#define DETALHEINDICADOR_H

#include <QDialog>
#include <QStringList>
#include <vector>


typedef std::vector<QString> vecQStr;

namespace Ui {
class DetalheIndicador;
}

class DetalheIndicador : public QDialog
{
    Q_OBJECT

public:
    explicit DetalheIndicador(QWidget *parent = 0);
    ~DetalheIndicador();

    void atualizarTabelaIndice(const QString& prestadora, const QString& uf, const vecQStr& vecIndicadores);

private:
    Ui::DetalheIndicador *ui;
};

#endif // DETALHEINDICADOR_H
