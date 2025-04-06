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


#include "pracaoc.h"


pracAOC::pracAOC(): QWidget()
{
	mainWin=new QMainForm();
	mainWin->setupUi(this);

    img=new uchar[WIDTHIMG*HEIGHTIMG];
	bCopia=new uchar[WIDTHIMG*HEIGHTIMG];
	
	imgBrushColor=new uchar[mainWin->currentBrushColor->width()*mainWin->currentBrushColor->height()];
	imgPenColor=new uchar[mainWin->currentPenColor->width()*mainWin->currentPenColor->height()];
	qimg = new QImage(img,WIDTHIMG,HEIGHTIMG, QImage::Format_Indexed8);

	qimgPenColor = new QImage(imgPenColor,mainWin->currentPenColor->width(),mainWin->currentPenColor->height(), QImage::Format_Indexed8);
	qimgBrushColor = new QImage(imgBrushColor,mainWin->currentBrushColor->width(),mainWin->currentBrushColor->height(), QImage::Format_Indexed8);

	for(long i=0; i<mainWin->currentPenColor->width()*mainWin->currentPenColor->height(); i++)
	{
		imgPenColor[i]=0;
		imgBrushColor[i]=7;
	}
	colorPenInd=0;
	colorBrushInd=7;
	
	for(long i=0; i<WIDTHIMG*HEIGHTIMG;i++)
	  img[i]=7;
	imageLabel = new QLabel;
	imageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	imageLabel->setPixmap(QPixmap::fromImage(*qimg));
	
    ctable.resize(256);
    colorDialog = new QColorDialog(this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);

	for(int i=0; i < 8; i++)
	{
      colorDialog->setStandardColor(i, qRgb(i*32,i*32,i*32));
	  ctable[i] = qRgb(i*32,i*32,i*32);
      colorDialog->setStandardColor(i+8, qRgb(255-i*32,0,0));
	  ctable[i+8] = qRgb(255-i*32,0,0);
      colorDialog->setStandardColor(i+16, qRgb(0,255-i*32,0));
	  ctable[i+16] = qRgb(0,255-i*32,0);
      colorDialog->setStandardColor(i+24, qRgb(0,0,255-i*32));
	  ctable[i+24] = qRgb(0,0,255-i*32);
      colorDialog->setStandardColor(i+32, qRgb(255-i*32,255-i*32,0));
	  ctable[i+32] = qRgb(255-i*32,255-i*32,0);
      colorDialog->setStandardColor(i+40, qRgb(0,255-i*32,255-i*32));
	  ctable[i+40] = qRgb(0,255-i*32,255-i*32);
	}
    colorDialog->setStandardColor(7, qRgb(255,255,255));
	ctable[7] = qRgb(255,255,255);
	
 	qimg->setColorTable(ctable);
	qimgPenColor->setColorTable(ctable);
	qimgBrushColor->setColorTable(ctable);
	
	imageLabel = new QLabel;
	imageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	zoom=1;	
	penWidth=1;
	qimgToShow=qimg->scaledToWidth(WIDTHIMG*zoom);
	imageLabel->setPixmap(QPixmap::fromImage(qimgToShow));

	mainWin->paintScrollArea->setBackgroundRole(QPalette::Dark);
 	mainWin->paintScrollArea->setWidget(imageLabel);

	connect ( mainWin->pushButtonCargar, SIGNAL (clicked()), this, SLOT( cargar() ) );
	connect ( mainWin->pushButtonGuardar, SIGNAL (clicked()), this, SLOT( guardar() ) );
	connect ( mainWin->pushButtonBorrar, SIGNAL (clicked()), this, SLOT( borrar() ) );
	connect ( mainWin->pushButtonModoLibre, SIGNAL (toggled(bool)), this, SLOT( selectModoLibre(bool) ) );	
	connect ( mainWin->pushButtonLinea, SIGNAL (toggled(bool)), this, SLOT( selectLinea(bool) ) );	
	connect ( mainWin->pushButtonRectangulo, SIGNAL (toggled(bool)), this, SLOT( selectRectangulo(bool) ) );	
	connect ( mainWin->pushButtonCirculo, SIGNAL (toggled(bool)), this, SLOT( selectCirculo(bool) ) );	
	connect ( mainWin->pushButtonRellenar, SIGNAL (toggled(bool)), this, SLOT( selectRellenar(bool) ) );	
	connect ( mainWin->pushButtonCopiar, SIGNAL (clicked()), this, SLOT( copiar() ) );
	connect ( mainWin->pushButtonCortar, SIGNAL (clicked()), this, SLOT( cortar() ) );
	connect ( mainWin->pushButtonPegar, SIGNAL (toggled(bool)), this, SLOT( selectPegar(bool) ) );		
	connect ( mainWin->pushButtonPegarEspecial, SIGNAL (toggled(bool)), this, SLOT( selectPegarEspecial(bool) ) );		
	connect ( mainWin->pushButtonPenColor, SIGNAL (clicked()), this, SLOT( colorPenSelection() ) );
	connect ( mainWin->pushButtonBrushColor, SIGNAL (clicked()), this, SLOT( colorBrushSelection() ) );
	connect ( mainWin->zoomHorizontalSlider, SIGNAL (valueChanged(int)), this, SLOT( changeZoom(int) ) );
	connect ( mainWin->penWidthSpinBox, SIGNAL (valueChanged(int)), this, SLOT( changePenWidth(int) ) );
	

	seleccion=false;
	copiaFlag=false;

}


