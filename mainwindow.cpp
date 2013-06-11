#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QDebug>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scene = new QGraphicsScene();
    this->ui->graphicsView->setScene(this->scene);
    this->nextPos = QPointF(0,0);

    QFile labels("/Users/tom/database/DB_labels_file.db");
    labels.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&labels);
    QString str;
    in >> str;
    while( !in.atEnd()){
        qDebug() << str;
        this->ui->comboBox->addItem(str);
        in >> str;
    }

    QFile nameDB("/Users/tom/database/DB_contents.db");
    nameDB.open(QIODevice::ReadOnly|QIODevice::Text);
    in.setDevice(&nameDB);
    in >> str;
    while( !in.atEnd()){
        Pictures* current_pics = new Pictures(QPixmap(str).scaled(150,150,Qt::KeepAspectRatio));
        current_pics->setPos(nextPos);
        this->pics << current_pics;
        this->scene->addItem(current_pics);
        nextPos += QPointF(150,0);
        if (nextPos.x() > 750){
            nextPos.setX(0);
            nextPos += QPointF(0,150);
        }
        in >> str;
    }
}

MainWindow::~MainWindow()
{
    for(int i =0; i < this->pics.size() ; i++)
        this->pics.removeFirst();
    delete scene;
    delete ui;
}

void MainWindow::on_addFolderButton_clicked(){
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            "/Users/tom/Pictures/TestCluster/",
                            "Images (*.png *.xpm *.jpg)");
    QFile nameDB("/Users/tom/database/DB_contents.db");
    nameDB.open(QIODevice::Append|QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&nameDB);
    for(int i = 0 ; i < files.size(); i ++){
        out << files[i] << "\n";
        Pictures* current_pics = new Pictures(QPixmap(files[i]).scaled(150,150,Qt::KeepAspectRatio));
        current_pics->setPos(nextPos);
        this->pics << current_pics;
        this->scene->addItem(current_pics);
        nextPos += QPointF(150,0);
        if (nextPos.x() > 750){
            nextPos.setX(0);
            nextPos += QPointF(0,150);
        }
    }
}

void MainWindow::on_addLabelButton_clicked(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Label"), tr("Label:"), QLineEdit::Normal,QString(), &ok);
    bool exists = false;
    for(int i = 0 ; i < this->ui->comboBox->count(); i++)
        exists = exists || (this->ui->comboBox->itemText(i) == text);
    if(exists || !ok)
        return;
    this->ui->comboBox->addItem(text);
    QFile labels("/Users/tom/database/DB_labels_file.db");
    labels.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
    QTextStream out(&labels);
    out << text << '\n';
}

void MainWindow::on_saveButton_clicked(){
    for(int i = 0; i < this->pics.size(); i++)
        pics[i]->add_labels(ui->comboBox->currentIndex());
}