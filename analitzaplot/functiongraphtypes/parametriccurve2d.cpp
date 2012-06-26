/*************************************************************************************
 *  Copyright (C) 2007-2009 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2010-2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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


#include "functiongraph.h"
#include "functiongraphfactory.h"

#include <QRectF>
#include "analitza/value.h"
#include "analitza/vector.h"


#include <QDebug>
#include <analitza/localize.h>
#include "functionutils.h"

using Analitza::Expression;
using Analitza::ExpressionType;
using Analitza::Variables;
using Analitza::Object;
using Analitza::Cn;

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
static const double pi=M_PI;


namespace
{
    /// The @p p1 and @p p2 parameters are the last 2 values found
    /// @p next is the next value found
    /// @returns whether we've found the gap

    bool traverse(double p1, double p2, double next)
    {
        static const double delta=3;
        double diff=p2-p1, diff2=next-p2;
        bool ret=false;

        if(diff>0 && diff2<-delta)
            ret=true;
        else if(diff<0 && diff2>delta)
            ret=true;

        return ret;
    }
}



class ANALITZAPLOT_EXPORT FunctionParametric : public AbstractPlaneCurve
{
public:
    TYPE_NAME("FunctionParametricVecto")
    EXPRESSION_TYPE(Analitza::ExpressionType(Analitza::ExpressionType::Lambda).addParameter(Analitza::ExpressionType(Analitza::ExpressionType::Value)).addParameter(Analitza::ExpressionType(Analitza::ExpressionType::Vector, Analitza::ExpressionType(Analitza::ExpressionType::Value), 2)))
    COORDDINATE_SYSTEM(Cartesian)
    ARGUMENTS("t")
    ICON_NAME("newparametric")
    EXAMPLES("t->vector {t,t**2}")    
    
    
    FunctionParametric(const Analitza::Expression &functionExpression, Analitza::Variables *variables);
    ~FunctionParametric()
    {
    }

    void update(const QRect& viewport);
    
    QPair<QPointF, QString> calc(const QPointF &mousepos);
    QLineF derivative(const QPointF &mousepos) const;
    
    //
    

private:
    Cn findTValueForPoint(const QPointF& p);

};

FunctionParametric::FunctionParametric(const Analitza::Expression &functionExpression, Analitza::Variables *variables)
:AbstractPlaneCurve(functionExpression, variables)
{
}

// 

void FunctionParametric::update(const QRect& viewport)
{
  Q_UNUSED(viewport);
    Q_ASSERT(analyzer.expression().isCorrect());
    //if(int(resolution())==points.capacity())
    //  return;
    
    
            //TODO CACHE en intervalvalues!!!
    static QPair<double, double> c_limits = interval("t");
    
//     if ()
//     
//     static QPair<double, double> o_limits = c_limits;
    
    
    double ulimit=c_limits.second;
    double dlimit=c_limits.first;
    
    
    
    points.clear();
    jumps.clear();
    //points.reserve(resolution());

    
//  double inv_res= double((ulimit-dlimit)/resolution());
    double inv_res= 0.01; 
//  double final=ulimit-inv_res;
    
        //by percy
    QRectF vp(viewport);
    
    vp.setTop(viewport.top() - 2);
    vp.setBottom(viewport.bottom() + 2);
    vp.setLeft(viewport.left() + 2);
    vp.setRight(viewport.right() - 2);
    
    QPointF curp;
    
    arg("t")->setValue(dlimit);
    Expression res;
    
    int i = 0;
    bool jlock = false;
    
    for(double t=dlimit; t<ulimit; t+=inv_res, ++i) {
        arg("t")->setValue(t);
        res=analyzer.calculateLambda();
        
        Cn x=res.elementAt(0).toReal();
        Cn y=res.elementAt(1).toReal();
        
        curp = QPointF(x.value(), y.value());
        
        if (vp.contains(curp))
        {
            addPoint(curp);
            jlock = false;
        }
        else if (!jlock)
        {
            jumps.append(i);
            jlock = true;
        }
        
        //      objectWalker(vo);
        Q_ASSERT(res.isVector());
    }
}



//Own
QPair<QPointF, QString> FunctionParametric::calc(const QPointF &point)
{
    
    arg("t")->setValue(findTValueForPoint(point).value());
    
    Expression res=analyzer.calculateLambda();
    Analitza::Cn comp1 = res.elementAt(0).toReal();
    Analitza::Cn comp2 = res.elementAt(1).toReal();
    
    return QPair<QPointF, QString>(QPointF(comp1.value(), comp2.value()), QString());

}

QLineF FunctionParametric::derivative(const QPointF &mousepos) const
{
//    if(m_deriv)
//     {
//         double theT = findTValueForPoint(point).value();
//         
//         Analitza::Analyzer a;
//         a.setExpression(*m_deriv);
//         a.setStack(m_runStack);
//         arg("t")->setValue(theT);
//         Expression exp = a.calculateLambda();
//         
//         Analitza::Cn comp1 = exp.elementAt(0).toReal();
//         Analitza::Cn comp2 = exp.elementAt(1).toReal();
// 
//         double m = comp2.value()/comp1.value();
// 
//         return FunctionUtils::slopeToLine(m);
//     }
//     else
//         return QLineF();

return QLineF();
    
}

Cn FunctionParametric::findTValueForPoint(const QPointF& p)
{
    //TODO esto no esta bien desde el principio no funciona bien no es robusto no funciona bien 
    // MEJORAR el algoritmo
    
//     Q_ASSERT(analyzer.isCorrect() && analyzer.expression().lambdaBody().isVector());
//     
//     Analitza::Analyzer f(analyzer.variables());
//     f.setExpression(analyzer.expression().lambdaBody().elementAt(0));
//     f.setExpression(f.dependenciesToLambda());
//     f.setStack(m_runStack);
// 
//     Analitza::Analyzer df(analyzer.variables());
//     df.setExpression(f.derivative("t"));
//     df.setStack(m_runStack);
// 
// //TODO
// //    Analitza::Analyzer g(analyzer.variables());
// //    g.setExpression(Analitza::Expression("t->" + analyzer.expression().lambdaBody().elementAt(1).toString() + "+" + QString::number(-1.0*point.y()), false));
// //    g.refExpression()->parameters()[0]->value() = vx;
// //
// //    Analitza::Analyzer dg(analyzer.variables());
// //    dg.setExpression(g.derivative("t"));
// //    dg.refExpression()->parameters()[0]->value() = vx;
// 
//     const int MAX_I = 256;
//     const double E = 0.0001;
//     double t0 = 1.0;
//     double t = t0;
//     double error = 1000.0;
//     int i = 0;
// 
//     while (true)
//     {
//         arg("t")->setValue(t0);
// 
//         double r = f.calculateLambda().toReal().value()-p.x();
//         double d = df.calculateLambda().toReal().value();
// 
//         i++;
//         t = t0 - r/d;
// 
//         if ((error < E) || (i > MAX_I))
//             break;
// 
//         error = fabs(t - t0);
//         t0 = t;
//     }
// 
// //TODO
// //    t0 = 1.0;
// //    t = t0;
// //    error = 1000.0;
// //    i = 0;
// //
// //    while (true)
// //    {
// //        arg("t")->setValue(t0);
// //
// //        double r = g.calculateLambda().toReal().value();
// //        double d = dg.calculateLambda().toReal().value();
// //
// //        i++;
// //        t = t0 - r/d;
// //
// //        if ((error < E) || (i > MAX_I))
// //            break;
// //
// //        error = fabs(t - t0);
// //        t0 = t;
// //    }
//     return Cn(t);

return Cn(0.0);
}


REGISTER_PLANECURVE(FunctionParametric)



