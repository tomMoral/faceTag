#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scene = new QGraphicsScene();
    this->ui->graphicsView->setScene(this->scene);
    this->nextPos = QPointF(0,0);


    QFile conf(".ft.config");
    if(!conf.open(QIODevice::ReadOnly|QIODevice::Text)){
        conf.open(QIODevice::ReadWrite|QIODevice::Text);
        db_dir = QFileDialog::getExistingDirectory(this,"Database Location");
        QTextStream(&conf) << db_dir;
    }
    else{
        db_dir = conf.readLine();
        db_dir.remove('\n');
    }
    Pictures::path(db_dir);

    db = QSqlDatabase::addDatabase("QSQLITE"); // becomes the new default connection
    QString dbPath = db_dir + "/FaceLabels.db";
    db.setDatabaseName(dbPath);
    if(!db.open()){
        qDebug() << "Database issue";
    }
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS DB_contents (id NUMBER, path TEXT, labels TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS DB_labels (id NUMBER, label VARCHAR(255))");


    query.prepare("SELECT path, id FROM DB_contents ");
    if(!query.exec())
        qDebug() << query.lastError();


    while(query.next()){
        QString str = query.value(0).toString();
        qint64 id = query.value(1).toInt();
        this->pics << new Pictures(QPixmap(str).scaled(150,150,Qt::KeepAspectRatio), id);
    }

    ui->comboBox->addItem("Remove");
    query.exec("SELECT * FROM DB_labels SORT BY id");
    while(query.next()){
        QString str = query.value(1).toString();
        this->ui->comboBox->addItem(str);
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

    QSqlQuery query(db);
    for(int i = 0 ; i < files.size(); i ++){
        query.prepare("SELECT path FROM DB_contents WHERE path = :path");
        query.bindValue(":path", files[i]);
        query.exec();
        if(!query.next()){
            query.prepare("INSERT INTO DB_contents (id, path, labels) VALUES (:id, :path, '');");
            query.bindValue(":id", pics.count());
            query.bindValue(":path", files[i]);
            if(!query.exec())
                qDebug() << query.lastError();


            long int id =  query.lastInsertId().toLongLong();
            Pictures* current_pics = new Pictures(QPixmap(files[i]).scaled(150,150,Qt::KeepAspectRatio),id);
            current_pics->setPos(nextPos);
            this->pics << current_pics;
            this->scene->addItem(current_pics);
            if (nextPos.x()+150 > 750){
                nextPos.setX(0);
                nextPos += QPointF(0,150);
            }
            else
                nextPos += QPointF(150,0);
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
    QSqlQuery query(db);
    query.prepare("INSERT INTO DB_labels (id, label) VALUES (:id, :lab)");
    query.bindValue(":id", ui->comboBox->count());
    query.bindValue(":lab", text);
    if(!query.exec())
        qDebug() << query.lastError();

    this->ui->comboBox->addItem(text);
}

void MainWindow::on_saveButton_clicked(){
    QSqlQuery query(db);
    int lab_id = ui->comboBox->currentIndex();
    if (lab_id == 0) return;
    for(int i = 0; i < this->pics.size(); i++){
        int state = pics[i]->getState();
        if(state != 0){
            query.prepare("UPDATE DB_contents SET labels = CASE labels WHEN ' ' THEN :lab ELSE labels ||  ',' || :lab1 END WHERE rowid = :id;");
            query.bindValue(":id", pics[i]->getId());
            QString lab = QString::number(lab_id) + ':' + ((state==1)?"1":"0");
            qDebug() << lab << pics[i]->getId();
            query.bindValue(":lab", lab);
            query.bindValue(":lab1", lab);
            if(!query.exec())
                qDebug() << query.lastError() << query.executedQuery();
        }
    }
    display(lab_id);
}


void MainWindow::on_removeButton_clicked(){
    QSqlQuery query(db);
    int lab_id = ui->comboBox->currentIndex();
    if (lab_id != 0) return;
    for(int i = 0; i < this->pics.size(); i++){
        int state = pics[i]->getState();
        if(state == 2){
            query.prepare("DELETE FROM DB_contents WHERE rowid = :id;");
            query.bindValue(":id", pics[i]->getId());
            if(!query.exec())
                qDebug() << query.lastError() << query.executedQuery();
        }
    }
    display(lab_id);
}


void MainWindow::display(int lab_id){
    for(int i = 0; i < pics_displayed.size(); i++){
        this->scene->removeItem(this->pics_displayed[i]);
    }
    while(pics_displayed.size()>0)
        pics_displayed.removeFirst();

    QSqlQuery query(db);
    query.prepare("SELECT id FROM DB_contents WHERE labels NOT LIKE :reg ");
    QString re = "%";
    re += QString::number(lab_id) +":_%";
    query.bindValue(":reg",  re);
    if(!query.exec())
        qDebug() << query.lastError();

    nextPos = QPointF(0,0);

    while(query.next()){
        qint64 id = query.value(0).toInt();
        Pictures* current_pics = pics[id];
        current_pics->setPos(nextPos);
        this->pics_displayed << current_pics;
        this->scene->addItem(current_pics);
        nextPos += QPointF(150,0);
        if (nextPos.x() >= 750){
            nextPos.setX(0);
            nextPos += QPointF(0,150);
        }
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int lab_id){
    display(lab_id);
}
