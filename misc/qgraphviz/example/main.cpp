/***************************************************************
QGVCore Sample
Copyright (c) 2014, Bergont Nicolas, All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.
***************************************************************/
#include "QGraphicsViewEc.h"
#include <QApplication>
#include <QString>
#include "QGVScene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGVScene scene("DEMO", NULL);

    scene.setGraphAttribute("label", "DEMO");
    scene.setGraphAttribute("splines", "ortho");
    scene.setGraphAttribute("rankdir", "LR");
    scene.setGraphAttribute("nodesep", "0.4");

    scene.setNodeAttribute("shape", "circle");
    scene.setNodeAttribute("style", "filled");
    scene.setNodeAttribute("fillcolor", "white");
    scene.setNodeAttribute("height", "1.2");
   
    scene.setEdgeAttribute("minlen", "3");

    QGVNode *src =  scene.addNode("src");
    QGVNode *dst =  scene.addNode("dst");
    QGVEdge *edge = scene.addEdge(src, dst, "dge");

    scene.applyLayout();

    QGraphicsViewEc view(&scene);
    view.resize(560, 720);
    view.show(); 
    return a.exec();
}
