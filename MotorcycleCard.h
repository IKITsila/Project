#ifndef MOTORCYCLECARD_H
#define MOTORCYCLECARD_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>

class MotorcycleCard : public QWidget {
    Q_OBJECT
public:
    QString m_name;
    QString m_price;
    QString m_hp;
    QString m_color;
    QString m_country;

    MotorcycleCard(const QString& name, const QString& price, const QString& imagePath, const QString& hp,
                   const QString& color, const QString& country, const QString& username, QWidget *parent = nullptr)

        : QWidget(parent), m_name(name), m_price(price), m_hp(hp), m_color(color), m_country(country), fullPath("D:/qt/Projects/Project_4/images/" + imagePath),
          m_description(hp + " л.с. " + color + " " + country), m_username(username) {


        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* nameLabel = new QLabel(name);

        QLabel* descLabel = new QLabel(m_description);
        descLabel->setStyleSheet("QLabel {color: grey;}");

        QLabel* imageLabel = new QLabel(this);
        QPixmap image(fullPath);
        image = image.scaled(200, 150, Qt::KeepAspectRatio);
        imageLabel->setPixmap(image);

        QHBoxLayout* priceANDButton = new QHBoxLayout(this);
        QLabel* priceLabel = new QLabel(price + " руб. ");
        priceANDButton->addWidget(priceLabel);

        if (m_username == "admin") {
            QPushButton* redButton = new QPushButton("Change");
            redButton->setMaximumWidth(50);
            priceANDButton->addWidget(redButton);
            connect(redButton, &QPushButton::clicked, this, &MotorcycleCard::changeClicked);

            QPushButton* deleteButton = new QPushButton("Delete");
            deleteButton->setMaximumWidth(50);
            priceANDButton->addWidget(deleteButton);
            connect(deleteButton, &QPushButton::clicked, this, &MotorcycleCard::deleteClicked);

        }
        else{
            QPushButton* order = new QPushButton("Order");
            order->setMaximumWidth(50);
            priceANDButton->addWidget(order);
            connect(order, &QPushButton::clicked, this, &MotorcycleCard::orderClicked);
        }
        layout->addWidget(nameLabel);
        layout->addWidget(imageLabel);
        layout->addWidget(descLabel);
        layout->addLayout(priceANDButton);
    }

signals:
    void deleteClicked();
    void changeClicked();
    void orderClicked();

private:
    QString fullPath;
    QString m_description;
    QString m_username;

};

#endif // MOTORCYCLECARD_H


