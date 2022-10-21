#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <array>
#include <QStringList>
#include <QDebug>
#include <algorithm>
#include <QFile>
#include <map>
#include <QTextStream>

#define DISPLAY_VISUALIZATION_TREE
#define WRITE_TO_FILE
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

void MainWindow::sort(std::vector<QString> &array, int start, int end, int depth, int maxDepth)
{
    std::map<int, std::vector<QString>> buckets;
    if (end <= start || depth > maxDepth || end - start < 2)
    {
        return;
    }
    for (int i = start; i < std::min(end, (int)array.size()); i++)
    {
        buckets[getChatAt(array[i], depth) + 1].push_back(array[i]);
    }

    int i = start;
    for (auto &[id, bucket] : buckets)
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
    int sortOffset = start;
#ifdef DISPLAY_VISUALIZATION_TREE
    for (QString const &str : array)
    {
        depthString[depth] += str + ", ";
    }

    for (auto const &[id, bucket] : buckets)
    {
        if (!bucket.empty())
        {
            bucketTree[depth].push_back(bucket);
        }
    }
#endif
    for (auto const &[id, bucket] : buckets)
    {
        if (id == 0)//ignore this one
        {
            continue;
        }
        if (!bucket.empty())
        {
            sort(array, sortOffset, sortOffset + (int)bucket.size(), depth + 1, maxDepth);
            sortOffset += bucket.size();
        }
    }
}

void MainWindow::beginSort()
{
#ifdef DISPLAY_VISUALIZATION_TREE
    if (treeModel)
    {
        for (int i = 0; i < treeElements.size(); i++)
        {
            delete treeElements[i];
        }
        delete treeModel;
        treeElements.clear();
        bucketTree.clear();
    }
#endif
    QString text = ui->plainTextEdit->toPlainText();
    QStringList words = text.split(QRegExp("[,|;| ]"));
    words.replaceInStrings(QRegExp(" +"), "");
    ui->resultLabel->clear();
    std::vector<QString> array = words.toVector().toStdVector();
    sort(array, 0, (int)array.size(), 0, ui->spinBox->value() - 1);

    for (QString const &str : array)
    {
        ui->resultLabel->setText(ui->resultLabel->text() + str + ", ");
    }

#ifdef DISPLAY_VISUALIZATION_TREE
    treeModel = new QStandardItemModel();
    for (auto const &[layer, buckets] : bucketTree)
    {
        QStandardItem *layerItem = new QStandardItem(QString::number(layer) + ": " + depthString[layer]);
        treeElements.push_back(layerItem);
        int bucketCount = 0;
        for (std::vector<QString> const &bucket : buckets)
        {
            QStandardItem *bucketItem = new QStandardItem(QString::number(bucketCount++));
            for (QString const &str : bucket)
            {
                bucketItem->appendRow(new QStandardItem(str));
            }
            layerItem->appendRow(bucketItem);
        }
        treeModel->appendRow(layerItem);
    }
    ui->treeView->setModel(treeModel);
#endif
#ifdef WRITE_TO_FILE
    QFile file("./out.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ui->resultLabel->text();
    }
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}
// ahcoha,ahboga,ahaoga,bazinga,eheh