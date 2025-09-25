#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化要检测的工具
    tools = {
        {"Git", "git", {"--version"}},
        {"Python", "python", {"--version"}},
        {"Node.js", "node", {"--version"}},
        {"CMake", "camke", {"--version"}},
        {"MSVC cl", "cl", {"--version"}},
    };
    // 初始化界面
    ui->tableWidget->setColumnCount(3);
    QStringList headers = {"工具名称", "状态", "版本"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(tools.size());

    connect(ui->buttonRefresh, &QPushButton::clicked, this, &MainWindow::checkTools);
    // 启动一次检测一次
    checkTools();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkTools()
{
    for (int i = 0; i < tools.size(); i++) {
        // 遍历 tools 列表（tools 是 QList<ToolInfo>）。tools.size() 返回元素数（int）。
        // 用常量引用读取第 i 个元素，避免拷贝。tool 是对原列表元素的引用（不能修改），效率更高。
        const ToolInfo &tool = tools[i];

        //调用 runCommand（你在别处实现的函数），传入可执行命令和参数列表，返回字符串（通常是命令输出或错误输出）。这是同步调用（会等待 runCommand 返回）。
        QString output = runCommand(tool.command, tool.args);

        // 在表格第 i 行第 0 列放入一个 QTableWidgetItem（显示工具名称）。注意：QTableWidget 会接管 QTableWidgetItem* 的内存管理（会负责删除）。
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tool.name));

        if (output.isEmpty()) {
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("未安装"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem("-"));
        } else {
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("已安装"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(output.trimmed()));
        }
    }
}

QString MainWindow::runCommand(const QString &cmd, const QStringList &args)
{
    // 在栈上创建一个 QProcess 临时对象，用来启动进程并捕获其输出。
    QProcess process;

    // 启动外部程序：cmd 是程序名（例如 "git"），args 是命令行参数（例如 {"--version"}）。QProcess 会在系统 PATH 中查找可执行文件。
    process.start(cmd, args);

    // 阻塞等待进程执行完成，最多等待 1500 毫秒（1.5 秒）。如果超时（或失败），函数返回空字符串。注意：这里是同步阻塞调用，会阻塞调用线程（通常是 UI 线程）直到超时或完成。
    if (!process.waitForFinished(1500)) {
        return "";
    }

    // 读取进程标准输出和标准错误输出。原代码将两者分别读出，再按顺序选择返回：优先返回 stdout，不然返回 stderr。
    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    if (!output.isEmpty()) {
        return output;
    } else if (!error.isEmpty()) {
        return error;
    }
    return "";
}
