/*************************************************************************************
 *  Copyright (C) 2015 by Aleix Pol <aleixpol@kde.org>                               *
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

#ifndef GRAPH3DITEM_H
#define GRAPH3DITEM_H

#include <QQuickFramebufferObject>
#include "plotter3d_es.h"

class Graph3DItem;

class Plotter3DRenderer : public QObject, public Analitza::Plotter3DES
{
public:
    Plotter3DRenderer(Graph3DItem* item);

    int currentPlot() const override { return 0; }
    void modelChanged() override {}
    void renderGL() override;

private:
    const Graph3DItem* m_item;
};

class Graph3DItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel)
    public:
        Graph3DItem(QQuickItem* parent = Q_NULLPTR);
        ~Graph3DItem();

        QAbstractItemModel* model() const;
        void setModel(QAbstractItemModel* model);

        QQuickFramebufferObject::Renderer * createRenderer() const override;

        Q_SCRIPTABLE QStringList addFunction(const QString& expression, Analitza::Variables* vars=0);
        Q_SCRIPTABLE void rotate(qreal x, qreal y);
        Q_SCRIPTABLE void scale(qreal s);

    private:
        Plotter3DRenderer *m_plotter;
};

#endif
