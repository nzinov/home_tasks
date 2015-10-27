#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graph.h"
#include "visualization.cpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    GraphScene scene;
    Vertex v;
    v.height = 5;
    VisualVertex vertex(&v);
    scene.addShadowedItem(&vertex);

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}
