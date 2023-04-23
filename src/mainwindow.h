#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QPainter>

struct Personel {
    int sicilNo;
    QString isim;
    QString soyisim;
    bool teknikPersonel;
    int maas;
    QString departman;
};

class MyFilterProxyModel : public QSortFilterProxyModel {
public:
    MyFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
        , m_showTechnicalPersonnel(true)
    {}

    void setShowTechnicalPersonnel(bool show);

    void setFilterText(const QString &text);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
private:
    bool m_showTechnicalPersonnel;
    QString m_filterText;
};

class SalaryDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTableView *tableView;
    QStandardItemModel *model;
    MyFilterProxyModel *proxyModel;
    QCheckBox *adminCheckBox;
    QLineEdit *searchLineEdit;
    SalaryDelegate * salaryDelegate;

    void createModel();
    void createView();
    void createLayout();
    void filterTextChanged(const QString& filterText);
    void toggleAdminPersonnel(bool checked);
};
#endif // MAINWINDOW_H
