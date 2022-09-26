#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void beginSort();

private:
    int getChatAt(QString const &, int) const;

    std::map<size_t, std::vector<std::vector<QString>>> bucketTree;
    std::map<size_t, QString> depthString;
    Ui::MainWindow *ui;

    void sort(std::vector<QString> &arr, int start, int end, int depth, int maxDepth);
};
#endif // MAINWINDOW_H
