/*************************************************************************************
 *  Copyright (C) 2012-2013 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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

#include "quadtree.h"
#include "mathutils.h"

using namespace Analitza;

MathUtils::QuadTree::QuadTree(double x, double y, double size)
    : x(x)
    , y(y)
    , size(size)
    , nodes({0})
{
}

MathUtils::QuadTree::~QuadTree()
{
    for (int i = 0; i < 4; ++i)
        delete nodes[i];
}



// 
// 
// Square::Square(double x, double y, double hEdge)
//     : QRectF(QPointF(x-hEdge, y-hEdge), QPointF(x+hEdge, y+hEdge))
// {
// 
// }
// 
// Square::Square(const QPointF& c, double hEdge)
//     : QRectF(QPointF(c.x()-hEdge, c.y()-hEdge), QPointF(c.x()+hEdge, c.y()+hEdge))
// { }
// 
// void Square::setCeter(const QPointF& c)
// {
//     QSizeF s=size();
//     double he = halfEdge();
// 
//     setTopLeft(QPointF(c.x()-he, c.y()-he));
//     setSize(s);
// }
// 
// void Square::setCenter(double x, double y)
// {
//     QSizeF s=size();
//     double he = halfEdge();
// 
//     setTopLeft(QPointF(x-he, y-he));
//     setSize(s);
// }
// 
// double Square::halfEdge() const
// {
//         return width()*0.5;
// }
// 
// 
// void Square::setHalfEdge(double he)
// {
//     QPointF c = center();
// 
//     setTopLeft(QPointF(c.x()-he, c.y()-he));
//     setBottomRight(QPointF(c.x()+he, c.y()+he));
// }
// 
// 
// 
// Quadtree::Quadtree(double largo_mundo) {
//     raiz = new QNode;
// 
//     raiz->cubo.setCenter(largo_mundo/2,largo_mundo/2);
// 
//     raiz->cubo.setHalfEdge(largo_mundo/2);
//     for(unsigned int i=0; i<8; i++) {
//         raiz->nodos[i]=NULL;
//     }
// }
// 
// Quadtree::Quadtree(Square cubo) {
//     raiz = new QNode;
//     raiz->cubo = cubo;
//     for(unsigned int i=0; i<8; i++) {
//         raiz->nodos[i]=NULL;
//     }
// }
// Quadtree::~Quadtree() {
//     borrar_rec(raiz);
// }
// 
// void Quadtree::inicializar_nodos(QNode* padre)
// {
//     double hhedge = padre->cubo.halfEdge()/2;
// 
//     //2 a la dim
//     for(unsigned int i=0; i<4; i++) {
//         padre->nodos[i] = new QNode;
//         padre->nodos[i]->cubo.setHalfEdge(hhedge);
//         for(unsigned int j=0; j<4; j++) {
//             padre->nodos[i]->nodos[j]=NULL;
//         }
//     }
// 
//     double x = padre->cubo.center().x();
//     double y = padre->cubo.center().y();
//     padre->nodos[0]->cubo.setCenter(x-hhedge, y-hhedge);
//     padre->nodos[1]->cubo.setCenter(x-hhedge, y+hhedge);
//     padre->nodos[2]->cubo.setCenter(x+hhedge, y-hhedge);
//     padre->nodos[3]->cubo.setCenter(x+hhedge, y+hhedge);
// }
// 
// void Quadtree::borrar_rec(QNode* nodo) {
//     if(nodo == NULL) {
//         return;
//     }
//     for(unsigned int i=0; i<4; i++) {
//         borrar_rec(nodo->nodos[i]);
//     }
//     delete nodo;
// }
// 
// 
// void Quadtree::crear_rec(QNode* nodo, unsigned int nivel_actual, unsigned int nivel_max) {
//     if(nivel_actual > nivel_max) {
//         return;
//     }
//     inicializar_nodos(nodo);
//     for(unsigned int i=0; i<4; i++) {
//         crear_rec(nodo->nodos[i],nivel_actual+1,nivel_max);
//     }
// }
// 
// QNode* Quadtree::get_raiz() {
//     return raiz;
// }
// 
// void Quadtree::crearNivel(unsigned int nivel) {
//     crear_rec(raiz,0,nivel);
// }
// 
// void Quadtree::bajarNivel(QNode* nodo) {
//     inicializar_nodos(nodo);
// }
// 
// void Quadtree::borrarHijos(QNode* padre) {
//     for(unsigned int i=0; i<4; i++) {
//         borrar_rec(padre->nodos[i]);
//         padre->nodos[i] = NULL;
//     }
// }