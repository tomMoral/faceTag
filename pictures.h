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

    static QString dir_db;

protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );

public:
    Pictures(const QPixmap & pixmap,
             QStringList labs = QStringList(), qint64 line = -1);

    static void path(QString dir_db);

public slots:
    void add_labels(QString labels);
    void save();
};

#endif // PICTURES_H
