/***************************************************************************
 *   Copyright (C) 2020 by pilar                                           *
 *   pilarb@unex.es                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PAINTER_H
#define PAINTER_H

#include <QtCore>

namespace painter{
	extern void borrar(uchar * img, uchar color);
    extern void modoLibre(uchar * img, int cI, int fI, int grosor, uchar color);
    extern void linea(uchar * img, int c1, int f1, int c2, int f2, int grosor, uchar color);
    extern void rectangulo(uchar * img, int c1, int f1, int c2, int f2, int grosor, uchar color);
    extern void circulo(uchar * img, int c_c, int f_c, int r, int grosor, uchar color);
	extern void rellenar(uchar * img, int x, int y, uchar color);
    extern void copiar(uchar * img, uchar * buffer, int c, int f, int w, int h);
    extern void cortar(uchar * img, uchar * buffer, int c, int f, int w, int h, uchar cFondo);
    extern void pegar(uchar * img, uchar * buffer, int c, int f, int w, int h);
    extern void pegarEspecial(uchar * img, uchar * buffer, int c, int f, int w, int h, uchar color);
}

#endif
