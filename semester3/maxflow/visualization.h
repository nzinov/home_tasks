#ifndef VISUALIZATION
#define VISUALIZATION
#include <QRectF>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QFont>
#include <QKeyEvent>
#include <QVector2D>
#include <Qt>
#include <QWidget>
#include "graph.h"
#include "log.h"

const int VERTEX_RADIUS = 10;
const int HEIGHT_MULTIPLIER = 7;

class ShadowedGraphicsElement {
public:
    virtual void paint(QPainter* painter) = 0;
    virtual void paintShadow(QPainter* painter) = 0;
    virtual void updateNetwork(const Network& network) = 0;
};

struct VisualVertex : public ShadowedGraphicsElement {
    QPoint position;
    size_t id;
    const Vertex* vertex;
    std::vector<VisualVertex*> adjacent;

    VisualVertex(size_t id); 
    QPoint getPosition(); 
    QPoint getShadowPosition(); 
    int height(); 
    virtual void updateNetwork(const Network& network); 
    virtual void paint(QPainter* painter); 
    virtual void paintShadow(QPainter* painter); 
    void addNeighbour(VisualVertex* vertex); 
    QVector2D calculateSprings(); 
    QVector2D calculateCharge(const std::vector<VisualVertex>& vertices); 
    void simulate(const std::vector<VisualVertex>& vertices); 
};

const int ARROW_LENGTH = 20;
const int ARROW_WIDTH = 5;
const int LABEL_SHIFT = 20;
const int SHIFT = 7;
const int FLOW_FACTOR = 2;

struct VisualEdge : public ShadowedGraphicsElement {
    VisualVertex* tail;
    VisualVertex* head;
    const Edge* edge;
    static void drawArrow(QPainter* painter, QPoint tail, QPoint head);

    VisualEdge(VisualVertex* tail, VisualVertex* head); 
    virtual void updateNetwork(const Network& network); 
    virtual void paint(QPainter* painter); 
    virtual void paintShadow(QPainter* painter); 
};

class Visualization : public QWidget {
    Q_OBJECT;
    std::vector<VisualVertex> vertices;
    std::vector<VisualEdge> edges;
    Log log;
    Network network;

    public:
    void load(Network network); 
    virtual void paintEvent(QPaintEvent*); 
    virtual void keyPressEvent(QKeyEvent* event);
    void updateNetwork(const Network& network); 
    VisualVertex* addVertex(); 
    VisualEdge* addEdge(size_t tail, size_t head); 
    void runAlgorithm();
    void stepForward(bool backward);
    void stepBackward();

public slots:
    void simulate();
};
#endif
