#include "visualization.h"

VisualVertex::VisualVertex(size_t id) : id(id) {};

QPoint VisualVertex::getPosition() {
    return position;
}

QPoint VisualVertex::getShadowPosition() {
    return position + QPoint(height(), height());
}

int VisualVertex::height() {
    return vertex->height;
}

void VisualVertex::updateNetwork(const Network& network) {
    vertex = network.get_vertex(id);
}

void VisualVertex::paint(QPainter* painter) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(position, VERTEX_RADIUS, VERTEX_RADIUS);
    painter->setFont(QFont("Input", 10, QFont::Bold));
    painter->drawText(QRect(position - QPoint(VERTEX_RADIUS, VERTEX_RADIUS),
                QSize(VERTEX_RADIUS*2, VERTEX_RADIUS*2)), Qt::AlignCenter, QString().setNum(id));
}

void VisualVertex::paintShadow(QPainter* painter) {
    painter->setBrush(Qt::gray);
    painter->drawEllipse(getShadowPosition(), VERTEX_RADIUS, VERTEX_RADIUS);
}

void VisualVertex::addNeighbour(VisualVertex* vertex) {
    adjacent.push_back(vertex);
}

QVector2D VisualVertex::calculateSprings() {
    QVector2D sum;
    for (auto neighbour : adjacent) {
        const int NORMAL_DIST = 30;
        const qreal COEF = 0.02;
        QVector2D move(neighbour->position - position);
        move -= move.normalized()*NORMAL_DIST;
        move *= COEF;
        sum += move;
    }
    return sum;
}

QVector2D VisualVertex::calculateCharge(const std::vector<VisualVertex>& vertices) {
    QVector2D sum;
    for (const VisualVertex& other : vertices) {
        if (&other == this) {
            continue;
        }
        const qreal COEF = 0.5;
        QVector2D move(other.position - position);
        sum += -move.normalized()*(COEF/move.length()*move.length());
    }
    return sum;
}

void VisualVertex::simulate(const std::vector<VisualVertex>& vertices) {
    QPoint move = (calculateCharge(vertices)).toPoint();
    this->position += move;
}

VisualEdge::VisualEdge(VisualVertex* tail, VisualVertex* head) : tail(tail), head(head) {}

void VisualEdge::updateNetwork(const Network& network) {
    edge = network.get_edge(tail->id, head->id);
}

void VisualEdge::paint(QPainter* painter) {
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

void VisualEdge::paintShadow(QPainter* painter) {
    if (edge->extra_capacity() > 0) {
        QVector2D direction(head->getPosition() - tail->getPosition());
        direction.normalize();
        QVector2D normal(direction.y(), -direction.x());
        painter->setPen(Qt::gray);
        painter->drawLine(tail->getShadowPosition() - (normal*SHIFT).toPoint(), head->getShadowPosition() - (normal*SHIFT).toPoint());
    }
}

void Visualization::load(Network new_network) {
    network = std::move(new_network);
    for (size_t i = 0 ; i < network.vertex_count; ++i) {
        addVertex();
    }
    for (size_t i = 0 ; i < network.vertex_count; ++i) {
        for (size_t j = 0 ; j < i; ++j) {
            if (network.edges[i][j].capacity > 0 || network.edge[j][i].capacity > 0) {
                addEdge(i, j);
            }
        }
    }
    updateNetwork(network);
}

void Visualization::paintEvent(QPaintEvent*) {
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
    static int counter = 0;
    painter.drawText(50, 50, QString().setNum(counter));
    ++counter;
}

void Visualization::updateNetwork(const Network& network) {
    for (VisualVertex& vertex : vertices) {
        vertex.updateNetwork(network);
    }
    for (VisualEdge& edge : edges) {
        edge.updateNetwork(network);
    }
}

VisualVertex* Visualization::addVertex() {
    vertices.emplace_back(vertices.size());
    return &vertices.back();
}

VisualEdge* Visualization::addEdge(size_t tail, size_t head) {
    edges.emplace_back(&vertices[tail], &vertices[head]);
    edges.emplace_back(&vertices[head], &vertices[tail]);
    vertices[tail].addNeighbour(&vertices[head]);
    vertices[head].addNeighbour(&vertices[tail]);
    return &edges.back();
}

void Visualization::simulate() {
    for (VisualVertex&  vertex : vertices) {
        vertex.simulate(vertices);
    }
    update();
}
