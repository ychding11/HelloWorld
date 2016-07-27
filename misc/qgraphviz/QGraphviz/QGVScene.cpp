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

#include "QGVScene.h"
// The following include allows the automoc to detect, that it must moc this class
#include "moc_QGVScene.cpp"
#include <QDebug>
#include <QGVNode.h>
#include <QGVEdge.h>
#include <QGVSubGraph.h>
#include <QGVCore.h>
#include <iostream>
#include <cstdio>

using std::cout;

ostream& operator<<(ostream &os, QGVScene &scene)
{
    os << "- QGVScene info:" << std::endl;
    os << "-   Context address: " << scene._context << std::endl;
    os << "-   Agraph  address: " << scene._graph << std::endl;
    os << "-   Layout  name   : " << scene._layoutName << std::endl;
    int i = 1;
    for (auto a : scene._nodes)
    {
        os << "-   QGVNode " << i << " info:"  << std::endl;
        os << "-     " << *a;
    }
    os << "- -------------------------" << std::endl;
    return os;
}

QGVScene::QGVScene(const QString &name, QObject *parent)
    : QGraphicsScene(parent)
    , _context(NULL)
    , _graph(NULL)
    , _layoutName("dot")
    , _nodes()
    , _edges()
    , _subGraphs()

{
	_context = gvContext();
	_graph   = agopen(name.toLocal8Bit().data(), Agdirected, NULL);
    if (!_context || !_graph)
    {
        cout << "- Constructor QGVScene object error." << std::endl;
    }
    cout << "- Construct QGVScene Object." << std::endl;
    //setGraphAttribute("fontname", QFont().family());
}

QGVScene::~QGVScene()
{
	gvFreeLayout(_context, _graph);
	agclose(_graph);
	gvFreeContext(_context);
    _graph = NULL;
    _context = NULL;
    _nodes.clear();
    _edges.clear();
    _subGraphs.clear();
}

