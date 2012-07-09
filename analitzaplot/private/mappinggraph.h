/*************************************************************************************
 *  Copyright (C) 2007-2011 by Aleix Pol <aleixpol@kde.org>                          *
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

#ifndef MAPPINGGRAPH_H
#define MAPPINGGRAPH_H

#include "analitzaplot/mathutils.h"

#include <QStringList>
#include <QColor>

class VisualItemsModel;

class ANALITZAPLOT_EXPORT VisualItem
{
friend class VisualItemsModel; // use case: if this item gets deleted by "delete someitem" the model will emit/exec removeitem signal

public:
    explicit VisualItem(const QString &name, const QColor& col);
    virtual ~VisualItem();
    
    virtual Analitza::Variables * variables() const = 0;
    virtual void setVariables(Analitza::Variables *variables) = 0;
    
    virtual const QString typeName() const = 0;
    virtual const Analitza::Expression & expression() const = 0;

    QString name() const { return m_name; }
    void setName(const QString &newName) { m_name = newName; }
    virtual QString iconName() const = 0;
    QColor color() const { return m_color; }
    void setColor(const QColor& newColor) { m_color = newColor; }
    virtual QStringList examples() const = 0;

    virtual int spaceDimension() const = 0; // dim of the space where the item can be drawn ... IS NOT the variety dimension
    virtual CoordinateSystem coordinateSystem() const = 0;
    PlotStyle plotStyle() { return m_plotStyle; }
    void setPlotStyle(PlotStyle ps) { m_plotStyle = ps; }
    bool isVisible() const { return m_graphVisible; }
    void setVisible(bool f) { m_graphVisible = f; }

    virtual QStringList errors() const = 0;
    virtual bool isCorrect() const = 0;
    
protected:
    VisualItem() {}
    VisualItem(const VisualItem &other) {}

//     VisualItemsModel * model() const;

    void emitDataChanged();

private:
    void setModel(VisualItemsModel *m);
    
    //gui
    QString m_name;
    QColor m_color;

    //graphDescription    
    PlotStyle m_plotStyle;
    bool m_graphVisible;
    
    //model expose item as write pointr ... so this will fix some situations (delete external, etc)
    VisualItemsModel *m_model;
    bool m_inDestructorSoDontDeleteMe; // lock para evitar que el removeitem del model llame al destructor de este item y se generen llamadas recursivas
};

#endif // MAPPINGGRAPH_H