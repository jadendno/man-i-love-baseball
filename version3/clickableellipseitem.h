#ifndef CLICKABLEELLIPSEITEM_H
#define CLICKABLEELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QString>

class ClickableEllipseItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    ClickableEllipseItem(const QString& stadiumName, QGraphicsItem* parent = nullptr)
        : QGraphicsEllipseItem(parent), stadiumName(stadiumName) {}

signals:
    void clicked(const QString& stadiumName);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        emit clicked(stadiumName);
        QGraphicsEllipseItem::mousePressEvent(event);
    }

private:
    QString stadiumName;
};

#endif // CLICKABLEELLIPSEITEM_H