pracAOC::~pracAOC()
{
}


void pracAOC::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	qimgPenColor->setColorTable(ctable);
	qimgBrushColor->setColorTable(ctable);
	painter.drawImage(QPoint(mainWin->currentPenColor->x(), mainWin->currentPenColor->y()), *qimgPenColor);
	painter.drawImage(QPoint(mainWin->currentBrushColor->x(), mainWin->currentBrushColor->y()), *qimgBrushColor);
	painter.end();
 	qimgToShow=qimg->scaledToWidth(WIDTHIMG*zoom);
	qimgToShow.setColorTable(ctable);
	imageLabel->setPixmap(QPixmap::fromImage(qimgToShow));

 	
}

void pracAOC::cargar()
{
	QImage qimgRead(640,480,QImage::Format_Indexed8);
	QImage qimgCRead(640,480,QImage::Format_Indexed8);
	QString fn = QFileDialog::getOpenFileName(this,"Elija la imagen a cargar",".", "Images (*.png *.xpm *.jpg)");
	if(qimgRead.load(fn))
	{
        for(int i=0; i<256 && i<qimgRead.colorTable().size(); i++)
			ctable[i]=qimgRead.colorTable()[i];

        qimgCRead=qimgRead.convertToFormat(QImage::Format_Indexed8, ctable);

        int width = ((qimgCRead.width()+3)/4)*4;

		for(int y=0; y<480 && y<qimgRead.height();y++)
			for(int x=0; x<640 && x<qimgRead.width(); x++)
                img[y*640+x]=(qimgCRead.bits())[(y*width+x)];
			
		update();
	}
}

void pracAOC::guardar()
{
	QImage qimgWrite(640,480,QImage::Format_Indexed8);
	QString fn = QFileDialog::getSaveFileName(this,"Indique el nombre del fichero",".", "Images (*.png *.xpm *.jpg)");
	qimgWrite.setColorTable(ctable);
	for(int y=0; y<480;y++)
		for(int x=0; x<640; x++)
			(qimgWrite.bits())[(y*640+x)]=img[y*640+x];

	qimgWrite.save(fn);
}

void pracAOC::borrar()
{
	painter::borrar(img, colorBrushInd);
 	update();
}

void pracAOC::selectModoLibre(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonLinea->setChecked(false);
	 mainWin->pushButtonRectangulo->setChecked(false);
	 mainWin->pushButtonCirculo->setChecked(false);
	 mainWin->pushButtonRellenar->setChecked(false);
	 mainWin->pushButtonPegar->setChecked(false);
	 mainWin->pushButtonPegarEspecial->setChecked(false);
  }
}

void pracAOC::selectLinea(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonModoLibre->setChecked(false);
	 mainWin->pushButtonRectangulo->setChecked(false);
	 mainWin->pushButtonCirculo->setChecked(false);
	 mainWin->pushButtonRellenar->setChecked(false);
	 mainWin->pushButtonPegar->setChecked(false);
	 mainWin->pushButtonPegarEspecial->setChecked(false);
  }
  
}

void pracAOC::selectRectangulo(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonModoLibre->setChecked(false);
	 mainWin->pushButtonLinea->setChecked(false);
	 mainWin->pushButtonCirculo->setChecked(false);
	 mainWin->pushButtonRellenar->setChecked(false);
	 mainWin->pushButtonPegar->setChecked(false);
	 mainWin->pushButtonPegarEspecial->setChecked(false);
  }

}

void pracAOC::selectCirculo(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonModoLibre->setChecked(false);
	 mainWin->pushButtonLinea->setChecked(false);
	 mainWin->pushButtonRectangulo->setChecked(false);
	 mainWin->pushButtonRellenar->setChecked(false);
	 mainWin->pushButtonPegar->setChecked(false);
	 mainWin->pushButtonPegarEspecial->setChecked(false);
  }

}

