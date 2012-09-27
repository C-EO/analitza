/*************************************************************************************
 *  Copyright (C) 2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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

#include "private/abstractsurface.h"
// #include "private/surfacefactory.h"
#include "private/functiongraphfactory.h"

#include <analitza/localize.h>
#include <analitza/value.h>
#include <analitza/vector.h>

#include "private/utils/marchingcubes.h"

class ImplicitSurf : public AbstractSurface , public MarchingCubes/*, static class? better macros FooClass*/
{
public:
    ImplicitSurf(const Analitza::Expression& e, Analitza::Variables* v);

    TYPE_NAME(I18N_NOOP("Implicit Surface"))
    EXPRESSION_TYPE(Analitza::ExpressionType(Analitza::ExpressionType::Lambda)
        .addParameter(Analitza::ExpressionType(Analitza::ExpressionType::Value))
        .addParameter(Analitza::ExpressionType(Analitza::ExpressionType::Value))
        .addParameter(Analitza::ExpressionType(Analitza::ExpressionType::Value))
        .addParameter(Analitza::ExpressionType(Analitza::ExpressionType::Value)))
    COORDDINATE_SYSTEM(Cartesian)
    PARAMETERS(QStringList("x") << "y" << "z")
    ICON_NAME("draw-square-inverted-corners")
    EXAMPLES(QStringList())

    //Own
    virtual ~ImplicitSurf() {}
    void update(const QVector3D & oppositecorner1, const QVector3D & oppositecorner2);
    
    double evalScalarField(double x, double y, double z);
};

double ImplicitSurf::evalScalarField(double x, double y, double z)
{
    arg("x")->setValue(x);
    arg("y")->setValue(y);
    arg("z")->setValue(z);
    
    return analyzer->calculateLambda().toReal().value();
}


ImplicitSurf::ImplicitSurf(const Analitza::Expression& e, Analitza::Variables* v): AbstractSurface(e, v)
{}

void ImplicitSurf::update(const QVector3D & oppositecorner1, const QVector3D & oppositecorner2)
{
    vertices.clear();
    normals.clear();
    indexes.clear();
    
    buildGeometry();

    vertices << MarchingCubes::_vertices;
    normals << MarchingCubes::_normals;
    indexes << MarchingCubes::_indexes;
}

REGISTER_SURFACE(ImplicitSurf)
