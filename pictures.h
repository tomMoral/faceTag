#ifndef PICTURES_H
#define PICTURES_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Pictures : public QGraphicsPixmapItem
{
    QStringList labels;
    QList<bool> values;
    qint64 id;
    int checked;
    QGraphicsItem* state;

    static QString dir_db;

protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );

public:
    Pictures(const QPixmap & pixmap, qint64 id,
             QStringList labs = QStringList());

    static void path(QString dir_db);
    int getState(){return checked;}
    int getId(){return id;}

public slots:
    void add_labels(QString labels);
    void save();
};

#endif // PICTURES_H
