#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void CreateOpenFileTool();
    void CreateOpenGlWidget(const QString& nm);

private slots:
    void onToolClicked();


private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
