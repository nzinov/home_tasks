#include <QRectF>
#include <QBrush>
#include <QPainter>
#include <QFont>
#include <QVector2D>
#include <Qt>
#include <QWidget>
#include <iostream>
#include "graph.h"
#include "log.h"

const int VERTEX_RADIUS = 10;
const int HEIGHT_MULTIPLIER = 1;

class ShadowedGraphicsElement {
public:
    virtual void paint(QPainter* painter) = 0;
    virtual void paintShadow(QPainter* painter) = 0;
    virtual void updateNetwork(const Network& network) = 0;
};

class VisualVertex : public ShadowedGraphicsElement {
public:
    QPoint position;
    size_t id;
    const Vertex* vertex;
    std::vector<VisualVertex*> adjacent;

public:
    VisualVertex(size_t id, const Network& network) : id(id) {
        updateNetwork(network);
    };

    QPoint getPosition() {
        return position;
    }

    QPoint getShadowPosition() {
        return position + QPoint(height(), height());
    }

    int height() {
        return vertex->height;
    }

    virtual void updateNetwork(const Network& network) {
        vertex = network.get_vertex(id);
    }

    virtual void paint(QPainter* painter) {
        painter->setBrush(Qt::red);
        painter->drawEllipse(position, VERTEX_RADIUS, VERTEX_RADIUS);
        painter->setFont(QFont("Input", 10, QFont::Bold));
        painter->drawText(QRect(position - QPoint(VERTEX_RADIUS, VERTEX_RADIUS),
                    QSize(VERTEX_RADIUS*2, VERTEX_RADIUS*2)), Qt::AlignCenter, QString().setNum(id));
    }

    virtual void paintShadow(QPainter* painter) {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(getShadowPosition(), VERTEX_RADIUS, VERTEX_RADIUS);
    }

    void addNeighbour(VisualVertex* vertex) {
        adjacent.push_back(vertex);
    }
};

const int ARROW_LENGTH = 20;
const int ARROW_WIDTH = 5;
const int LABEL_SHIFT = 10;
const int SHIFT = 5;
const int FLOW_FACTOR = 3;

class VisualEdge : public ShadowedGraphicsElement {
public:
    VisualVertex* tail;
    VisualVertex* head;
    const Edge* edge;

public:
    VisualEdge(VisualVertex* tail, VisualVertex* head, const Network& network) : tail(tail), head(head) {
        updateNetwork(network);
    }

    virtual void updateNetwork(const Network& network) {
        edge = network.get_edge(tail->id, head->id);
    }

    virtual void paint(QPainter* painter) {
        if (edge->extra_capacity() > 0) {
            painter->setPen(Qt::black);
            QVector2D direction(head->getPosition() - tail->getPosition());
            direction.normalize();
            QVector2D normal(direction.y(), -direction.x());
            painter->drawLine(head->getPosition() - (direction*VERTEX_RADIUS).toPoint() - (normal*SHIFT).toPoint(),
                    head->getPosition() - (direction*ARROW_LENGTH + normal*ARROW_WIDTH).toPoint() - (normal*SHIFT).toPoint());
            painter->drawLine(head->getPosition() - (direction*VERTEX_RADIUS).toPoint() - (normal*SHIFT).toPoint(),
                    head->getPosition() - (direction*ARROW_LENGTH - normal*ARROW_WIDTH).toPoint() - (normal*SHIFT).toPoint());
            painter->drawText(tail->getPosition() + (head->getPosition() - tail->getPosition()) / 2 - LABEL_SHIFT*normal.toPoint(), "x");
            painter->setPen(QPen(Qt::black, FLOW_FACTOR*edge->extra_capacity()));
            painter->drawLine(tail->getPosition() - (normal*SHIFT).toPoint(), head->getPosition() - (normal*SHIFT).toPoint());
            if (false) {
                painter->setPen(QPen(Qt::red, FLOW_FACTOR*edge->flow));
                painter->drawLine(tail->getPosition() - (normal*SHIFT).toPoint(), head->getPosition() - (normal*SHIFT).toPoint());
            };
            painter->setPen(Qt::black);
        }
    }

    virtual void paintShadow(QPainter* painter) {
        if (edge->extra_capacity() > 0) {
            QVector2D direction(head->getPosition() - tail->getPosition());
            direction.normalize();
            QVector2D normal(direction.y(), -direction.x());
            painter->setPen(Qt::gray);
            painter->drawLine(tail->getShadowPosition() - (normal*SHIFT).toPoint(), head->getShadowPosition() - (normal*SHIFT).toPoint());
        }
    }
};

class Visualization : public QWidget {
    std::vector<VisualVertex> vertices;
    std::vector<VisualEdge> edges;
    Log log;
    Network network;

    public:
    void load() {
        size_t vertex_count, edge_count;
        std::cin >> vertex_count >> edge_count;
        network = Network(vertex_count);
        for (size_t i = 0; i < vertex_count; ++i) {
            addVertex(VisualVertex(i, network));
        }
        for (size_t i = 0; i < edge_count; ++i) {
            size_t tail, head, capacity;
            std::cin >> tail >> head >> capacity;
            network.add_edge(tail - 1, head - 1, capacity);
            addEdge(VisualEdge(&vertices[tail - 1], &vertices[head - 1], network));
            addEdge(VisualEdge(&vertices[head - 1], &vertices[tail - 1], network));
            vertices[tail - 1].addNeighbour(&vertices[head - 1]);
            vertices[head - 1].addNeighbour(&vertices[tail - 1]);
        }
    }

    virtual void paintEvent(QPaintEvent*) {
        QPainter painter(this);
        for (auto edge : edges) {
            edge.paintShadow(&painter);
        }
        for (auto vertex : vertices) {
            vertex.paintShadow(&painter);
        }
        for (auto edge : edges) {
            edge.paint(&painter);
        }
        for (auto vertex : vertices) {
            vertex.paint(&painter);
        }
    }

    void updateNetwork(const Network& network) {
        for (auto vertex : vertices) {
            vertex.updateNetwork(network);
        }
        for (auto edge : edges) {
            edge.updateNetwork(network);
        }
    }

    void addVertex(VisualVertex element) {
        vertices.push_back(element);
    }

    void addEdge(VisualEdge element) {
        edges.push_back(element);
    }
};
