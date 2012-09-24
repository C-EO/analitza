/*************************************************************************************
 *  Copyright (C) 2012 by Aleix Pol <aleixpol@kde.org>                               *
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

#include "expressionstream.h"

using namespace Analitza;

ExpressionStream::ExpressionStream(QTextStream* dev)
	: m_dev(dev)
{}

bool ExpressionStream::atEnd() const
{
	return m_dev->atEnd();
}

Expression ExpressionStream::next()
{
	QString line;
	for(; !atEnd(); ) {
		line += m_dev->readLine();
		if(Expression::isCompleteExpression(line) || m_dev->atEnd()) {
			break;
		}
	}
	return Analitza::Expression(line, Expression::isMathML(line));
}