#ifndef ORDERS_H
#define ORDERS_H
#include <QTableWidget>

class Orders : public QWidget
{
    Q_OBJECT

public:
    QString m_username;

    Orders(const QString username , QWidget *parent = nullptr);
};

#endif // ORDERS_H
