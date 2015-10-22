#include <vector>
#include <iostream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graph.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    scene.addText("Hello, world!");

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}
