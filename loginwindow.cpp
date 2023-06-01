#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLineEdit>
#include <QFile>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>

#include "loginwindow.h"
#include "mainwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    createLoginForm();
    createRegisterForm();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_loginLineEdit);
    mainLayout->addWidget(m_passwordLineEdit);
    mainLayout->addWidget(m_loginButton);
    mainLayout->addWidget(m_registerButton);
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_newLoginLineEdit);
    mainLayout->addWidget(m_newPasswordLineEdit);
    mainLayout->addWidget(m_createButton);
    mainLayout->addWidget(m_backButton);
    setLayout(mainLayout);

    setWindowTitle(tr("Login"));
}

void LoginWindow::on_loginButton_clicked()
{
    QString login = m_loginLineEdit->text();
    QString password = m_passwordLineEdit->text();
    if (login.isEmpty() || password.isEmpty()){
        QMessageBox::critical(this, "Ошибка входа!", "Должны быть заполнены все поля!");
        return;
    }
    QString username;
    bool found = false;

    // Чтение файла JSON и поиск пользователя
    QFile file("D:/qt/Projects/Project_3/users.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject object = doc.object();

        // Поиск пользователя по логину и паролю
        if (object.contains(login)) {
            QJsonObject userObject = object[login].toObject();
            SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
            if (crypto.decryptToString(userObject["password"].toString()) == password) {
                found = true;
                username = login;
            }
            else{
                QMessageBox::critical(this, "Ошибка входа!", "Неверно введенные данные логина и пароля!");
                return;
            }
        }

        file.close();
    }

    if (found) {
        // Показываем главное окно приложения
        MainWindow *mainWindow = new MainWindow(username);
        mainWindow->show();
        hide();
    } else {
        QMessageBox::critical(this, "Ошибка входа!", "Пользователя с данным логином не существует!");
    }
}

void LoginWindow::on_registerButton_clicked()
{
    setWindowTitle(tr("Register"));
    // Показываем форму для регистрации
    m_loginLineEdit->hide();
    m_passwordLineEdit->hide();
    m_loginButton->hide();
    m_registerButton->hide();
    m_newLoginLineEdit->clear();
    m_newLoginLineEdit->show();
    m_newLoginLineEdit->clear();
    m_newPasswordLineEdit->show();
    m_createButton->show();
    m_backButton->show();
}

void LoginWindow::createLoginForm()
{
    m_loginLineEdit = new QLineEdit;
    m_passwordLineEdit = new QLineEdit;
    m_passwordLineEdit->setEchoMode(QLineEdit::Password);
    m_loginButton = new QPushButton(tr("Login"));
    m_registerButton = new QPushButton(tr("Register"));

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::on_loginButton_clicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::on_registerButton_clicked);
}

void LoginWindow::createRegisterForm()
{
    m_newLoginLineEdit = new QLineEdit;
    m_newPasswordLineEdit = new QLineEdit;
    m_createButton = new QPushButton(tr("Create"));
    m_backButton = new QPushButton(tr("Back"));

    m_newLoginLineEdit->hide();
    m_newPasswordLineEdit->hide();
    m_createButton->hide();
    m_backButton->hide();

    connect(m_backButton, &QPushButton::clicked, [this]() {
        setWindowTitle(tr("Login"));
        m_newLoginLineEdit->hide();
        m_newPasswordLineEdit->hide();
        m_createButton->hide();
        m_backButton->hide();
        m_loginLineEdit->clear();
        m_loginLineEdit->show();
        m_passwordLineEdit->clear();
        m_passwordLineEdit->show();
        m_loginButton->show();
        m_registerButton->show();
    });

    connect(m_createButton, &QPushButton::clicked, [this]() {
        if (m_newLoginLineEdit->text().contains(" ") || m_newLoginLineEdit->text().length()<4 || m_newLoginLineEdit->text().length()>16){
            QMessageBox::critical(this, "Ошибка ввода данных", "Неверный ввода логина!");
            m_newLoginLineEdit->clear();
            m_newPasswordLineEdit->clear();
            return;
        }
        if(m_newPasswordLineEdit->text().contains(" ") || m_newPasswordLineEdit->text().length()<6 || m_newPasswordLineEdit->text().length()>20){
            QMessageBox::critical(this, "Ошибка ввода данных", "Неверный ввода пароля!");
            m_newPasswordLineEdit->clear();
            return;
        }
        QString login = m_newLoginLineEdit->text();
        QString password = m_newPasswordLineEdit->text();
        SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
        QString crypted_passwd = crypto.encryptToString(password);
        if (login.isEmpty() || password.isEmpty()){
            QMessageBox::critical(this, "Ошибка входа!", "Должны быть заполнены все поля!");
            return;
        }

        // Чтение файла
        QFile file("D:/qt/Projects/Project_3/users.json");
           if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
               QByteArray data = file.readAll();
               QJsonDocument doc = QJsonDocument::fromJson(data);
               QJsonObject object = doc.object();

               // Проверка, что пользователя с таким логином нет в файле
               if (!object.contains(login)) {
                   // Создание нового объекта пользователя
                   QJsonObject newUserObject;
                   newUserObject["password"] = crypted_passwd;

                   // Добавление нового пользователя в файл
                   object[login] = newUserObject;
                   doc.setObject(object);
                   file.seek(0);
                   file.write(doc.toJson());
                   file.close();

                   // Переключение на форму логина
                   m_newLoginLineEdit->hide();
                   m_newPasswordLineEdit->hide();
                   m_createButton->hide();
                   m_backButton->hide();
                   m_loginLineEdit->setText(login);
                   m_loginLineEdit->show();
                   m_passwordLineEdit->setText(password);
                   m_passwordLineEdit->show();
                   m_loginButton->show();
                   m_registerButton->show();
               } else {
                   QMessageBox::critical(this, "Ошибка регистрации", "Пользователь с данным логином уже существует!");
               }
           }
       });
}
