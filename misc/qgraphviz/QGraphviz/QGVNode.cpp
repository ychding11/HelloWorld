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

#include <QGVNode.h>
#include <QGVCore.h>
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>
#include <iostream>

using std::cout;
using std::ostream;

ostream& operator<<(ostream &os, const QGVNode &node)
{
    os << "- Agnode_t address: " << node._node << std::endl;
    return os;
}

QGVNode::QGVNode(Agnode_t *node, QGVScene *scene): _scene(scene), _node(node)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVNode::QGVNode(const QString &name, QGVScene *scene) :  _label(name), _scene(scene)
{
	char *nodeName = name.toLocal8Bit().data();
    //char *nodeName = const_cast<char*>(name.toStdString().c_str());
	_node = agnode(_scene->_graph, nodeName, TRUE);
    if(_node == NULL)
    {
        qCritical() << "- Create agnode failed.";
    }
    qDebug() << "- Construct QGNode:" << name;
    qDebug() << "  QGVNode Address: " << this; 
    qDebug() << "  Agnode_t address: " << _node;
    qDebug() << "  Agnode_t label: " << nodeName;
    qDebug() << "  QString label: " << _label;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVNode::~QGVNode()
{
    _scene->removeItem(this);
    /* release Agnode_t resources in graphviz */
	_node = NULL;
}

QString QGVNode::label() const
{
    return getAttribute("label");
}

void QGVNode::setLabel(const QString &label)
{
    setAttribute("label", label);
    _label = label;
}

QRectF QGVNode::boundingRect() const
{
    return _path.boundingRect();
}

void QGVNode::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    cout << "QGVNode::paint()" << std::endl;
    painter->save();
    painter->setPen(_pen);
    if(isSelected())
    {
        QBrush tbrush(_brush);
        tbrush.setColor(tbrush.color().darker(120));
        painter->setBrush(tbrush);
    }
    else
        painter->setBrush(_brush);

    painter->drawPath(_path);
    //painter->setPen(QGVCore::toColor(getAttribute(labelColour)));
    QColor labelColor(attribute("labelfontcolor"));
    painter->setPen(labelColor);
    const QRectF rect = boundingRect().adjusted(2,2,-2,-2); //Margin
    if(_icon.isNull())
    {
        qDebug() << "- Prepare drawing QGVNode Label:" << _label; 
        painter->drawText(rect, Qt::AlignCenter, _label);
    }
    else
    {
        painter->drawText(rect.adjusted(0,0,0, -rect.height()*2/3), Qt::AlignCenter , QGVNode::label());

        const QRectF img_rect = rect.adjusted(0, rect.height()/3,0, 0);
        QImage img = _icon.scaled(img_rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter->drawImage(img_rect.topLeft() + QPointF((img_rect.width() - img.rect().width())/2, 0), img);
    }
    painter->restore();
}

const char* QGVNode::attribute(char *attr) const
{
	const char* value = agget(_node, attr);
    if(value) return value;
    return NULL;
}

void QGVNode::attribute(char *attr, char *value)
{
    char defaultValue[] = "";
	agsafeset(_node, attr, value, defaultValue);
}

void QGVNode::setAttribute(const QString &name, const QString &value)
{
    char defaultValue[] = "";
	agsafeset(_node, name.toLocal8Bit().data(), value.toLocal8Bit().data(), defaultValue);
}

QString QGVNode::getAttribute(const QString &name) const
{
	char* value = agget(_node, name.toLocal8Bit().data());
    if(value) return value;
    return QString();
}

void QGVNode::setIcon(const QImage &icon) { _icon = icon; }

void QGVNode::updateLayout()
{
    cout << "- QGVNode::updateLayout()" << std::endl;
    prepareGeometryChange();
	qreal width  = ND_width(_node) *  DotDefaultDPI;
	qreal height = ND_height(_node) * DotDefaultDPI;

    //ND_coord gives the position of the center of the node, in points
    pointf nodePos = ND_coord(_node);
    printf("- Agnode_t 0x%p, x = %lf, y = %lf, w = %lf, h = %lf\n", _node, nodePos.x, nodePos.y, width, height);
    qreal gheight  = GD_bb(_scene->_graph).UR.y;
    QPointF centerPoint(nodePos.x, gheight - nodePos.y);
    QPointF cornerPoint(centerPoint.x() - width / 2.0, centerPoint.y() - height / 2.0);

    //Node path
	_path = QGVCore::toPath(ND_shape(_node)->name, (polygon_t*)ND_shape_info(_node), width, height);

	setPos(centerPoint);
    setZValue(1);

    _brush.setStyle(QGVCore::toBrushStyle(getAttribute("style")));
    _brush.setColor(QGVCore::toColor(getAttribute("fillcolor")));
    _pen.setWidth(1);
    _pen.setColor(QGVCore::toColor(getAttribute("color")));

    setToolTip(getAttribute("tooltip"));
}
