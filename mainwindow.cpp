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

std::vector<QString> MainWindow::sort(std::vector<QString> &array, int start, int end)
{
    std::vector<QString> result;
    std::array<std::vector<QString>, 255> buckets;
    if (start >= end)
    {
        return array;
    }
    for (QString const &str : array)
    {
        buckets[str.at(start).unicode()].push_back(str);
    }
    for (std::vector<QString> &bucket : buckets)
    {
        if (bucket.size() > 1)
        {
            auto res = sort(bucket, start + 1, end);
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
    QStringList words = text.split(",");
    ui->resultLabel->clear();
    std::vector<QString> array = words.toVector().toStdVector();
    array = sort(array, 0, ui->spinBox->value());
    for (QString const &str : array)
    {
        ui->resultLabel->setText(ui->resultLabel->text() + str + ", ");
    }

    QStandardItemModel *model = new QStandardItemModel();
    int layer = 0;
    for (auto const &layers : bucketTree)
    {
        QStandardItem *layerItem = new QStandardItem(QString::number(layer++));
        for (auto const &bucket : layers)
        {
            QString layerStr;
            for (QString const &str : bucket)
            {
                layerStr += str + ", ";
            }
            layerItem->appendRow(new QStandardItem(layerStr));
        }
        model->appendRow(layerItem);
    }

    ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
