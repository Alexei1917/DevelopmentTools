#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct ToolInfo{
    QString name;
    QString command;
    QStringList args;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void checkTools();
    QString runCommand(const QString &cmd, const QStringList &args);

private:
    Ui::MainWindow *ui;
    QList<ToolInfo> tools;
};
#endif // MAINWINDOW_H
