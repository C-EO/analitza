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

#include "plotsmodel.h"

#include <surface.h>
#include <planecurve.h>
#include <spacecurve.h>

#include "analitza/analyzer.h"
#include "analitza/variables.h"
#include "analitza/localize.h"

#include <QDebug>
#include <QPixmap>
#include <QFont>
#include <QIcon>
#include <KIcon>
// #include <kcategorizedsortfilterproxymodel.h>

Q_DECLARE_METATYPE(PlotItem*);

PlotsModel::PlotsModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_itemCanCallModelRemoveItem(true)
{
}

PlotsModel::~PlotsModel()
{
    qDeleteAll(m_items);
    m_items.clear();
}

Qt::ItemFlags PlotsModel::flags(const QModelIndex & index) const
{
    if(index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    else
        return 0;
}

QVariant PlotsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal) {
        switch(section) 
        {
            case 0: return i18nc("@title:column", "Name");
            case 1: return i18nc("@title:column", "Plot");
        }
    }
    
    return QVariant();
}
QVariant PlotsModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row()>=m_items.count())
        return QVariant();

    PlotItem *tmpcurve = m_items.at(index.row());
    
    switch(role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch(index.column()) 
            {
                case 0: return tmpcurve->name();
                case 1: return tmpcurve->expression().toString();
            }
        case Qt::DecorationRole:
            if(index.column()==0)
            {
                QPixmap ico(16, 16);
                ico.fill(tmpcurve->color());
                return QIcon(ico);
            } 
            else 
                return QIcon::fromTheme(tmpcurve->iconName());
        case Qt::ToolTipRole:
            return tmpcurve->name();
        case Qt::StatusTipRole:
            return tmpcurve->typeName();
        case Qt::CheckStateRole:
            if(index.column()==0) 
                return tmpcurve->isVisible() ? Qt::Checked : Qt::Unchecked;
            break;
        case DimensionRole:
            return int(tmpcurve->spaceDimension());
        case PlotRole:
            return qVariantFromValue<PlotItem*>(tmpcurve);
    }

    
    return QVariant();
}

bool PlotsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) return false;

    switch(role)
    {
        case Qt::EditRole:
            switch(index.column()) 
            {
                case 0:
                {
                    if (m_items[index.row()]->name() == value.toString()) return false;
                    
                    m_items[index.row()]->setName(value.toString());
                    emit dataChanged(index, index);
                    return true;
                }
                case 1: //exp
                {
                    if (m_items[index.row()]->expression().toString() == value.toString()) return false;

                    if (FunctionGraph::canDraw(Analitza::Expression(value.toString()), m_items[index.row()]->spaceDimension()))
                    {
                        //TODO GSOC todo por el momento debemos hacer un typcast a functiongraph pues es el unico hijo de plotitem
                        FunctionGraph *fg = static_cast<FunctionGraph*>(m_items[index.row()]);
                        fg->setExpression(Analitza::Expression(value.toString()), m_items[index.row()]->spaceDimension());
                        emit dataChanged(index, index);
                        return true;
                    }
                    return false;
                }
            }
        case Qt::CheckStateRole:
        {
            m_items[index.row()]->setVisible(value.toBool());
            emit dataChanged(index, index);
            return true;
        }

    }
     
    return false;
}

int PlotsModel::rowCount(const QModelIndex & parent) const
{
    if(parent.isValid())
        return 0;
    else
        return m_items.size();
}

int PlotsModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}


bool PlotsModel::removeRows(int row, int count, const QModelIndex& parent)
{
    Q_ASSERT(row<m_items.size());
    if(parent.isValid())
        return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);

    for (int i = 0; i < count; ++i) 
    {
        PlotItem *tmpcurve = m_items[row];

        m_itemCanCallModelRemoveItem = false;

        if (!tmpcurve->m_inDestructorSoDontDeleteMe)
        {
            delete tmpcurve;
        }

        m_itemCanCallModelRemoveItem = true;

        m_items.removeAt(row);
    }

    endRemoveRows();
    
    return true;
}

void PlotsModel::addPlot(PlotItem* it)
{
    Q_ASSERT(it);

    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());

    it->setModel(this);
    m_items.append(it);

    endInsertRows();
}
