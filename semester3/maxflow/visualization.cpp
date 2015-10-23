#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QRectF>
#include <QBrush>
#include <Qt>
#include "graph.h"

const qreal VERTEX_RADIUS = 5;
const int HEIGHT_MULTIPLIER = 1;

class VisualVertex : public QGraphicsItem {
public:
    Vertex *wrapped_vertex;

    VisualVertex(QGraphicsItem *parent, Vertex *wrapped_vertex)
        : QGraphicsItem(parent),
          wrapped_vertex(wrapped_vertex) {
          setX(50);
          setY(50);
        }

    int height() const {
        return wrapped_vertex->height * HEIGHT_MULTIPLIER;
    }

    QPointF shadow_shift() const {
        return QPointF(height(), height());
    }

    QRectF main_rect() const {
        return QRectF(-VERTEX_RADIUS, -VERTEX_RADIUS, 2*VERTEX_RADIUS, 2*VERTEX_RADIUS);
    }

    virtual void paint(QPainter *painter,
            const QStyleOptionGraphicsItem*,
            QWidget*) {
        painter->setBrush(Qt::red);
        painter->drawEllipse(main_rect());
        painter->setBrush(Qt::gray);
        painter->drawEllipse(main_rect().translated(shadow_shift()));
    }

    virtual QRectF boundingRect() const {
        return main_rect() | main_rect().translated(shadow_shift());
    }
};
