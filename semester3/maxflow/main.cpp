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
    Vertex v;
    v.height = 5;
    VisualVertex vertex(&v);
    canvas.addElement(&vertex);
    canvas.show();

    return app.exec();
}
