#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <array>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <algorithm>

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

void MainWindow::sort(std::vector<QString> &array, int start, int end, int depth)
{
    std::array<std::vector<QString>, 256> buckets;
    if (end <= start || depth > end)
    {
        return;
    }
    for (int i = start; i < std::min(end, (int)array.size()); i++)
    {
        buckets[getChatAt(array[i], depth) + 1].push_back(array[i]);
    }

    int i = start;
    for (std::vector<QString> const &bucket : buckets)
    {
        if (!bucket.empty())
        {
            for (QString const &str : bucket)
            {
                array[i] = str;
                i++;
            }
        }
    }
    for (QString const &str : array)
    {
        qInfo() << str;
    }
    qInfo() << "\n";
    int sortOffset = start;

    bucketTree[depth] = std::vector<std::vector<QString>>();
    bucketTree[depth].push_back(array);
    for (int r = 0; r < 255; r++)
    {
        if (!buckets[r].empty())
        {
            //qInfo() << "Sorting from " << sortOffset << " to " << sortOffset + (int)buckets[r].size() << " at " << depth + 1;
            sort(array, sortOffset, sortOffset + (int)buckets[r].size(), depth + 1);
            sortOffset += buckets[r].size();
        }
    }
}

void MainWindow::beginSort()
{

    QString text = ui->plainTextEdit->toPlainText();
    QStringList words = text.split(QRegExp("[,|;| ]"));
    words.replaceInStrings(QRegExp(" +"), "");
    ui->resultLabel->clear();
    std::vector<QString> array = words.toVector().toStdVector();
    sort(array, 0, ui->spinBox->value(), 0);
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
            QStandardItem *bucketItem = new QStandardItem("ello");
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
// uwu,ahcoha,ahboga,ahaoga,bazinga,eheh