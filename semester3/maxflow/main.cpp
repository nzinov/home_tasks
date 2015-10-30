#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "graph.h"
#include "visualization.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Visualization canvas;
    Network network(2);
    network.add_edge(1, 0, 1);
    canvas.addVertex()->position = QPoint(100, 300);
    canvas.addVertex()->position = QPoint(700, 600);
    canvas.addEdge(0, 1);
    canvas.addEdge(1, 0);
    canvas.updateNetwork(network);
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &canvas, SLOT(simulate()));
    timer.start(25);
    canvas.show();
    return app.exec();
}
