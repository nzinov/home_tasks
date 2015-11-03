#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QtGlobal>
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
    canvas.load(read_network());
    qsrand(57);
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &canvas, SLOT(simulate()));
    timer.start(25);
    canvas.show();
    return app.exec();
}
