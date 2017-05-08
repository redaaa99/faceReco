#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QPixmap pix("C:/Users/p/Desktop/avatar.png");
    int w = ui->label->width();
    int h = ui->label->height();
    ui->label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    ui->label_2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    ui->label_3->setText("Nom : image.png");
    ui->label_4->setText("Correspond à :");

}

MainWindow::~MainWindow()
{
    delete ui;
}
