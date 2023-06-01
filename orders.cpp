#include "orders.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>
#include <QJsonParseError>
#include <QStringList>
#include <QVBoxLayout>
#include <QTableWidget>

Orders::Orders(const QString username,QWidget *parent)
    : QWidget(parent)
    , m_username(username)
{

    setWindowTitle("Заказы");

    QTableWidget* orders = new QTableWidget(this);
    orders->setMinimumHeight(250);
    orders->setMinimumWidth(400);
    QVBoxLayout* see = new QVBoxLayout();
    orders->setColumnCount(3);
    orders->setHorizontalHeaderLabels(QStringList() << "Клиент" << "Модель" << "Цена");
    see->addWidget(orders);

    QString filename = "D:/qt/Projects/Project_4/order.json";
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Could not open file " + filename);
    }
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(nullptr, "Error", "Error parsing json file " + filename + ": " + error.errorString());
    }
    file.close();

    QJsonObject prod = document.object();
    QJsonArray products = prod["orders"].toArray();
    int countVisibleorders = 0;
    if(m_username == "admin"){
        orders->setRowCount(products.size());
        for(int i = 0; i < products.size(); i++){
            QJsonObject model = products.at(i).toObject();
            qDebug() << model;
            QTableWidgetItem* nameC = new QTableWidgetItem(model["name"].toString());
            qDebug() << model["name"].toString();
            QTableWidgetItem* nameM = new QTableWidgetItem(model["modelName"].toString());
            qDebug() << model["modelName"].toString();
            QTableWidgetItem* priceM = new QTableWidgetItem(model["price"].toString());
            orders->setItem(i, 0, nameC);
            orders->setItem(i, 1, nameM);
            orders->setItem(i, 2, priceM);
        }
    }
    else{
        int visible = 0;
        for(int i = 0; i < products.size(); i++){
            QJsonObject model = products.at(i).toObject();
            if(model["name"] == m_username)
            {
                visible++;
            }
        }
         orders->setRowCount(visible);
        int t = 0;
        for(int i = 0; i < products.size(); i++){
            QJsonObject model = products.at(i).toObject();
            if(model["name"] == m_username)
            {
                QTableWidgetItem* nameC = new QTableWidgetItem(model["name"].toString());
                qDebug() << model["name"].toString();
                QTableWidgetItem* nameM = new QTableWidgetItem(model["modelName"].toString());
                qDebug() << model["modelName"].toString();
                QTableWidgetItem* priceM = new QTableWidgetItem(model["price"].toString());
                orders->setItem(t, 0, nameC);
                orders->setItem(t, 1, nameM);
                orders->setItem(t, 2, priceM);
                ++t;

            }
        }


    }
    resize(400, 300);
}
