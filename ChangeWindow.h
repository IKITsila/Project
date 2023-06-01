#ifndef CHANGEWINDOW_H
#define CHANGEWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

class ChangeWindow : public QWidget
{
    Q_OBJECT

public:
    QString id;
    ChangeWindow(QString name, QString hp, QString color, QString country, QString price, QWidget *parent = nullptr);

private slots:
    void on_save_clicked();
    void on_discard_clicked();

private:
    QLineEdit* nameEdit;
    QLineEdit* hpEdit;
    QLineEdit* colorEdit;
    QLineEdit* countryEdit;
    QLineEdit* priceEdit;

    QPushButton* save;
    QPushButton* discard;

signals:
    void windowClosed();

protected:
    void closeEvent(QCloseEvent* event) override;
};
#endif // CHANGEWINDOW_H
