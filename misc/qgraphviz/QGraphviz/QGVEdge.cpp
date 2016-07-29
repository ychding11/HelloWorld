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

#include <QGVEdge.h>
#include <QGVCore.h>
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>

using std::cout;

ostream& operator<<(ostream &os, const QGVEdge &edge)
{
    os << "- QGVEdge info:" << std::endl;
    os << "- ----------------" << std::endl;
    return os;
}

QGVEdge::QGVEdge(Agedge_t *edge, QGVScene *scene) :  _scene(scene), _edge(edge)
{
    qDebug() << "- Construct an QGVEdge:" << this;
    qDebug() << "  Agedge_t address:"  << _edge;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVEdge::~QGVEdge()
{
    _scene->removeItem(this);
    /* release _edge resouces in graphviz */
	_edge = NULL;
}

QString QGVEdge::label() const { return getAttribute("xlabel"); }

void QGVEdge::setLabel(const QString &label)
{
     qDebug() << "- Set QGVEdge " << this << " Label:" << label;
     setAttribute("xlabel", label); 
     _label = label;
}

QRectF QGVEdge::boundingRect() const
{
    return _path.boundingRect() | _head_arrow.boundingRect() | _tail_arrow.boundingRect() | _label_rect;
}

QPainterPath QGVEdge::shape() const
{
    QPainterPathStroker ps;
    ps.setCapStyle(_pen.capStyle());
    ps.setWidth(_pen.widthF() + 10);
    ps.setJoinStyle(_pen.joinStyle());
    ps.setMiterLimit(_pen.miterLimit());
    return ps.createStroke(_path);
}

void QGVEdge::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    cout << __FUNCTION__ << std::endl;
    painter->save();
    if(isSelected())
    {
        QPen tpen(_pen);
        tpen.setColor(_pen.color().darker(120));
        tpen.setStyle(Qt::DotLine);
        painter->setPen(tpen);
    }
    else
        painter->setPen(_pen);

    painter->drawPath(_path);

    /*
    QRectF pp = _path.controlPointRect();
    if(pp.width() < pp.height())
    {
        painter->save();
        painter->translate(_label_rect.topLeft());
        painter->rotate(90);
        painter->drawText(QRectF(QPointF(0, -_label_rect.width()), _label_rect.size()), Qt::AlignCenter, _label);
        painter->restore();
    }
    else
    */
    qDebug() << "- Prepare drawing edge label:" << _label << " Edge bounding box:" << _label_rect;
    painter->drawText(_label_rect, Qt::AlignCenter, _label);

    painter->setBrush(QBrush(_pen.color(), Qt::SolidPattern));
    painter->drawPolygon(_head_arrow);
    painter->drawPolygon(_tail_arrow);
    painter->restore();
}

void QGVEdge::setAttribute(const QString &name, const QString &value)
{
    qDebug() << "- Set edge attribute: " << name.toLocal8Bit().data() <<" " << value.toLocal8Bit().data();
    char empty[] = "";
	agsafeset(_edge, name.toLocal8Bit().data(), value.toLocal8Bit().data(), empty);
}

QString QGVEdge::getAttribute(const QString &name) const
{
	char* value = agget(_edge, name.toLocal8Bit().data());
    if(value) return value;
    return QString();
}

void QGVEdge::updateLayout()
{
    qDebug() << "- QGVEdge::updateLayout()";
    prepareGeometryChange();
    
    // Convert graphviz concept into Qt
	//qreal gheight = QGVCore::graphHeight(_scene->_graph);
    qreal gheight  = GD_bb(_scene->_graph).UR.y;
	const splines* spl = ED_spl(_edge);
    _path = QGVCore::toPath(spl, gheight);

    // Control points number is always 3 * n + 1
    if((spl->list != 0) && (spl->list->size % 3 == 1))
    {
        if(spl->list->sflag)
        {
            _tail_arrow = toArrow(QLineF(QGVCore::toPoint(spl->list->list[0], gheight), QGVCore::toPoint(spl->list->sp, gheight)));
        }

        if(spl->list->eflag)
        {
            _head_arrow = toArrow(QLineF(QGVCore::toPoint(spl->list->list[spl->list->size-1], gheight), QGVCore::toPoint(spl->list->ep, gheight)));
        }
    }

    _pen.setWidth(1);
    _pen.setColor(QGVCore::toColor(getAttribute("color")));
    _pen.setStyle(QGVCore::toPenStyle(getAttribute("style")));

    //Edge label
	textlabel_t *xlabel = ED_xlabel(_edge);
    if(xlabel)
    {
        _label = xlabel->text;
        _label_rect.setSize(QSize(xlabel->dimen.x, xlabel->dimen.y));
		_label_rect.moveCenter(QGVCore::toPoint(xlabel->pos, gheight));
    }

    setToolTip(getAttribute("tooltip"));
}

QPolygonF QGVEdge::toArrow(const QLineF &line) const
{
    QLineF n = line.normalVector();
    QPointF o(n.dx() / 3.0, n.dy() / 3.0);

    //Only support normal arrow type
    QPolygonF polygon;
    polygon.append(line.p1() + o);
    polygon.append(line.p2());
    polygon.append(line.p1() - o);

    return polygon;
}
