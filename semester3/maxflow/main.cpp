#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graph.h"
#include "visualization.cpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    Vertex v;
    v.height = 5;
    VisualVertex vertex(nullptr, &v);
    scene.addItem(&vertex);

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}
