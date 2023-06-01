#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include "motorcyclecard.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& username, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_username;
    QList<MotorcycleCard*> m_motorcycleCards;
    void loadMotorcycles();

public slots:
        void changeMotorcycleCard();

private slots:
    void on_logout_clicked();
    void deleteMotorcycleCard();
    void makeOrder();
    void seeOrders();

    void handleChildWindowClosed() {
        QLayout* layout = centralWidget()->layout();
        QLayoutItem* item;
        while ((item = layout->takeAt(1)) != nullptr) {
            delete item->widget();
            delete item;
        }
        m_motorcycleCards.clear();
        loadMotorcycles();
    }



//    void saveClicked();
};
#endif // MAINWINDOW_H
