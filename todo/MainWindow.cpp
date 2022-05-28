#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mTasks()
{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked,
            this, &MainWindow::addTask);
    updateStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTask() {
    bool ok;
    QString name = QInputDialog::getText(this,
    tr("Add task"),
    tr("Task name"),
    QLineEdit::Normal,
    tr("untitled task"), &ok);
    if (ok && !name.isEmpty()) {
        qDebug()<<"new Task";
        Task* task = new Task(name);
        connect(task, &Task::removed,
                this, &MainWindow::removeTask);
        mTasks.append(task);
        ui->tasksLayout->addWidget(task);
        updateStatus();
    }
}

void MainWindow::removeTask(Task *task) {
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    delete task;
    updateStatus();
}

void MainWindow::taskStatusChanged(Task *) {
    updateStatus();
}

void MainWindow::updateStatus() {
    int doneCount = 0;
    for (auto task : mTasks) {
        doneCount += task->isCompleted();
    }
    int left = mTasks.size() - doneCount;
    ui->statusLabel->setText(QString("Status %1 todo / %2 completed")
                             .arg(left)
                             .arg(doneCount));
}

