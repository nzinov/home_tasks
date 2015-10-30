#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "graph.h"
#include "visualization.cpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Visualization canvas;
    Network network(2);
    network.add_edge(1, 0, 1);
    VisualVertex vertex(0, network);
    vertex.position = QPoint(100, 100);
    VisualVertex uertex(1, network);
    uertex.position = QPoint(200, 500);
    VisualEdge edge(&uertex, &vertex, network);
    VisualEdge edge2(&vertex, &uertex, network);
    canvas.addEdge(edge);
    canvas.addEdge(edge2);
    canvas.addVertex(vertex);
    canvas.addVertex(uertex);
    canvas.updateNetwork(network);
    canvas.show();
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &canvas, SLOT(simulate()));
    timer.start(500);

    return app.exec();
}