void pracAOC::selectRellenar(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonModoLibre->setChecked(false);
	 mainWin->pushButtonLinea->setChecked(false);
	 mainWin->pushButtonRectangulo->setChecked(false);
	 mainWin->pushButtonCirculo->setChecked(false);
	 mainWin->pushButtonPegar->setChecked(false);
	 mainWin->pushButtonPegarEspecial->setChecked(false);
  }

}

void pracAOC::selectPegar(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonModoLibre->setChecked(false);
	 mainWin->pushButtonLinea->setChecked(false);
	 mainWin->pushButtonRectangulo->setChecked(false);
	 mainWin->pushButtonCirculo->setChecked(false);
	 mainWin->pushButtonRellenar->setChecked(false);
	 mainWin->pushButtonPegarEspecial->setChecked(false);
  }

}

void pracAOC::selectPegarEspecial(bool checked)
{
  if(checked)
  {
	 mainWin->pushButtonModoLibre->setChecked(false);
	 mainWin->pushButtonLinea->setChecked(false);
	 mainWin->pushButtonRectangulo->setChecked(false);
	 mainWin->pushButtonCirculo->setChecked(false);
	 mainWin->pushButtonRellenar->setChecked(false);
	 mainWin->pushButtonPegar->setChecked(false);
  }

}



void pracAOC::copiar()
{

	if(seleccion)
	{
	  stopSelection();
	  update();
	  painter::copiar(img, bCopia, xSelIni, ySelIni, xSelFin-xSelIni, ySelFin-ySelIni);
	  copiaFlag=true;
	  wCopia=xSelFin-xSelIni;
	  hCopia=ySelFin-ySelIni;

	}

}

void pracAOC::cortar()
{

	if(seleccion)
	{
	  stopSelection();
	  update();
	  painter::cortar(img, bCopia, xSelIni, ySelIni, xSelFin-xSelIni, ySelFin-ySelIni,colorBrushInd);
	  copiaFlag=true;
	  wCopia=xSelFin-xSelIni;
	  hCopia=ySelFin-ySelIni;

	}

}



void pracAOC::changeZoom(int z)
{
	zoom=z;
	update();
}

void pracAOC::changePenWidth(int p)
{
	penWidth=p;
	update();
}


