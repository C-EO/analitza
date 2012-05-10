/*************************************************************************************
 *  Copyright (C) 2008 by Aleix Pol <aleixpol@kde.org>                               *
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

#ifndef STRINGEXPRESSIONWRITER_H
#define STRINGEXPRESSIONWRITER_H

#include "expressionwriter.h"
#include "operator.h"

namespace Analitza
{

/**
 *	This class represents the string expression writer.
 *
 *	@author Aleix Pol <aleixpol@kde.org>  
 */

class StringExpressionWriter : public ExpressionWriter
{
	public:
		StringExpressionWriter(const Object* o);
		
		virtual QString accept(const Ci* var);
		virtual QString accept(const Cn* var);
		virtual QString accept(const Container* var);
		virtual QString accept(const Operator* var);
		virtual QString accept(const Vector* var);
		virtual QString accept(const List* l);
		virtual QString accept(const Apply* a);
		virtual QString accept(const CustomObject* c);
		
		QString result() const { return m_result; }
		
		static int weight(const Analitza::Operator* op, int size, int pos);
		static const QMap<Operator::OperatorType, QString> s_operators;
	private:
		template <class T>
			static QStringList allValues(T it, const T& itEnd, ExpressionWriter* writer);

		QString m_result;
};

}
#endif
