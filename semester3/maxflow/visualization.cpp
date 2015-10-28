#include <QRectF>
#include <QBrush>
#include <QPainter>
#include <Qt>
#include <QWidget>
#include "graph.h"

const int VERTEX_RADIUS = 5;
const int HEIGHT_MULTIPLIER = 1;

class ShadowedGraphicsElement {
public:
    virtual void paint(QPainter* painter) = 0;
    virtual void paintShadow(QPainter* painter) = 0;
};

class ShadedCanvas : public QWidget {
    std::vector<ShadowedGraphicsElement*> elements;

    public:
    virtual void paintEvent(QPaintEvent*) {
        QPainter painter(this);
        for (auto element : elements) {
            element->paintShadow(&painter);
        }
        for (auto element : elements) {
            element->paint(&painter);
        }
    }

    void addElement(ShadowedGraphicsElement* element) {
        elements.push_back(element);
    }
};

class VisualVertex : public ShadowedGraphicsElement {
public:
    QPoint position;
    Vertex* wrappedVertex;

public:
    VisualVertex(Vertex* vertex) : wrappedVertex(vertex) {};

    QPoint getPosition() {
        return position;
    }

    QPoint getShadowPosition() {
        return position + QPoint(height(), height());
    }

    int height() {
        return wrappedVertex->height*HEIGHT_MULTIPLIER;
    }

    virtual void paint(QPainter* painter) {
        painter->setBrush(Qt::red);
        painter->drawEllipse(position, VERTEX_RADIUS, VERTEX_RADIUS);
    }

    virtual void paintShadow(QPainter* painter) {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(getShadowPosition(), VERTEX_RADIUS, VERTEX_RADIUS);
    }
};

class VisualEdge : public ShadowedGraphicsElement {
public:
    VisualVertex* tail;
    VisualVertex* head;

public:
    virtual void paint(QPainter* painter) {
        painter->setPen(Qt::black);
        painter->drawLine(tail->getPosition(), head->getPosition());
    }

    virtual void paintShadow(QPainter* painter) {
        painter->setPen(Qt::gray);
        painter->drawLine(tail->getShadowPosition(), head->getShadowPosition());
    }
};

