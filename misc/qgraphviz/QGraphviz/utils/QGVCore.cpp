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

#include "QGVCore.h"
#include <QDebug>

qreal QGVCore::graphHeight(Agraph_t *graph)
{
    //Hauteur totale du graphique (permet d'effectuer le calcul inverse des coordonnées)
    return GD_bb(graph).UR.y;
}

QPointF QGVCore::toPoint(pointf p, qreal gheight)
{
    //Le repere Y commence du bas dans graphViz et du haut pour Qt !
    return QPointF(p.x, gheight - p.y);
}

QPointF QGVCore::toPoint(point p, qreal gheight)
{
    //Le repere Y commence du bas dans graphViz et du haut pour Qt !
    return QPointF(p.x, gheight - p.y);
}

QPointF QGVCore::centerToOrigin(const QPointF &p, qreal width, qreal height)
{
    //L'origine d'un objet est le centre dans graphViz et du haut gauche pour Qt !
    return QPointF(p.x() - width/2, p.y() - height/2);
}

QPolygonF QGVCore::toPolygon(const polygon_t *poly, qreal width, qreal height)
{
    if (poly->peripheries != 1)
        qWarning("unsupported number of peripheries %d", poly->peripheries);

    const int sides = poly->sides;
    const pointf* vertices = poly->vertices;

    QPolygonF polygon;
    for (int side = 0; side < sides; side++)
        polygon.append(QPointF(vertices[side].x + width/2, vertices[side].y + height/2));
    return polygon;
}

QPainterPath QGVCore::toPath(const char *type, const polygon_t *poly, qreal width, qreal height)
{
    QPainterPath path;
    if ((strcmp(type, "rectangle") == 0) ||
        (strcmp(type, "box") == 0) ||
        (strcmp(type, "hexagon") == 0) ||
        (strcmp(type, "polygon") == 0) ||
        (strcmp(type, "diamond") == 0))
    {
        QPolygonF polygon = toPolygon(poly, width, height);
        polygon.append(polygon[0]);
        path.addPolygon(polygon);
    }
    else if ((strcmp(type, "ellipse") == 0) ||
            (strcmp(type, "circle") == 0))
    {
        QPolygonF polygon = toPolygon(poly, width, height);
        path.addEllipse(QRectF(polygon[0], polygon[1]));
    }
    else
    {
        qWarning("unsupported shape %s", type);
    }
    return path;
}

QPainterPath QGVCore::toPath(const splines *spl, qreal gheight)
{
    QPainterPath path;
    if((spl->list != 0) && (spl->list->size%3 == 1))
    {
        bezier bez = spl->list[0];
        //If there is a starting point, draw a line from it to the first curve point
        if(bez.sflag)
        {
            path.moveTo(toPoint(bez.sp, gheight));
            path.lineTo(toPoint(bez.list[0], gheight));
        }
        else
            path.moveTo(toPoint(bez.list[0], gheight));

        //Loop over the curve points
        for(int i=1; i<bez.size; i+=3)
            path.cubicTo(toPoint(bez.list[i], gheight), toPoint(bez.list[i+1], gheight), toPoint(bez.list[i+2], gheight));

        //If there is an ending point, draw a line to it
        if(bez.eflag)
            path.lineTo(toPoint(bez.ep, gheight));
    }
    return path;
}

Qt::BrushStyle QGVCore::toBrushStyle(const QString &style)
{
    if(style == "filled")
        return Qt::SolidPattern;
    return Qt::NoBrush;
}

Qt::PenStyle QGVCore::toPenStyle(const QString &style)
{
    if(style =="dashed")
        return Qt::DashLine;
    else if(style == "dotted")
        return Qt::DotLine;
    return Qt::SolidLine;
}

QColor QGVCore::toColor(const QString &color)
{
    return QColor(color);
}
