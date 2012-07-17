/*************************************************************************************
 *  Copyright (C) 2007-2009 by Aleix Pol <aleixpol@kde.org>                          *
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

#ifndef FUNCTION_H
#define FUNCTION_H

#include <QRect>
#include <QLine>
#include <QPair>
#include <analitza/expression.h>
#include "analitzaguiexport.h"
#include <QPen>

namespace Analitza
{
class Analyzer;
class Variables;
}

/**
 *	Calculates 2D functions.
 *	@author Aleix Pol i Gonzalez
 */

struct FunctionImpl;

class ANALITZAGUI_EXPORT Function
{
	public:
		/** Defines a function axe type. */
		enum Axe { Cartesian=0, Polar};
		
		/** Constructor. Create an empty function. */
		Function();
		
		/** Copy constructor. */
		Function(const Function& f);
		
		/** Constructor. Creates a new function.
			@param name the function name.
			@param newExp the function expression tree.
			@param pen the pen used to paint the function.
		*/
		Function(const QString& name, const Analitza::Expression& newExp, Analitza::Variables* v, const QPen& m_pen,
				double uplimit, double downlimit);
		
		/** Destructor. */
		~Function();
		
		/** Defines a new function behaviour.
			@param viewport sets the coordinates the function will fit to.
		*/
		void update_points(const QRectF& viewport);
		
		/** @param resolution sets how many points will the function have. */
		void setResolution(unsigned int resolution);
		
		uint resolution() const;
		
		/** Retrieves the color of the function. */
		QColor color() const { return m_pen.color(); }
		
		/** Sets the color of the function. */
		void setColor(const QColor& newColor) { m_pen.setColor(newColor); }
		
		/** Sets whether the function has to be shown. */
		void setShown(bool newShow) { m_show=newShow; }
		
		/** Returns whether the function can be drawn. */
		bool isShown() const;
		
		/** Equal operator. */
		bool operator==(const Function& f) const { return f.m_name==m_name; }
		
		/** Retrieves the function's name. */
		QString name() const { return m_name; }
		
		/** Sets @p newName the new name of the function. */
		void setName(const QString &newName) { m_name = newName; }
		
		/** Copies a function */
		Function operator=(const Function& f);
		
		/** Returns the type of axe that the function has. */
		Axe axeType() const;
		
		/** Returns the slope of the current function in the point @p p. */
		QLineF derivative(const QPointF& p) const;
		
		/** Retrieves the calculated points of the function. */
		const QVector<QPointF>& points() const;
		
		QPair<QPointF, QString> calc(const QPointF& dp);
		
		/** Queries if it is a correct function. */
		bool isCorrect() const;
		
		/** @returns an icon name related to the type of the function. */
		QString icon() const;
		
		QStringList errors() const;
		
		const Analitza::Expression& expression() const;
		
		QList<int> jumps() const;
		
		bool allDisconnected() const;
		
	private:
		FunctionImpl* m_function;
		Analitza::Expression m_expression;
		bool m_show;
		QPen m_pen;
		QString m_name;
		QStringList m_err;
};

#endif
