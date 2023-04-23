
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
        {1002, "Ayşe", "Demir", true, 4500, "Teknik"},
        {1003, "Mehmet", "Çelik", false, 1500, "Pazarlama"},
        {1004, "Fatma", "Kara", true, 5000, "İnsan Kaynakları"},
         {1005, "Ali", "Şahin", false, 1300, "IT"}};

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

    // Yeni bir QTableView nesnesi oluşturulur ve model ile bağlanır
    tableView = new QTableView(this);
    tableView->setModel(proxyModel);

    // Tablo başlıklarının otomatik olarak boyutlandırılması sağlanır
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Tabloda sıralama etkinleştirilir
    tableView->setSortingEnabled(true);

    // SalaryDelegate nesnesi, maaş sütununu kırmızıya boyamak için kullanılacaktır
    SalaryDelegate *salaryDelegate = new SalaryDelegate();
    tableView->setItemDelegateForColumn(4, salaryDelegate);

    // Tabloyu ana pencereye eklenir
    setCentralWidget(tableView);
}

void MainWindow::createLayout()
{
    // Create search box and connect it to slot
    searchLineEdit = new QLineEdit(this);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterTextChanged);

    // Create toggle button to show/hide administrative personnel
    adminCheckBox = new QCheckBox("Show administrative personnel", this);
    adminCheckBox->setChecked(true);
    connect(adminCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleAdminPersonnel);

    // Create layout to hold search box and toggle button horizontally
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(adminCheckBox);

    // Create main layout and add search and table view
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(tableView);

    // Set main layout for main window
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
    // Show all personnel
    proxyModel->setShowTechnicalPersonnel(true);
    } else {
    // Hide administrative personnel
    proxyModel->setShowTechnicalPersonnel(false);
    }
}

