class MotorcycleCard : public QWidget {
    Q_OBJECT
public:
    MotorcycleCard(const QString& name, const QString& description, const QString& username, QWidget *parent = nullptr)
        : QWidget(parent), m_name(name), m_description(description), m_username(username) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* nameLabel = new QLabel(name);
        QLabel* descLabel = new QLabel(description);
        layout->addWidget(nameLabel);
        layout->addWidget(descLabel);

        if (m_username == "admin") {
            QPushButton* deleteButton = new QPushButton("Delete");
            layout->addWidget(deleteButton);
            connect(deleteButton, &QPushButton::clicked, this, &MotorcycleCard::deleteClicked);
        }
    }

signals:
    void deleteClicked();

private:
    QString m_name;
    QString m_description;
    QString m_username;
};
