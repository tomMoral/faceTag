#ifndef PICTURES_H
#define PICTURES_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Pictures : public QGraphicsPixmapItem
{
    QStringList labels;
    QList<bool> values;
    qint64 lineSave;
    int checked;
    QGraphicsItem* state;

protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );

public:
    Pictures(const QPixmap & pixmap,
             QStringList labs = QStringList(), qint64 line = -1);

public slots:
    void add_labels(QString labels);
    void save();
};

#endif // PICTURES_H
