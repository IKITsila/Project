#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include "simplecrypt.h"

class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    void createLoginForm();
    void createRegisterForm();

    QLineEdit *m_loginLineEdit;
    QLineEdit *m_passwordLineEdit;
    QPushButton *m_loginButton;
    QPushButton *m_registerButton;

    QLineEdit *m_newLoginLineEdit;
    QLineEdit *m_newPasswordLineEdit;
    QPushButton *m_createButton;
    QPushButton *m_backButton;
};

#endif // LOGINWINDOW_H
