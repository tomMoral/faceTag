#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

public slots:
    void on_addFolderButton_clicked();
    void on_addLabelButton_clicked();
    
private:
    Ui::MainWindow *ui;
    QList<Pictures*> pics;
    QGraphicsScene* scene;
    QPointF nextPos;
};

#endif // MAINWINDOW_H
