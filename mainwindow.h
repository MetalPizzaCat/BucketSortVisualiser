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
    Ui::MainWindow *ui;

    std::vector<QString> sort(std::vector<QString> &arr, int start, int end);
};
#endif // MAINWINDOW_H