void QGVScene::setGraphAttribute(const QString &name, const QString &value)
{
    /* _graph must be a root graph */
	agattr(_graph, AGRAPH, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setNodeAttribute(const QString &name, const QString &value)
{
    /* _graph must be a root graph */
	agattr(_graph, AGNODE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setEdgeAttribute(const QString &name, const QString &value)
{
    /* _graph must be a root graph */
	agattr(_graph, AGEDGE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

QGVNode* QGVScene::addNode(const QString &label)
{
    //char *name = label.toLocal8Bit().data();
    char *name = const_cast<char*>(label.toStdString().c_str());
    cout << "- Add Node into scene. Node name: " << name << std::endl;
	QGVNode *item = new QGVNode(name, this);
    _nodes.push_back(item);
    addItem(item);
    return item;
}

QGVEdge *QGVScene::addEdge(QGVNode *source, QGVNode *target, const string &label)
{
#if 1
    cout << "- Add an edge into scene." << std::endl;
	Agedge_t* edge = agedge(_graph, source->_node, target->_node, NULL, TRUE);
    if(edge == NULL)
    {
        cout << "- Create egde failed:" << label;
        return 0;
    }

	QGVEdge *item = new QGVEdge(edge, this);
    item->setLabel(label.c_str());
    addItem(item);
    _edges.push_back(item);
    return item;
    #endif
}

QGVSubGraph* QGVScene::addSubGraph(const QString &name, bool cluster)
{
#if 0
    Agraph_t* sgraph;
    if(cluster)
		sgraph = agsubg(_graph, ("cluster_" + name).toLocal8Bit().data(), TRUE);
    else
		sgraph = agsubg(_graph, name.toLocal8Bit().data(), TRUE);

    if(sgraph == NULL)
    {
        qWarning()<<"- Create subGraph failed:" << name;
        return 0;
    }

	QGVSubGraph *item = new QGVSubGraph(sgraph, this);
    addItem(item);
    _subGraphs.push_back(item);
    return item;
    #endif
    return 0;
}

void QGVScene::setRootNode(QGVNode *node)
{
    char attr[] = "root";
	agset(_graph, attr, node->label().toLocal8Bit().data());
}

void QGVScene::loadLayout(const QString &text)
{
#if 0
     gvFreeLayout(_context, _graph);
	_graph =  QGVCore::agmemread2(text.toLocal8Bit().constData());
	if(gvLayout(_context, _graph, _layoutName.c_str()) != 0)
    {
        qCritical()<<" gvLayout() error" << agerrors() << QString::fromLocal8Bit(aglasterr());
        return;
    }

    //Debug output
	//gvRenderFilename(_context, _graph, "png", "debug.png");

    //Read nodes and edges
	for (Agnode_t* node = agfstnode(_graph); node != NULL; node = agnxtnode(_graph, node))
    {
		QGVNode *inode = new QGVNode(node, this);
        inode->updateLayout();
        addItem(inode);
		for (Agedge_t* edge = agfstout(_graph, node); edge != NULL; edge = agnxtout(_graph, edge))
        {
			QGVEdge *iedge = new QGVEdge(edge, this);
            iedge->updateLayout();
            addItem(iedge);
        }
    }
    #endif
    update();
}

void QGVScene::applyLayout()
{
    gvFreeLayout(_context, _graph);
    if(gvLayout(_context, _graph, _layoutName.c_str()) != 0)
    {
        /*
         * Si plantage ici :
         *  - Verifier que les dll sont dans le repertoire d'execution
         *  - Verifie que le fichier "configN" est dans le repertoire d'execution !
         */
        qCritical()<<" gvLayout() error" << agerrors() << QString::fromLocal8Bit(aglasterr());
        return;
    }
    cout << "- Current Layout:" << _layoutName << std::endl;
    //Debug output
	//gvRenderFilename(_context->context(), _graph->graph(), "canon", "debug.dot");
	//gvRenderFilename(_context->context(), _graph->graph(), "png", "debug.png");
    cout << *this << std::endl;

    for (auto a : _nodes)
    {
        a->updateLayout();
    }
    for (auto a : _edges)
    {
        a->updateLayout();
    }
    for (auto a : _subGraphs)
    {
        //a->updateLayout();
    }

    //Graph label
	textlabel_t *xlabel = GD_label(_graph);
    if(xlabel)
    {
        cout << "- Graph label text from graphviz: " << (xlabel->text) << std::endl;
        QGraphicsTextItem *item = addText(xlabel->text);
        QPointF pos = QGVCore::centerToOrigin(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_graph)), xlabel->dimen.x, -4);
		item->setPos(pos);
    }
    update();
}

void QGVScene::clear()
{
	gvFreeLayout(_context, _graph);
    _nodes.clear();
    _edges.clear();
    _subGraphs.clear();
    QGraphicsScene::clear();
}

#include <QGraphicsSceneContextMenuEvent>
void QGVScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QGraphicsItem *item = itemAt(contextMenuEvent->scenePos(), QTransform());
    if(item)
    {
        item->setSelected(true);
        if(item->type() == QGVNode::Type)
            emit nodeContextMenu(qgraphicsitem_cast<QGVNode*>(item));
        else if(item->type() == QGVEdge::Type)
            emit edgeContextMenu(qgraphicsitem_cast<QGVEdge*>(item));
        else if(item->type() == QGVSubGraph::Type)
            emit subGraphContextMenu(qgraphicsitem_cast<QGVSubGraph*>(item));
        else
            emit graphContextMenuEvent();
    }
    QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void QGVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
    if(item)
    {
        if(item->type() == QGVNode::Type)
            emit nodeDoubleClick(qgraphicsitem_cast<QGVNode*>(item));
        else if(item->type() == QGVEdge::Type)
            emit edgeDoubleClick(qgraphicsitem_cast<QGVEdge*>(item));
        else if(item->type() == QGVSubGraph::Type)
            emit subGraphDoubleClick(qgraphicsitem_cast<QGVSubGraph*>(item));
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

#include <QVarLengthArray>
#include <QPainter>
void QGVScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    const int gridSize = 25;

    const qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    const qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->setPen(QColor(Qt::lightGray).lighter(110));
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(Qt::black);
    //painter->drawRect(sceneRect());
}
