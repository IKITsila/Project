#include "ChangeWindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>

ChangeWindow::ChangeWindow(QString name, QString hp, QString color, QString country, QString price,QWidget *parent)
    : QWidget(parent)
    , id(name)
{
    QVBoxLayout* layout = new QVBoxLayout;
    nameEdit = new QLineEdit(name);
    hpEdit = new QLineEdit(hp);
    colorEdit = new QLineEdit(color);
    countryEdit = new QLineEdit(country);
    priceEdit = new QLineEdit(price);
    layout->addWidget(nameEdit);
    layout->addWidget(hpEdit);
    layout->addWidget(priceEdit);
    layout->addWidget(colorEdit);
    layout->addWidget(countryEdit);

    QHBoxLayout* buttons = new QHBoxLayout;
    save = new QPushButton("save");
    discard = new QPushButton("discard");
    buttons->addWidget(save);
    buttons->addWidget(discard);
    layout->addLayout(buttons);
    setLayout(layout);

    connect(save, &QPushButton::clicked, this, &ChangeWindow::on_save_clicked);

    connect(discard, &QPushButton::clicked, this, &ChangeWindow::on_discard_clicked);

}

void ChangeWindow::on_discard_clicked(){
    close();
}

void ChangeWindow::on_save_clicked(){

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
        if(model["name"] == id){
            model["name"] = nameEdit->text();
            model["price"] = priceEdit->text();
                QJsonObject desc = model["description"].toObject();
                desc["hp"] = hpEdit->text();
                desc["color"] = colorEdit->text();
                desc["country"] = countryEdit->text();
            model["description"] = desc;
            products.replace(i, model);
            prod["products"] = products;

        }
    }

    document.setObject(prod);


    QFile writeFile("D:/qt/Projects/Project_4/products.json");
    if (!writeFile.open(QFile::WriteOnly | QFile::Truncate)) {

    }
    QTextStream wirteStream(&writeFile);
    wirteStream << document.toJson();
    writeFile.close();
    close();
}

void ChangeWindow::closeEvent(QCloseEvent* event) {
    emit windowClosed();
    QWidget::closeEvent(event);
}
