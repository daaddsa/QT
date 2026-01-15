#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    connect(ui->btnClose, &QToolButton::clicked, this, &QWidget::close);
    connect(ui->btnClear, &QPushButton::clicked, this, [this]() {
        ui->txtQuery->clear();
        ui->listUsers->clear();
        ui->listGroups->clear();
        ui->listMessages->clear();
        ui->lblStatus->setText("请输入关键字进行搜索");
    });

    connect(ui->btnSearch, &QPushButton::clicked, this, [this]() { performSearch(); });
    connect(ui->txtQuery, &QLineEdit::returnPressed, this, [this]() { performSearch(); });
    connect(ui->comboScope, &QComboBox::currentIndexChanged, this, [this](int idx) {
        if (idx == 1) ui->tabResults->setCurrentWidget(ui->tabUsers);
        else if (idx == 2) ui->tabResults->setCurrentWidget(ui->tabGroups);
        else if (idx == 3) ui->tabResults->setCurrentWidget(ui->tabMessages);
        performSearch();
    });

    connect(ui->listUsers, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        if (!item) return;
        const QString u = item->text().trimmed();
        if (u.isEmpty()) return;
        emit contactActivated(u);
    });
}

searchDialog::~searchDialog()
{
    delete ui;
}

void searchDialog::setContacts(const QStringList &users)
{
    m_contacts = users;
}

void searchDialog::setGroups(const QStringList &groups)
{
    m_groups = groups;
}

void searchDialog::setMessages(const QStringList &messages)
{
    m_messages = messages;
}

void searchDialog::setQueryAndSearch(const QString &query)
{
    ui->txtQuery->setText(query);
    performSearch();
}

void searchDialog::performSearch()
{
    const QString q = ui->txtQuery->text().trimmed();
    const int scope = ui->comboScope->currentIndex();

    ui->listUsers->clear();
    ui->listGroups->clear();
    ui->listMessages->clear();

    if (q.isEmpty()) {
        updateStatus(0, 0, 0);
        return;
    }

    const Qt::CaseSensitivity cs = Qt::CaseInsensitive;

    int userCount = 0;
    int groupCount = 0;
    int messageCount = 0;

    if (scope == 0 || scope == 1) {
        for (const QString &u : m_contacts) {
            if (u.contains(q, cs)) {
                ui->listUsers->addItem(u);
                ++userCount;
            }
        }
    }

    if (scope == 0 || scope == 2) {
        for (const QString &g : m_groups) {
            if (g.contains(q, cs)) {
                ui->listGroups->addItem(g);
                ++groupCount;
            }
        }
    }

    if (scope == 0 || scope == 3) {
        for (const QString &m : m_messages) {
            if (m.contains(q, cs)) {
                ui->listMessages->addItem(m);
                ++messageCount;
            }
        }
    }

    updateStatus(userCount, groupCount, messageCount);
}

void searchDialog::updateStatus(int users, int groups, int messages)
{
    const QString q = ui->txtQuery->text().trimmed();
    if (q.isEmpty()) {
        ui->lblStatus->setText("请输入关键字进行搜索");
        return;
    }
    ui->lblStatus->setText(QString("关键字：%1  联系人：%2  群组：%3  消息：%4").arg(q).arg(users).arg(groups).arg(messages));
}
