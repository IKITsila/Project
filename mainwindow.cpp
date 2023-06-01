#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QJsonParseError>
#include "motorcyclecard.h"
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QTextStream>
#include <QDialog>
#include <QLineEdit>
#include "ChangeWindow.h"
#include "orders.h"
#include <QDateTime>

QJsonObject readJsonFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Could not open file " + filename);
        return QJsonObject();
    }
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(nullptr, "Error", "Error parsing json file " + filename + ": " + error.errorString());
        return QJsonObject();
    }
    return document.object();
}

MainWindow::MainWindow(const QString& username,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_username(username)
{
    ui->setupUi(this);

    QGridLayout* centralLayout = new QGridLayout();
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);

    QHBoxLayout* outting = new QHBoxLayout(this);
    QLabel* Name = new QLabel(m_username);
    centralLayout->addWidget(Name);
    QPushButton* logout = new QPushButton("out");
    connect(logout, &QPushButton::clicked, this, &MainWindow::on_logout_clicked);
    QPushButton* orders = new QPushButton("orders");
    connect(orders, &QPushButton::clicked, this, &MainWindow::seeOrders);

    outting->addWidget(Name);
    outting -> addWidget(orders);
    outting->addWidget(logout);
    centralLayout->addLayout(outting,0,0);

    loadMotorcycles();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow;
    loginWindow->show();
    close();
}

void MainWindow::loadMotorcycles(){

    QJsonObject prod = readJsonFile("D:/qt/Projects/Project_4/products.json");
    QJsonArray products = prod.value("products").toArray();

    for (const auto &product : products) {
        QJsonObject obj = product.toObject();
        QString name = obj["name"].toString();
        QString price = obj["price"].toString();

        QJsonObject description = obj["description"].toObject();

        QString color = description["color"].toString();
        QString country = description["country"].toString();
        QString hp = description["hp"].toString();

        QString imagePath = obj["image"].toString();

        MotorcycleCard* card = new MotorcycleCard(name, price, imagePath, hp, color, country, m_username);
        connect(card, &MotorcycleCard::deleteClicked, this, &MainWindow::deleteMotorcycleCard);
        connect(card, &MotorcycleCard::changeClicked, this, &MainWindow::changeMotorcycleCard);
        connect(card, &MotorcycleCard::orderClicked, this, &MainWindow::makeOrder);
        m_motorcycleCards.append(card);
        centralWidget()->layout()->addWidget(card);

    }


}

void MainWindow::deleteMotorcycleCard() {

    MotorcycleCard* card = qobject_cast<MotorcycleCard*>(sender());
    if (card) {
        centralWidget()->layout()->removeWidget(card);
        m_motorcycleCards.removeOne(card);
        delete card;

        QString filename = "D:/qt/Projects/Project_4/products.json";
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file " + filename);
        }
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError) {
            QMessageBox::critical(nullptr, "Error", "Error parsing json file " + filename + ": " + error.errorString());
        }
        file.close();
        QJsonObject prod = document.object();
        QJsonArray products = prod["products"].toArray();

        for(int i = 0; i < products.size();i++){
            QJsonObject model = products.at(i).toObject();
            if(model["name"] == card->m_name){

                products.removeAt(i);
                prod["products"] = products;

            }
        }

        document.setObject(prod);
        QFile writeFile("D:/qt/Projects/Project_4/products.json");
        if (!writeFile.open(QFile::WriteOnly | QFile::Truncate)) {
            return;
        }
        QTextStream wirteStream(&writeFile);
        wirteStream << document.toJson();
        writeFile.close();

    }
}

void MainWindow::changeMotorcycleCard(){

    MotorcycleCard* card = qobject_cast<MotorcycleCard*>(sender());
    ChangeWindow* change = new ChangeWindow(card->m_name, card->m_hp, card->m_color, card->m_country, card->m_price);
    change->show();
    connect(change, &ChangeWindow::windowClosed, this, &MainWindow::handleChildWindowClosed);
}

void MainWindow::makeOrder(){

    MotorcycleCard* card = qobject_cast<MotorcycleCard*>(sender());

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

    for(int i = 0; i < products.size();i++){
        QJsonObject model = products.at(i).toObject();

        if(model["name"] == m_username){
            if(model["modelName"] == card->m_name){
            QMessageBox::critical(this, "Ошибка создания заявки!", "Вы уже создали заявку на данную модель!");
            return;
            }
            else{
                continue;
            }
        }
        else{
          continue;
        }

    }

    QJsonObject newOrder;
    newOrder.insert("name", m_username);
    newOrder.insert("modelName", card->m_name);
    newOrder.insert("price", card->m_price);
    products.append(newOrder);
    prod["orders"] = products;
    document.setObject(prod);
    qDebug() << document;
    file.open(QIODevice::Truncate);
    file.close();

    file.open((QIODevice::WriteOnly | QIODevice::Text));

    QTextStream wirteStream(&file);
    wirteStream << document.toJson();
    file.close();
    QMessageBox::information(this, "Создание заявки", "Заявка успешно создана! ");

}

void MainWindow::seeOrders(){

    Orders* orders = new Orders(m_username);
    orders->show();
}
