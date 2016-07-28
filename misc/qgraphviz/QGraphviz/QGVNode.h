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

#ifndef QGVNODE_H
#define QGVNODE_H

#include "qgv.h"
#include <QGraphicsItem>
#include <QPen>
#include <gvc.h>
#include <iostream>

using std::ostream;

class QGVEdge;
class QGVScene;

/**
 * @brief Node item
 *
 */
class QGVCORE_EXPORT QGVNode : public QGraphicsItem
{
public:
    ~QGVNode();

    QString label() const;
    void setLabel(const QString &label);

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &label, const QString &value);
    QString getAttribute(const QString &name) const;

    void setIcon(const QImage &icon);

    const char* attribute(char *attr) const;
    void  attribute(char *attr, char *value);

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    friend ostream& operator<<(ostream &os, const QGVNode &node);

private:
    friend class QGVScene;
    friend class QGVSubGraph;

    void updateLayout();
	QGVNode(Agnode_t *node, QGVScene *scene);
	QGVNode(const QString &name, QGVScene *scene);

		// Not implemented in QGVNode.cpp
//		QPainterPath makeShape(Agnode_t* node) const;
//		QPolygonF makeShapeHelper(Agnode_t* node) const;

    QPainterPath _path;
    QPen _pen;
    QBrush _brush;
    QImage _icon;
    QString _label;

    QGVScene *_scene;
	Agnode_t *_node;
};

#endif // QGVNODE_H
