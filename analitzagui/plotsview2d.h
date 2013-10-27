/*************************************************************************************
 *  Copyright (C) 2007-2008 by Aleix Pol <aleixpol@kde.org>                          *
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

#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QModelIndex>

#include "analitzaguiexport.h"
#include <analitzaplot/plotter2d.h>


class QItemSelectionModel;

namespace Analitza
{
class PlotsModel;

/**
 * \class PlotsView2D
 * 
 * \ingroup AnalitzaGUIModule
 *
 * \brief Widget that allows visualization of 2D plots.
 *
 * This class lets you create a widget that can draw multiple 2D graphs. This widget 
 * use Plotter2D as a backend.
 */

class ANALITZAGUI_EXPORT PlotsView2D : public QWidget, public Plotter2D
{
Q_OBJECT
Q_PROPERTY(bool squares READ squares WRITE setSquares)
public:
    /** The graph mode will especify the selection mode we are using at the moment */
    enum GraphMode {
        None=0,     /**< Normal behaviour */
        Pan,        /**< Panning, translates the viewport. */
        Selection   /**< There is a rectangle delimiting a region, for zooming. */
    };
    
    enum Format { PNG, SVG };
    
    /** Constructor. Constructs a new Graph2D. */
    PlotsView2D(QWidget* parent = 0);
    
    ~PlotsView2D();
    
    QSize sizeHint() const { return QSize(100, 100); }
    
    /** Saves the graphs to a file located at @p path. */
    bool toImage(const QString& path, Format f);
    
    /** Returns the viewing port */
    QRectF definedViewport() const;
    
    void setSelectionModel(QItemSelectionModel* selection);

	virtual void showEvent(QShowEvent* ev);

public slots:
    /** Marks the image as dirty and repaints everything. */
    void forceRepaint() { valid=false; repaint(); }

    /** Sets the viewport to a default viewport. */
    void resetViewport() { setViewport(defViewport); }
    
    /** Zooms in to the Viewport center */
    void zoomIn();
    
    /** Zooms out */
    void zoomOut();
        
    /** Returns whether it has a little border frame. */
    bool isFramed() const { return m_framed; }
    
    /** Sets whether it has a little border frame. */
    void setFramed(bool fr) { m_framed=fr; }
    
    /** Returns whether it is a read-only widget. */
    bool isReadOnly() const { return m_readonly; }
    
    /** Sets whether it is a read-only widget. */
    void setReadOnly(bool ro);
    
    void snapshotToClipboard();
    
    //exposed from plotter2d as slots...
    void setXAxisLabel(const QString &label) { Plotter2D::setXAxisLabel(label); }
    void setYAxisLabel(const QString &label) { Plotter2D::setYAxisLabel(label); }
    void updateGridColor(const QColor &color) { Plotter2D::updateGridColor(color); }
    void updateTickScale(QString s, qreal v, int n, int d) { Plotter2D::updateTickScale(s,v,n,d); }
    void setTicksShown(QFlags<Qt::Orientation> o) { Plotter2D::setTicksShown(o); }
    void setAxesShown(QFlags<Qt::Orientation> o) { Plotter2D::setAxesShown(o); }
    
private slots:
    void updateFuncs(const QModelIndex & parent, int start, int end); //update al insertar itesm
    void updateFuncs(const QModelIndex& start, const QModelIndex& end); //update al setdata 
    void addFuncs(const QModelIndex & parent, int start, int end);
    void removeFuncs(const QModelIndex & parent, int start, int end);
    void changeViewport(const QRectF& vp) { setViewport(vp); }
    
signals:
    /** Emits a status when it changes. */
    void status(const QString &msg);
    
    void viewportChanged(const QRectF&);
    
private:
    virtual void viewportChanged();
    virtual int currentFunction() const;
    virtual void modelChanged();
    
    //painting
    QPixmap buffer;
    bool valid;
    QPointF mark;
    QPoint cursorPos;
    
    //events
    void paintEvent( QPaintEvent * );
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent * e );
    void wheelEvent(QWheelEvent *e);
    void resizeEvent(QResizeEvent *);

    GraphMode mode;
    QPoint press; QPoint last;
    
    //presentation
    QPointF ant;
    QRectF defViewport;
    void drawFunctions(QPaintDevice*);
        
    void sendStatus(const QString& msg) { emit status(msg); }
    bool m_framed;
    bool m_readonly;
    QString m_posText;
    QItemSelectionModel* m_selection;
    QAbstractItemModel *m_currentModel; // use this pointer to disconnect signals when change the model
};

}

#endif
