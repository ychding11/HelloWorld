/*
 * Copyright (C) 2014-2016  Yaochuang Ding - <ych_ding@163.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution, and in the same 
 *    place and form as other copyright, license and disclaimer information.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */

#ifndef QGVSCENE_H
#define QGVSCENE_H

#include "qgv.h"
#include <QGraphicsScene>
#include <string>
#include <vector>
#include <gvc.h>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;

class QGVNode;
class QGVEdge;
class QGVSubGraph;

class QGVCORE_EXPORT QGVScene : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit QGVScene(const QString &name, QObject *parent = 0);
    ~QGVScene();

    void setGraphAttribute(const QString &name, const QString &value);
    void setNodeAttribute(const QString &name, const QString &value);
    void setEdgeAttribute(const QString &name, const QString &value);

    QGVNode* addNode(const QString& label);
    QGVEdge* addEdge(QGVNode* source, QGVNode* target, const string& label = string());
    QGVSubGraph* addSubGraph(const QString& name, bool cluster=true);

    void setRootNode(QGVNode *node);

    void loadLayout(const QString &text);
    void applyLayout();
    void clear();
    friend ostream& operator<<(ostream &os, QGVScene &scene);

signals:
    void nodeContextMenu(QGVNode* node);
    void nodeDoubleClick(QGVNode* node);

    void edgeContextMenu(QGVEdge* edge);
    void edgeDoubleClick(QGVEdge* edge);

    void subGraphContextMenu(QGVSubGraph* graph);
    void subGraphDoubleClick(QGVSubGraph* graph);

    void graphContextMenuEvent();
    
public slots:

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * contextMenuEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void drawBackground(QPainter * painter, const QRectF & rect);

private:
    friend class QGVNode;
    friend class QGVEdge;
    friend class QGVSubGraph;

	GVC_t       *_context;
	Agraph_t    *_graph;
    string      _layoutName;
    //QFont _font;

    vector<QGVNode*> _nodes;
    vector<QGVEdge*> _edges;
    vector<QGVSubGraph*> _subGraphs;
};

#endif // QGVSCENE_H
