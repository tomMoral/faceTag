#include "pictures.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

Pictures::Pictures(const QPixmap & pixmap, QStringList labs, qint64 line):
    QGraphicsPixmapItem(pixmap)
{
    this->checked = 0;
    this->state = NULL;
    this->labels = labs;
    this->lineSave = line;
}

void Pictures::mousePressEvent ( QGraphicsSceneMouseEvent * event ){
    if(checked != 0){
        checked = 0;
        delete this->state;
        this->state = NULL;
        QGraphicsPixmapItem::mousePressEvent(event);
        return;
    }
    if(event->button() == Qt::LeftButton){
        checked = 1;
        this->state = new QGraphicsPixmapItem(QPixmap("/Users/tom/prog/faceTag/check.png").scaled(150, 150),this);
        this->state->setZValue(10);
    }
    else{
        checked = 2;
        this->state = new QGraphicsPixmapItem(QPixmap("/Users/tom/prog/faceTag/cross.gif").scaled(150, 150),this);
    }
}

void Pictures::add_labels(QString label){
    if(this->checked !=0){
        this->labels << label;
        this->values << (this->checked == 1);
    }
}
