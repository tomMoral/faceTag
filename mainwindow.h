#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "pictures.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void display(int lab_id);

public slots:
    void on_addFolderButton_clicked();
    void on_addLabelButton_clicked();
    void on_saveButton_clicked();
    void on_comboBox_currentIndexChanged(int lab_id);
    void on_removeButton_clicked();
    void on_getButton_clicked();
    
private:
    Ui::MainWindow *ui;
    QList<Pictures*> pics, pics_displayed;
    QGraphicsScene* scene;
    QPointF nextPos;
    QString db_dir;
    QSqlDatabase db;
    QStringList labels;
};

#endif // MAINWINDOW_H
