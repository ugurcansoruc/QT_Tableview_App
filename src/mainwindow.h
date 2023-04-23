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

    void setShowTechnicalPersonnel(bool show)
    {
        if (m_showTechnicalPersonnel != show) {
            m_showTechnicalPersonnel = show;
            invalidateFilter();
        }
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        QModelIndex index = sourceModel()->index(source_row, 3, source_parent); // 3: teknikPersonel sütununun indeksi

        bool technicalPersonnel = sourceModel()->data(index, Qt::DisplayRole) == "Evet" ? true : false;

        if (m_showTechnicalPersonnel) {
            return true; // tüm teknik personeli göster
        } else {
            return !technicalPersonnel; // sadece teknik personel olmayanları göster
        }
    }
private:
    bool m_showTechnicalPersonnel;
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

    void createModel();
    void createView();
    void createLayout();
    void filterTextChanged(const QString& filterText);
    void toggleAdminPersonnel(bool checked);
};

class SalaryDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
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
};

#endif // MAINWINDOW_H
