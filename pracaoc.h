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


#ifndef PRACAOC_H
#define PRACAOC_H

#include <ui_mainForm.h>
#include <QtCore>
#include <QLabel>
#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <iostream>
#include "painter.h"
#include <QColorDialog>


#define WIDTHIMG 640
#define HEIGHTIMG 480

using namespace Ui;

enum SelectionType { LINEA, CIRCULO, RECTANGULO };

class pracAOC:public QWidget
{
    Q_OBJECT

public:
    pracAOC();
    ~pracAOC();
		
		

private:

		QMainForm * mainWin;

		QVector<QRgb> ctable;
		QImage * qimg, qimgToShow, * qimgPenColor, * qimgBrushColor;
		uchar * img, * imgPenColor, * imgBrushColor, * bCopia;
		QLabel * imageLabel;
		unsigned char colorPenInd, colorBrushInd;

        QColorDialog * colorDialog;
		
		bool seleccion;
		int xIni, yIni, xFin, yFin;
		int xSelIni, ySelIni, xSelFin, ySelFin;		
		int zoom;
		int penWidth;
		int wCopia, hCopia;
		bool copiaFlag;
		
		SelectionType selectionDraw;
		
		void startSelection(QPoint pos);
		void updateSelectionRectangulo();
		void updateSelectionLinea();
		void updateSelectionCirculo();
		void updateSelection(QPoint pos);
		void stopSelection();

protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void closeEvent(QCloseEvent *event);

public slots:

		void paintEvent(QPaintEvent *);
		void cargar();
		void guardar();
		void borrar();
		void selectModoLibre(bool checked);
		void selectLinea(bool checked);		
		void selectRectangulo(bool checked);
		void selectCirculo(bool checked);
		void selectRellenar(bool checked);
		void selectPegar(bool checked);		
		void selectPegarEspecial(bool checked);		
		void copiar();
		void cortar();
		void changeZoom(int);
		void changePenWidth(int);
		void colorPenSelection();
		void colorBrushSelection();

};




#endif
