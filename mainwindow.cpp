#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <array>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::pressed, this, &MainWindow::beginSort);
}

int MainWindow::getChatAt(QString const &str, int id) const
{
    return str.size() > id ? str[id].unicode() : -1;
}

std::vector<QString> MainWindow::sort(std::vector<QString> &array, int start, int end)
{
    std::vector<QString> result;
    std::array<std::vector<QString>, 256> buckets;
    if (start >= end)
    {
        return array;
    }
    for (QString const &str : array)
    {
        buckets[getChatAt(str, start) + 1].push_back(str);
    }
    for (std::vector<QString> &bucket : buckets)
    {
        if (bucket.size() > 1)
        {
            std::vector<QString> res = sort(bucket, start + 1, end);
            result.insert(result.end(), res.begin(), res.end());
        }
        else
        {
            result.insert(result.end(), bucket.begin(), bucket.end());
        }
    }
    bucketTree[start].push_back(result);
    return result;
}

void MainWindow::beginSort()
{

    QString text = ui->plainTextEdit->toPlainText();
    QStringList words = text.split(QRegExp("[,|;| ]"));
    words.replaceInStrings(QRegExp(" +"), "");
    ui->resultLabel->clear();
    std::vector<QString> array = words.toVector().toStdVector();
    array = sort(array, 0, ui->spinBox->value());
    for (QString const &str : array)
    {
        ui->resultLabel->setText(ui->resultLabel->text() + str + ", ");
    }
    QStandardItemModel *model = new QStandardItemModel();
    for (auto const &[layer, buckets] : bucketTree)
    {
        QStandardItem *layerItem = new QStandardItem(QString::number(layer));
        for (std::vector<QString> const &bucket : buckets)
        {
            QChar ch = bucket[0][(int)layer];
            QStandardItem *bucketItem = new QStandardItem(ch);
            for (QString const &str : bucket)
            {
                bucketItem->appendRow(new QStandardItem(str));
            }
            layerItem->appendRow(bucketItem);
        }
        model->appendRow(layerItem);
    }
    ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// uwu,ahooha,ahhoga,ahooga,bazinga,eheh