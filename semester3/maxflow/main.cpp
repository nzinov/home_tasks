#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graph.h"
#include "visualization.cpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    ShadedCanvas canvas;
    Vertex v, u;
    v.height = 5;
    u.height = 15;
    VisualVertex vertex(&v);
    vertex.position = QPoint(100, 100);
    VisualVertex uertex(&u);
    uertex.position = QPoint(200, 500);
    VisualEdge edge;
    edge.head = &vertex;
    edge.tail = &uertex;
    canvas.addElement(&edge);
    canvas.addElement(&vertex);
    canvas.addElement(&uertex);
    canvas.show();

    return app.exec();
}