void pracAOC::colorPenSelection()
{
    QColor color = colorDialog->getColor(Qt::white,this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(color.isValid())
    {
        QRgb rgbColor = color.rgb();
        for(int i=0; i<colorDialog->customCount() && i<208;i++)
            ctable[i+48]=colorDialog->customColor(i).rgb();
        colorPenInd=ctable.indexOf(rgbColor);
        for(long i=0; i<mainWin->currentPenColor->width()*mainWin->currentPenColor->height(); i++)
            imgPenColor[i]=colorPenInd;
    }

 	update();
}

void pracAOC::colorBrushSelection()
{
    QColor color = colorDialog->getColor(Qt::white,this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(color.isValid())
    {
        QRgb rgbColor = color.rgb();
        for(int i=0; i<colorDialog->customCount() && i<208;i++)
          ctable[i+48]=colorDialog->customColor(i).rgb();
        colorBrushInd=ctable.indexOf(rgbColor);
        for(long i=0; i<mainWin->currentBrushColor->width()*mainWin->currentBrushColor->height(); i++)
            imgBrushColor[i]=colorBrushInd;
    }
 	update();
}

void pracAOC::startSelection(QPoint pos)
{
	xFin=xIni=pos.x()/zoom;
	yFin=yIni=pos.y()/zoom;
	if(xIni>=0 && xIni<WIDTHIMG && yIni>=0 && yIni<HEIGHTIMG)
	{
	  seleccion=true;
	  if(mainWin->pushButtonLinea->isChecked())
		selectionDraw=LINEA;
	  else
		if(mainWin->pushButtonCirculo->isChecked())
		  selectionDraw=CIRCULO;
		else
		  selectionDraw=RECTANGULO;
	}

}

void pracAOC::updateSelectionRectangulo()
{
	int x, y;
	
	if(xIni<xFin){
		for(x=xIni;x<xFin;x+=2){
			img[yIni*WIDTHIMG+x]=255-img[yIni*WIDTHIMG+x];
			img[yFin*WIDTHIMG+x]=255-img[yFin*WIDTHIMG+x];
		}
	}
	else{
		for(x=xIni;x>xFin;x-=2){
			img[yIni*WIDTHIMG+x]=255-img[yIni*WIDTHIMG+x];
			img[yFin*WIDTHIMG+x]=255-img[yFin*WIDTHIMG+x];
		}
	}
	if(yIni<yFin){
		for(y=yIni;y<yFin;y+=2){
			img[y*WIDTHIMG+xIni]=255-img[y*WIDTHIMG+xIni];
			img[y*WIDTHIMG+xFin]=255-img[y*WIDTHIMG+xFin];
		}

	}
	else{
		for(y=yIni;y>yFin;y-=2){
			img[y*WIDTHIMG+xIni]=255-img[y*WIDTHIMG+xIni];
			img[y*WIDTHIMG+xFin]=255-img[y*WIDTHIMG+xFin];
		}
	}
	img[yIni*WIDTHIMG+xIni]=255-img[yIni*WIDTHIMG+xIni];
	img[yFin*WIDTHIMG+xFin]=255-img[yFin*WIDTHIMG+xFin];


}

void pracAOC::updateSelectionLinea()
{
  
  	int x, y;
	
	if(abs(xIni-xFin)>abs(yIni-yFin))
	{
	  if(xIni<xFin){
		  for(x=xIni;x<xFin;x+=2){
			  y=((x-xIni)*(yFin-yIni))/(xFin-xIni)+yIni;
			  img[y*WIDTHIMG+x]=255-img[y*WIDTHIMG+x];
		  }
	  }
	  else{
		  for(x=xIni;x>xFin;x-=2){
			  y=((x-xIni)*(yFin-yIni))/(xFin-xIni)+yIni;
			  img[y*WIDTHIMG+x]=255-img[y*WIDTHIMG+x];
		  }
	  }
	}
	else
	{
	  if(yIni<yFin){
		  for(y=yIni;y<yFin;y+=2){
			  x=((y-yIni)*(xFin-xIni))/(yFin-yIni)+xIni;
			  img[y*WIDTHIMG+x]=255-img[y*WIDTHIMG+x];
		  }

	  }
	  else{
		  for(y=yIni;y>yFin;y-=2){
			  x=((y-yIni)*(xFin-xIni))/(yFin-yIni)+xIni;
			  img[y*WIDTHIMG+x]=255-img[y*WIDTHIMG+x];
		  }
	  }
	}

}

void pracAOC::updateSelectionCirculo()
{
	int x, y;
	int r;
	
	r=(int) sqrt((float)((xFin-xIni)*(xFin-xIni)+(yFin-yIni)*(yFin-yIni)));
	
	for(x=xIni-r;x<=xIni+r;x+=2){
		img[(yIni-r)*WIDTHIMG+x]=255-img[(yIni-r)*WIDTHIMG+x];
		img[(yIni+r)*WIDTHIMG+x]=255-img[(yIni+r)*WIDTHIMG+x];
	}
	
	for(y=yIni-r;y<yIni+r;y+=2){
		img[y*WIDTHIMG+(xIni-r)]=255-img[y*WIDTHIMG+(xIni-r)];
		img[y*WIDTHIMG+(xIni+r)]=255-img[y*WIDTHIMG+(xIni+r)];
	}

}

void pracAOC::updateSelection(QPoint pos)
{
  	int xInter, yInter;
	int r;
	
	if(seleccion){

		xInter=pos.x()/zoom;
		yInter=pos.y()/zoom;
		if(xInter>=WIDTHIMG) xInter=WIDTHIMG-1;
		if(yInter>=HEIGHTIMG) yInter=HEIGHTIMG-1;
		if((xInter!=xFin || yInter!=yFin))
		{
			switch(selectionDraw)
			{
			  case LINEA: 
						  updateSelectionLinea();
						  break;
			  case CIRCULO:
						  updateSelectionCirculo();
						  break;
			  default:
						  updateSelectionRectangulo();
						  break;
			}
            r=(int) sqrt((float)((xInter-xIni)*(xInter-xIni)+(yInter-yIni)*(yInter-yIni)));
			if(selectionDraw!=CIRCULO || ((xIni-r)>=0 && (xIni+r)<WIDTHIMG && (yIni-r)>=0 && (yIni+r)<HEIGHTIMG))
			{
			  xFin=xInter;
			  yFin=yInter;
			}
			switch(selectionDraw)
			{
			  case LINEA: 
						  updateSelectionLinea();
						  break;
			  case CIRCULO:
						  updateSelectionCirculo();
						  break;
			  default:
						  updateSelectionRectangulo();
						  break;
			}


			update();
		}
    }
}

void pracAOC::stopSelection()
{
	if(seleccion)
	{
		seleccion=false;
		switch(selectionDraw)
		{
		  case LINEA: 
					  updateSelectionLinea();
					  break;
		  case CIRCULO:
					  updateSelectionCirculo();
					  break;
		  default:
					  updateSelectionRectangulo();
					  break;
		}

	}
}

void pracAOC::mousePressEvent(QMouseEvent *event)
{
	QPoint pos=mainWin->paintScrollArea->widget()->mapFrom(this,event->pos());
	int x=pos.x()/zoom;
	int y=pos.y()/zoom;
	if(x>=0 && x<WIDTHIMG && y>=0 && y<HEIGHTIMG)
	{
	  if(seleccion) stopSelection();
	  if(mainWin->pushButtonModoLibre->isChecked())
	  {
		  if(x+penWidth<WIDTHIMG && y+penWidth<HEIGHTIMG)
		  {
			painter::modoLibre(img, x, y, penWidth, colorPenInd);
			update();
		  }
	  }
	  
	  else
	  {
		  if(mainWin->pushButtonRellenar->isChecked())
		  {
			  painter::rellenar(img, x, y, colorBrushInd);
			  update();
		  }
		  else
		  {
			if(mainWin->pushButtonPegar->isChecked())
			{
			  if(copiaFlag)
			  {
				painter::pegar(img, bCopia, x, y, wCopia, hCopia);
				update();
			  }
			}	
			else
			{
			  if(mainWin->pushButtonPegarEspecial->isChecked())
			  {
				if(copiaFlag)
				{
				  painter::pegarEspecial(img, bCopia, x, y, wCopia, hCopia, colorBrushInd);
				  update();
				}
			  }	
			  else
				if (event->button() == Qt::LeftButton && mainWin->paintScrollArea->geometry().contains(event->pos()))
					startSelection(pos); 
			}
		  }
	  }
	}
}



void pracAOC::mouseMoveEvent(QMouseEvent *event)
{

	int x, y;

	QPoint pos=mainWin->paintScrollArea->widget()->mapFrom(this,event->pos());
	x=pos.x()/zoom;
	y=pos.y()/zoom;
	if(x>=0 && x<WIDTHIMG && y>=0 && y<HEIGHTIMG)
	{
	  if(mainWin->pushButtonModoLibre->isChecked())
	  {
          if(x+penWidth<=WIDTHIMG && y+penWidth<=HEIGHTIMG)
		  {
			painter::modoLibre(img, x, y, penWidth, colorPenInd);
			update();
		  }
	  }
	  else
	  {
		if (mainWin->paintScrollArea->geometry().contains(event->pos())) 
		  if(seleccion)
			updateSelection(pos);
		  
	  }
	}
}
void pracAOC::mouseReleaseEvent(QMouseEvent *event)
{
    if (seleccion)
	{
		if(xIni>xFin){
			xSelIni=xFin;
			xSelFin=xIni;
		}
		else{
			xSelIni=xIni;
			xSelFin=xFin;
		}

		if(yIni>yFin){
			ySelFin=yIni;
			ySelIni=yFin;
		}
		else{
			ySelFin=yFin;
			ySelIni=yIni;
		}

		if(mainWin->pushButtonLinea->isChecked())
		{
		  	stopSelection();
			int x1, x2, y1, y2;
			if(abs(xIni-xFin)>abs(yIni-yFin))
			{
				if(xIni<=xFin)
				{
					x1=xIni;	y1=yIni;	x2=xFin;	y2=yFin;
				}
				else
				{
					x1=xFin;	y1=yFin;	x2=xIni;	y2=yIni;
				}
				  
			}
			else
			{
				if(yIni<=yFin)
				{
					x1=xIni;	y1=yIni;	x2=xFin;	y2=yFin;
				}
				else
				{
					x1=xFin;	y1=yFin;	x2=xIni;	y2=yIni;
				}

			}
			painter::linea(img, x1, y1, x2, y2, penWidth, colorPenInd);
		}
		if(mainWin->pushButtonCirculo->isChecked())
		{
		  	stopSelection();
			int xc, yc, r;

			xc=xIni;
			yc=yIni;
            r=(int) sqrt((float)((xFin-xIni)*(xFin-xIni)+(yFin-yIni)*(yFin-yIni)));

			painter::circulo(img, xc, yc, r, penWidth, colorPenInd);			
		}
		
		if(mainWin->pushButtonRectangulo->isChecked())
		{
		  	stopSelection();
			painter::rectangulo(img, xSelIni, ySelIni, xSelFin, ySelFin, penWidth, colorPenInd);
			
		}
				
		update();
    }
}



void pracAOC::closeEvent(QCloseEvent *event)
{
 	delete mainWin;

}
