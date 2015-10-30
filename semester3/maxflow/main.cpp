#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <iostream>
#include "graph.h"
#include "visualization.h"

Network read_network() {
    size_t vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Network network(vertex_count);
    for (size_t i = 0; i < edge_count; ++i) {
        size_t tail, head, capacity;
        std::cin >> tail >> head >> capacity;
        network.add_edge(tail - 1, head - 1, capacity);
    }
    return network;
}

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
