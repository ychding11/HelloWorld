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
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "ui_MainWindow.h"
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _scene = new QGVScene("DEMO", this);

    _scene->setGraphAttribute("label", "DEMO");
    _scene->setGraphAttribute("splines", "ortho");
    _scene->setGraphAttribute("rankdir", "LR");
    _scene->setGraphAttribute("nodesep", "0.4");

    _scene->setNodeAttribute("shape", "circle");
    _scene->setNodeAttribute("style", "filled");
    _scene->setNodeAttribute("fillcolor", "white");
    _scene->setNodeAttribute("height", "1.2");
    
    _scene->setEdgeAttribute("minlen", "3");

    QGVNode *src =  _scene->addNode(string("src"));
    QGVNode *dst =  _scene->addNode(string("dst"));
    QGVEdge *edge = _scene->addEdge(src, dst, "test");

    _scene->applyLayout();

    connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
}

MainWindow::~MainWindow()
{
    delete _scene;
}

void MainWindow::drawGraph()
{
    //loadGraph("graph.txt");

    //_scene->addNode("Test");
    //Layout scene
    _scene->applyLayout();

}

#if 0
void MainWindow::loadGraph(const QString & filename)
{
    fstream file(filename.toStdString().c_str());
    int numNode, numEdge;
    int u, v, w;
    file >> numNode;
    vector<QGVNode* > nodes(numNode);
    for (int i = 0; i < numNode; ++i)
    {
        char label[32];
        sprintf(label, "%d", i);
        nodes[i] = _scene->addNode(label);
    }
    file >> numEdge;
    for (int i = 0; i < numEdge; ++i)
    {
        file >> u >> v >> w;
        char label[32];
        sprintf(label, "%d", w);
        _scene->addEdge(nodes[u], nodes[v], label);
    }
}
#endif

void MainWindow::nodeContextMenu(QGVNode *node)
{
    //Context menu exemple
    QMenu menu(node->label());

    menu.addSeparator();
    menu.addAction(tr("Informations"));
    menu.addAction(tr("Options"));

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0) return;
}

void MainWindow::nodeDoubleClick(QGVNode *node)
{
    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
}
