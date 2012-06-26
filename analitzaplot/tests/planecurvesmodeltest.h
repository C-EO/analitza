/*************************************************************************************
 *  Copyright (C) 2007 by Aleix Pol <aleixpol@kde.org>                               *
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

#ifndef ANALITZATEST_H
#define ANALITZATEST_H

#include <QObject>

/**
	@author Aleix Pol
*/
class FunctionGraph;
namespace Analitza { class Variables; }

class FunctionTest : public QObject
{
Q_OBJECT
	public:
		FunctionTest(QObject *parent = 0);
		~FunctionTest();

	private slots:
		void initTestCase();

		//TODO gsoc2012
// 		void testCorrect();
// 		void testCorrect_data();
//
// 		void testCopy();
// 		void testCopy_data();
//
// 		void testJumps();
// 		void testJumps_data();

		void cleanupTestCase();
	private:
		Analitza::Variables* m_vars;
};

#endif
