/*************************************************************************************
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

#include "plotsdictionarymodel.h"
#include "plotsmodel.h"
#include <analitza/expression.h>
#include <analitza/expressionstream.h>
#include <KStandardDirs>
#include <KLocalizedString>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotsfactory.h>
#include <QFile>

using namespace Analitza;

PlotsDictionaryModel::PlotsDictionaryModel(QObject* parent)
    : QStandardItemModel(parent)
    , m_currentItem(-1)
{
    setHorizontalHeaderLabels(QStringList() << i18nc("@title:column", "Name"));
    
    createDictionary("Dictionary1", "libanalitza/data/plots/basic_curves.plots");
    createDictionary("Conics", "libanalitza/data/plots/conics.plots");
    createDictionary("Polar", "libanalitza/data/plots/polar.plots");
}

PlotsDictionaryModel::~PlotsDictionaryModel()
{}

void PlotsDictionaryModel::createDictionary(const QString& title, const QString& file)
{
    QString localurl = KStandardDirs::locate("data", file);

    QFile device(localurl);

    if (device.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&device);
        Analitza::ExpressionStream s(&stream);
        
        while(!s.atEnd()) {
            Analitza::Expression expression(s.next());
            Q_ASSERT(expression.isCorrect());
            Q_ASSERT(!expression.name().isEmpty());
            QStringList comments = expression.comments();

            QStandardItem* item = new QStandardItem;
            item->setText(expression.name());
            if(!comments.isEmpty())
                item->setToolTip(i18nc("dictionary", comments.first().trimmed().toUtf8())); //see Messages.sh for more info
            item->setData(expression.toString(), ExpressionRole);
            item->setData(title, TitleRole);
            item->setData(localurl, FileRole);
            appendRow(item);
        }
    } else
        qWarning() << "couldn't open" << localurl;
}

PlotsModel* PlotsDictionaryModel::plotModel()
{
    if(!m_plots) {
        m_plots = new PlotsModel(this);
        updatePlotsModel();
    }
    return m_plots;
}

int PlotsDictionaryModel::currentRow() const
{
    return m_currentItem;
}

void PlotsDictionaryModel::setCurrentRow(int row)
{
    if(row == m_currentItem)
        return;
    m_currentItem = row;
    if(m_plots)
        updatePlotsModel();
}

void PlotsDictionaryModel::updatePlotsModel()
{
    Q_ASSERT(m_plots);
    m_plots->clear();
    if(m_currentItem<0)
        return;
    
    QModelIndex idx = index(m_currentItem, 0);
    Analitza::Expression exp(idx.data(ExpressionRole).toString());
    PlotBuilder req = PlotsFactory::self()->requestPlot(exp, Dim2D);
//     qDebug() << exp.toString();
    Q_ASSERT(req.canDraw());
    m_plots->addPlot(req.create(Qt::blue, idx.data(Qt::DisplayRole).toString()));
}

void PlotsDictionaryModel::setCurrentIndex(const QModelIndex& idx)
{
    setCurrentRow(idx.row());
}
