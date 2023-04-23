#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createModel();
    createView();
    createLayout();
}

MainWindow::~MainWindow()
{
    delete tableView;
    delete model;
    delete proxyModel;
    delete adminCheckBox;
    delete searchLineEdit;
    delete salaryDelegate;
    delete ui;
}

void MainWindow::createModel()
{
    model = new QStandardItemModel(this);
    model->setColumnCount(6);

    model->setHeaderData(0, Qt::Horizontal, "Sicil No");
    model->setHeaderData(1, Qt::Horizontal, "İsim");
    model->setHeaderData(2, Qt::Horizontal, "Soyisim");
    model->setHeaderData(3, Qt::Horizontal, "Teknik Personel");
    model->setHeaderData(4, Qt::Horizontal, "Maaş");
    model->setHeaderData(5, Qt::Horizontal, "Departman");

    QList<Personel> personelList = {
        {1001, "Ahmet", "Yılmaz", false, 3000, "Muhasebe"},
        {1002, "Ayşe", "Demir", true, 1500, "Teknik"},
        {1003, "Mehmet", "Çelik", false, 4500, "Pazarlama"},
        {1004, "Ali", "Şahin", true, 1300, "IT"},
        {1005, "Fatma", "Kara", false, 5000, "İnsan Kaynakları"}
    };

    for (const auto &personel : personelList) {
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(QString::number(personel.sicilNo)));
        rowItems.append(new QStandardItem(personel.isim));
        rowItems.append(new QStandardItem(personel.soyisim));
        rowItems.append(new QStandardItem(personel.teknikPersonel ? "Evet" : "Hayır"));
        rowItems.append(new QStandardItem(QString::number(personel.maas)));
        rowItems.append(new QStandardItem(personel.departman));
        model->appendRow(rowItems);
    }
}

void MainWindow::createView()
{
    proxyModel = new MyFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    tableView = new QTableView(this);
    tableView->setModel(proxyModel);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableView->setSortingEnabled(true);

    salaryDelegate = new SalaryDelegate();
    tableView->setItemDelegateForColumn(4, salaryDelegate);

    setCentralWidget(tableView);
}

void MainWindow::createLayout()
{
    searchLineEdit = new QLineEdit(this);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterTextChanged);

    adminCheckBox = new QCheckBox("Teknik Personeli Göster", this);
    adminCheckBox->setChecked(true);
    connect(adminCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleAdminPersonnel);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(adminCheckBox);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(tableView);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::filterTextChanged(const QString& filterText)
{
    proxyModel->setFilterText(filterText);
}

void MainWindow::toggleAdminPersonnel(bool checked)
{
    if (checked) {
        proxyModel->setShowTechnicalPersonnel(true);
    } else {
        proxyModel->setShowTechnicalPersonnel(false);
    }
}

void MyFilterProxyModel::setShowTechnicalPersonnel(bool show)
{
    if (m_showTechnicalPersonnel != show) {
        m_showTechnicalPersonnel = show;
        invalidateFilter();
    }
}

void MyFilterProxyModel::setFilterText(const QString &text)
{
    if (m_filterText != text) {
        m_filterText = text;
        invalidateFilter();
    }
}

bool MyFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Arama kriteri ile satırı karşılaştır
    bool match = false;
    for (int i = 0; i < sourceModel()->columnCount(); ++i) {
        QModelIndex index = sourceModel()->index(source_row, i, source_parent);
        QString data = sourceModel()->data(index, Qt::DisplayRole).toString();
        if (data.contains(m_filterText, Qt::CaseInsensitive)) {
            match = true;
            break;
        }
    }
    if (!match)
        return false;

    // Teknik personel gösterimini kontrol et
    QModelIndex index = sourceModel()->index(source_row, 3, source_parent); // 3: teknikPersonel sütununun indeksi
    bool technicalPersonnel = sourceModel()->data(index, Qt::DisplayRole) == "Evet" ? true : false;
    if (!m_showTechnicalPersonnel && technicalPersonnel)
        return false;

    return true;
}


void SalaryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == 4) { // 4: maaş sütununun indeksi
        int salary = index.data(Qt::DisplayRole).toInt();
        if (salary < 2000) {
            painter->fillRect(option.rect, Qt::red);
        }
        painter->drawText(option.rect, Qt::AlignVCenter | Qt::AlignHCenter, QString::number(salary));
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

