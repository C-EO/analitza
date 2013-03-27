/*************************************************************************************
 *  Copyright (C) 2007-2009 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2013 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
 *                                                                                   * 
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include "mathutils.h"

#include <cmath>

using namespace Analitza;

ExpressionType MathUtils::createFunctionType(int fromDim, int toDim)
{
    Q_ASSERT(fromDim > 0);
    Q_ASSERT(toDim > 0);
    
    ExpressionType ret(ExpressionType::Lambda);
    
    for (int i = 0; i < fromDim; ++i)
        ret.addParameter(ExpressionType(ExpressionType::Value));
    
    if (toDim == 1)
        ret.addParameter(ExpressionType(ExpressionType::Value));
    else
        ret.addParameter(ExpressionType(ExpressionType::Vector, ExpressionType(ExpressionType::Value), toDim));
    
    return ret;
}

void MathUtils::polarToCartesian(double radial, double polar, double &x, double &y)
{
    x = radial*cos(polar);
    y = radial*sin(polar);
}

void MathUtils::cartesianToPolar(double x, double y, double &radial, double &polar)
{
    radial = sqrt(x*x+y*y);
    polar = atan2(y,x);
}

void MathUtils::cylindricalToCartesian(double radial, double polar, double height, double &x, double &y, double &z)
{
    x = radial*cos(polar);
    y = radial*sin(polar);
    z = height;
}

void MathUtils::sphericalToCartesian(double radial, double azimuth, double polar, double &x, double &y, double &z)
{
    x = radial*cos(azimuth)*sin(polar);
    y = radial*sin(azimuth)*sin(polar);
    z = radial*cos(polar);
}

bool MathUtils::traverse(double p1, double p2, double next)
{
    static const double delta=3;
    double diff=p2-p1, diff2=next-p2;
    
    bool ret = (diff>0 && diff2<-delta) || (diff<0 && diff2>delta);
    
    return ret;
}

QLineF MathUtils::slopeToLine(const double &der)
{
    double arcder = atan(der);
    const double len=6.*der;
    QPointF from, to;
    from.setX(len*cos(arcder));
    from.setY(len*sin(arcder));

    to.setX(-len*cos(arcder));
    to.setY(-len*sin(arcder));
    return QLineF(from, to);
}

QLineF MathUtils::mirrorXY(const QLineF& l)
{
    return QLineF(l.y1(), l.x1(), l.y2(), l.x2());
}
