#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <Qt>
#include "graph.h"

const qreal VERTEX_RADIUS = 5;
const int HEIGHT_MULTIPLIER = 1;

class ShadowedGraphicsItem;
class Shadow : public QGraphicsItem {
public:
    ShadowedGraphicsItem* owner = 0;

    Shadow(ShadowedGraphicsItem *owner) : owner(owner) {}
    void updatePos();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
            const QStyleOptionGraphicsItem*,
            QWidget*);
};

class ShadowedGraphicsItem : public QGraphicsItem {
    Shadow shadow;
public:
    ShadowedGraphicsItem() : shadow{this} {}
    QGraphicsItem* getShadow() {
        return &shadow;
    };

    virtual QRectF shadowBoundingRect() const = 0;
    virtual void paintShadow(QPainter *painter) = 0;
};

void Shadow::updatePos() {
    setPos(owner->pos());
}

QRectF Shadow::boundingRect() const {
    const_cast<Shadow*>(this)->updatePos();
    return owner->shadowBoundingRect();
}

void Shadow::paint(QPainter *painter,
        const QStyleOptionGraphicsItem*,
        QWidget*) {
    updatePos();
    owner->paintShadow(painter);
}

class GraphScene : public QGraphicsScene {
    QGraphicsWidget radiant_layer;
    QGraphicsWidget shadow_layer;

public:
    GraphScene() {
        addItem(&shadow_layer);
        addItem(&radiant_layer);
    }

    void addShadowedItem(ShadowedGraphicsItem* item) {
        item->setParentItem(&radiant_layer);
        QGraphicsItem* shadow = item->getShadow();
        shadow->setParentItem(&shadow_layer);
        addItem(item);
        addItem(shadow);
    }
};


class VisualVertex : public ShadowedGraphicsItem {
public:
    Vertex *wrapped_vertex;

    VisualVertex(Vertex *wrapped_vertex) :
            wrapped_vertex(wrapped_vertex) {
        setX(50);
        setY(50);
    }

    int height() const {
        return wrapped_vertex->height * HEIGHT_MULTIPLIER;
    }

    QPointF shadowShift() const {
        return QPointF(height(), height());
    }

    virtual void paint(QPainter *painter,
            const QStyleOptionGraphicsItem*,
            QWidget*) {
        painter->setBrush(Qt::red);
        painter->drawEllipse(boundingRect());
    }

    virtual void paintShadow(QPainter *painter) {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(shadowBoundingRect());
    }

    virtual QRectF boundingRect() const {
        return QRectF(-VERTEX_RADIUS, -VERTEX_RADIUS, 2*VERTEX_RADIUS, 2*VERTEX_RADIUS);
    }

    virtual QRectF shadowBoundingRect() const {
        return boundingRect().translated(shadowShift());
    }
};
