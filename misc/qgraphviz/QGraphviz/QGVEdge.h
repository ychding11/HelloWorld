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

#ifndef QGVEDGE_H
#define QGVEDGE_H

#include "qgv.h"
#include <QGraphicsItem>
#include <QPen>
#include <gvc.h>
#include <iostream>

using std::ostream;

class QGVNode;
class QGVScene;

class QGVCORE_EXPORT QGVEdge : public QGraphicsItem
{
public:
    ~QGVEdge();

    QString label() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setLabel(const QString &label);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setAttribute(const QString &name, const QString &value);
    QString getAttribute(const QString &name) const;

    void updateLayout();

    enum { Type = UserType + 3 };
    int type() const { return Type; }

    friend ostream& operator<<(ostream &os, const QGVEdge &edge);
private:
	QGVEdge(Agedge_t *edge, QGVScene *scene);
    QPolygonF toArrow(const QLineF &normal) const;

    friend class QGVScene;
    //friend class QGVSubGraph;

    QGVScene *_scene;
	Agedge_t *_edge;

    QPainterPath _path;
    QPen _pen;
    QPolygonF _head_arrow;
    QPolygonF _tail_arrow;

    QString _label;
    QRectF  _label_rect;
};

#endif // QGVEDGE_H
