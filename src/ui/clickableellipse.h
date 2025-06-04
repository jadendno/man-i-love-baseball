#pragma once

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class ClickableEllipse : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    ClickableEllipse(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr)
        : QObject(), QGraphicsEllipseItem(x, y, w, h, parent)
    {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setZValue(1);   // makes sure it’s above anything at z=0
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton) {
            qDebug() << "CLICKED ellipse at" << event->scenePos();
            emit clicked();
        }
        QGraphicsEllipseItem::mousePressEvent(event);
    }
};
