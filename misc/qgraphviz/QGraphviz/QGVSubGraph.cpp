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

#include "QGVSubGraph.h"
#include <QGVCore.h>
#include <QGVScene.h>
#include <QGVNode.h>
#include <QDebug>
#include <QPainter>

QGVSubGraph::QGVSubGraph(Agraph_t *subGraph, QGVScene *scene):  _scene(scene), _sgraph(subGraph)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVSubGraph::~QGVSubGraph()
{
    _scene->removeItem(this);
	//delete _sgraph;
	_sgraph = NULL;
}

QString QGVSubGraph::name() const
{
	return QString::fromLocal8Bit(GD_label(_sgraph)->text);
}

QGVNode *QGVSubGraph::addNode(const QString &label)
{
	Agnode_t *node = agnode(_sgraph, NULL, TRUE);
    if(node == NULL)
    {
        qWarning()<<"- Create sub node failed:" << label;
        return 0;
    }
	agsubnode(_sgraph, node, TRUE);

	QGVNode *item = new QGVNode(node, _scene);
    item->setLabel(label);
    _scene->addItem(item);
    _scene->_nodes.append(item);
    _nodes.append(item);
    return item;
}

QGVSubGraph *QGVSubGraph::addSubGraph(const QString &name, bool cluster)
{
    Agraph_t* sgraph = NULL;
    if(cluster)
		sgraph = agsubg(_sgraph, ("cluster_" + name).toLocal8Bit().data(), TRUE);
    else
		sgraph = agsubg(_sgraph, name.toLocal8Bit().data(), TRUE);

    if(sgraph == NULL)
    {
        qWarning() << "- Create subGraph failed:" << name;
        return 0;
    }

	QGVSubGraph *item = new QGVSubGraph(sgraph, _scene);
    _scene->_subGraphs.append(item);
    _scene->addItem(item);
    return item;
}

QRectF QGVSubGraph::boundingRect() const
{
    return QRectF(0,0, _width, _height);
}

void QGVSubGraph::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setPen(_pen);
    painter->setBrush(_brush);
    painter->drawRect(boundingRect());
    painter->drawText(_label_rect, Qt::AlignCenter, _label);
    painter->restore();
}

void QGVSubGraph::setAttribute(const QString &name, const QString &value)
{
    char empty[] = "";
	agsafeset(_sgraph, name.toLocal8Bit().data(), value.toLocal8Bit().data(), empty);
}

QString QGVSubGraph::getAttribute(const QString &name) const
{
	char* value = agget(_sgraph, name.toLocal8Bit().data());
    if(value) return value;
    return QString();
}

void QGVSubGraph::updateLayout()
{
    prepareGeometryChange();

    //SubGraph box
	boxf box = GD_bb(_sgraph);
    pointf p1 = box.UR;
    pointf p2 = box.LL;
    _width  = p1.x - p2.x;
    _height = p1.y - p2.y;

	qreal gheight = QGVCore::graphHeight(_scene->_graph);
    setPos(p2.x, gheight - p1.y);

    _pen.setWidth(1);
    _pen.setColor(QGVCore::toColor(getAttribute("color")));
    _brush.setStyle(QGVCore::toBrushStyle(getAttribute("style")));
    _brush.setColor(QGVCore::toColor(getAttribute("fillcolor")));

    //SubGraph label
	textlabel_t *xlabel = GD_label(_sgraph);
    if(xlabel)
    {
        _label = xlabel->text;
        _label_rect.setSize(QSize(xlabel->dimen.x, xlabel->dimen.y));
		_label_rect.moveCenter(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_scene->_graph)) - pos());
    }
}
