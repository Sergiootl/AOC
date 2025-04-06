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

#include "painter.h"



void painter::borrar(uchar * img, uchar color)
{
    asm volatile(


        "mov %0, %%rsi;" //img
        "mov %1, %%dl ;"//color
        "mov $480, %%rax;"
        "imul $640, %%rax;"//np
        "mov $0, %%rbx;"//p
        "jmp BorrarCondicion;"
        "BucleBorrar:"
                "mov %%dl, (%%rsi, %%rbx) ;"
        "inc %%rbx ;"
        "BorrarCondicion:;"
        "cmp %%rax, %%rbx ;"
        "jl BucleBorrar ;"
        ";"

        :
        : "m" (img), "m" (color)
        : "%rax","%rbx", "%rcx", "%rdx", "%rsi","%rdi", "memory"

    );

}

void painter::modoLibre(uchar * img, int cI, int fI, int grosor, uchar color)
{
    asm volatile(
        "mov %0, %%rsi ;"
        "mov %1, %%ebx;"
        "movsx %%ebx, %%rbx;"
        "mov %2, %%eax ;"
        "movsx %%eax, %%rax;"
        "mov %3, %%ecx ;"
        "movsx %%ecx, %%rcx;"
        "mov %4, %%dl ;"
        "imul $640, %%rax ;"
        "add %%rbx, %%rax ;"
        "add %%rax, %%rsi ;"
        "mov $0, %%rax	;"
"bModoLapizFilas:"
        "mov $0, %%rbx	;"
    "bModoLapizColumnas:"
            "mov %%dl, (%%rsi, %%rbx) ;"
            "inc %%rbx ;"
            "cmp %%rcx, %%rbx ;"
            "jl bModoLapizColumnas ;"
        "add $640, %%rsi ;"
        "inc %%rax ;"
        "cmp %%rcx, %%rax ;"
        "jl bModoLapizFilas ;"
        :
        : "m" (img), "m" (cI), "m" (fI), "m" (grosor), "m" (color)
        : "%rax","%rbx", "%rcx", "%rdx","%rsi","%rdi", "memory"
    );
}


void painter::linea(uchar * img, int c1, int f1, int c2, int f2, int grosor, uchar color)
{
	
    asm volatile(
        "mov %0, %%rsi;" //dirImg
        "mov %1, %%r15d;"
        "movsx %%r15d, %%r15;" //c1
        "mov %2, %%r14d;"
        "movsx %%r14d, %%r14;" //f1
        "mov %3, %%r13d;"
        "movsx %%r13d, %%r13;" //c2
        "mov %4, %%r12d;"
        "movsx %%r12d, %%r12;" //f2
        "mov %5, %%r11d;"
        "movsx %%r11d, %%r11;" //grosor
        "mov %6, %%r10b;" //color
        "mov %%r13, %%r9;"
        "sub %%r15, %%r9;" //dC
        "mov %%r12, %%r8;"
        "sub %%r14, %%r8;" //dF
        "cmp %%r8, %%r9;"
        "jle ColMayorFil;"
            "mov %%r15, %%rbx;" //c
            "Col1MayorCol2:"
                "mov %%rbx, %%rax;" //f
                "sub %%r15, %%rax;"
                "imul %%r8, %%rax;"
                "cqto;"
                "idiv %%r9;"
                "add %%r14, %%rax;" //f
                "mov %%rax, %%rcx;"
                "imul $640, %%rcx;"
                "add %%rbx, %%rcx;" //offLinea
                "mov $0, %%rdi;" //g
                "BucleGrosor:"
                    "mov %%r10b, (%%rsi,%%rcx);"
                    "add $640, %%rcx;"
                    "inc %%rax;"
                "inc %%rdi;"
                "cmp %%r11, %%rdi;"
                "jge FinBucleGrosor;"
                "cmp $480, %%rax;"
                "jl BucleGrosor;"
                "FinBucleGrosor:"
            "inc %%rbx;"
            "cmp %%r13, %%rbx;"
            "jl Col1MayorCol2;"
        "ColMayorFil:"
            "mov %%r14, %%rbx;" //f
            "Fila1MayorFila2:"
                "mov %%rbx, %%rax;" //c
                "sub %%r14, %%rax;"
                "imul %%r9, %%rax;"
                "cqto;"
                "idiv %%r8;"
                "add %%r15, %%rax;" //c

                "mov %%rbx, %%rcx;"
                "imul $640, %%rcx;"
                "add %%rax, %%rcx;" //offLinea
                "mov $0, %%rdi;" //g
                "BucleGrosorFila:"
                    "mov %%r10b, (%%rsi,%%rcx);"
                    "inc %%rcx;"
                    "inc %%rax;"
                "inc %%rdi;"
                "cmp %%r11, %%rdi;"
                "jge FinBucleGrosorFila;"
                "cmp $640, %%rax;"
                "jl BucleGrosorFila;"
                "FinBucleGrosorFila:"
            "inc %%rbx;"
            "cmp %%r12, %%rbx;"
            "jl Fila1MayorFila2;"

        ";"

        :
        : "m" (img), "m" (c1), "m" (f1), "m" (c2), "m" (f2), "m" (grosor), "m" (color)
        : "%rax","%rbx", "%rcx", "%rdx","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15", "%rsi","%rdi", "memory"
	
  );
}



void painter::rectangulo(uchar * img, int c1, int f1, int c2, int f2, int grosor, uchar color)
{
	
    asm volatile(
        "mov %0,%%rsi;" //dirImg
        "mov %1, %%ebx;"
        "movsx %%ebx, %%rbx;" //c1
        "mov %2, %%eax;"
        "movsx %%eax, %%rax;" //f1
        "mov %3, %%ecx;"
        "movsx %%ecx, %%rcx;" //c2
        "mov %4, %%edx;"
        "movsx %%edx, %%rdx;" //f2
        "mov %5, %%edi;"
        "movsx %%edi, %%rdi;" //grosor
        "mov %6, %%r15b;" //color
        "mov %%rbx, %%r14;" //c
        "Bucle1Rect:"
            "mov %%rax, %%r13;"
            "imul $640, %%r13;"
            "add %%r14, %%r13;" //offLinea
            "mov %%rax, %%r12;" //f
            "BucleGrosorRect:"
                "mov %%r15b, (%%rsi,%%r13);"
                "add $640, %%r13;"
                "inc %%r12;"
            "mov %%rax, %%r11;"
            "add %%rdi, %%r11;"
            "cmp %%r11, %%r12;"
            "jge FinBucleGrosorRect;"
            "cmp $480, %%r12;"
            "jl BucleGrosorRect;"
            "FinBucleGrosorRect:"
            //2 bucle
            "mov %%rdx, %%r13;"
            "imul $640, %%r13;"
            "add %%r14, %%r13;" //offLinea
            "mov %%rdx, %%r12;" //f
            "BucleGrosorRect2:"
                "mov %%r15b, (%%rsi,%%r13);"
                "add $640, %%r13;"
                "inc %%r12;"
            "mov %%rdx, %%r11;"
            "add %%rdi, %%r11;"
            "cmp %%r11, %%r12;"
            "jge FinBucleGrosorRect2;"
            "cmp $480, %%r12;"
            "jl BucleGrosorRect2;"
            "FinBucleGrosorRect2:"
        "inc %%r14;"
        "cmp %%rcx, %%r14;"
        "jle Bucle1Rect;"
        //BUCLE DE LAS FILAS DEL RECTANGULO
        "mov %%rax, %%r14;" //f
        "Bucle2Rect:"
        "cmp $480, %%r14;"
        "jge CondIFRect2;"
            "mov %%r14, %%r13;"
            "imul $640, %%r13;"
            "add %%rbx, %%r13;" //offLinea
            "mov %%rbx, %%r12;" //c
            "Bucle2GrosorRect2:"
                "mov %%r15b, (%%rsi,%%r13);"
                "inc %%r13;"
            "inc %%r12;"
            "mov %%rbx, %%r11;"
            "add %%rdi, %%r11;"
            "cmp %%r11, %%r12;"
            "jge FinBucle2GrosorRect2;"
            "cmp $640, %%r12;"
            "jl Bucle2GrosorRect2;"
            "FinBucle2GrosorRect2:"
            //2 bucle
            "mov %%r14, %%r13;"
            "imul $640, %%r13;"
           "add %%rcx, %%r13;" //offLinea
            "mov %%rcx, %%r12;" //c
           "Bucle2Grosor2Rect2:"
                "mov %%r15b, (%%rsi,%%r13);"
                "inc %%r13;"
            "inc %%r12;"
            "mov %%rcx, %%r11;"
            "add %%rdi, %%r11;"
            "cmp %%r11, %%r12;"
            "jge FinBucle2Grosor2Rect2;"
            "cmp $640, %%r12;"
            "jl Bucle2Grosor2Rect2;"
            "FinBucle2Grosor2Rect2:"
         "CondIFRect2:"
         "inc %%r14;"
         "mov %%rdx, %%r11;"
         "add %%rdi, %%r11;"
         "cmp %%r11, %%r14;"
         "jl Bucle2Rect;"



        ";"

        :
        : "m" (img), "m" (c1), "m" (f1), "m" (c2), "m" (f2), "m" (grosor), "m" (color)
        : "%rax","%rbx", "%rcx", "%rdx","%r11","%r12","%r13","%r14","%r15", "%rsi","%rdi", "memory"
    );


}


void painter::circulo(uchar * img, int c_c, int f_c, int r, int grosor, uchar color)
{
    // En caso de implementar el procedimiento utilizando instrucciones de FPU,
    // esta variable se debe usar para intercambio de datos entre CPU y FPU.
    // Su uso no es necesario si se utilizan las instrucciones sobre escalares de SSE.
    int aux;

    asm volatile(
        ";"

        :
        : "m" (img), "m" (c_c), "m" (f_c), "m" (r), "m" (grosor), "m" (color), "m" (aux)
        : "%rax","%rbx", "%rcx", "%rdx", "%rsi","%rdi", "memory"
    );

}



void painter::rellenar(uchar * img, int c, int f, uchar color)
{
    asm volatile(
        ";"

        :
        : "m" (img), "m" (c), "m" (f), "m" (color)
        : "%rax","%rbx", "%rcx", "%rdx", "%rsi","%rdi", "memory"

    );


}



void painter::copiar(uchar * img, uchar * buffer, int c, int f, int w, int h)
{
    asm volatile(
        "mov %0, %%rsi;" //img
        "mov %3, %%eax;"
        "movsx %%eax, %%rax;" //f
        "imul $640,%%rax;"
        "add %%rax, %%rsi;"
        "mov %1, %%rdi;"//dirBuffer
        "mov %2, %%ebx;"
        "movsx %%ebx, %%rbx;" //c
        "mov %4, %%ecx;"
        "movsx %%ecx, %%rcx;" //w
        "mov %5, %%edx;"
        "movsx %%edx, %%rdx;" //h
        "mov $0, %%r15;" //cF
        "BucleCopy:"
                "mov %%rbx, %%r14;" //offC
                "mov $0, %%r13;" //cC
                "BucleCopy2:"
                    "mov (%%rsi,%%r14), %%r12b;"
                    "mov %%r12b, (%%rdi);"
                    "inc %%rdi;"
                    "inc %%r14;"

                "inc %%r13 ;"
                "cmp %%rcx, %%r13;"
                "jl BucleCopy2;"

                "add $640, %%rsi;"

        "inc %%r15 ;"
        "cmp %%rdx, %%r15 ;"
        "jl BucleCopy ;"
        ";"

        :
        : "m" (img), "m" (buffer), "m" (c), "m" (f), "m" (w), "m" (h)
        : "%rax","%rbx", "%rcx", "%rdx","%r12","%r13","%r14","%r15", "%rsi","%rdi", "memory"

  );
}

void painter::cortar(uchar * img, uchar * buffer, int c, int f, int w, int h, uchar cFondo)
{
    asm volatile(
                "mov %0, %%rsi;" //img
                "mov %3, %%eax;"
                "movsx %%eax, %%rax;" //f
                "imul $640,%%rax;"
                "add %%rax, %%rsi;"
                "mov %1, %%rdi;"//dirBuffer
                "mov %2, %%ebx;"
                "movsx %%ebx, %%rbx;" //c
                "mov %4, %%ecx;"
                "movsx %%ecx, %%rcx;" //w
                "mov %5, %%edx;"
                "movsx %%edx, %%rdx;" //h
                "mov %6, %%r10b;" //cFondo
                "mov $0, %%r15;" //cF
                "BucleCortar:"
                        "mov %%rbx, %%r14;" //offC
                        "mov $0, %%r13;" //cC
                        "BucleCortar2:"
                            "mov (%%rsi,%% r14), %%r12b;"
                            "mov %%r12b, (%%rdi);"
                            "mov %%r10b,(%%rsi,%% r14);"
                            "inc %%rdi;"
                            "inc %%r14;"

                        "inc %%r13;"
                        "cmp %%rcx, %%r13;"
                        "jl BucleCortar2;"

                        "add $640, %%rsi;"

                "inc %%r15 ;"
                "cmp %%rdx, %%r15 ;"
                "jl BucleCortar;"
                  ";"

        :
        : "m" (img), "m" (buffer), "m" (c), "m" (f), "m" (w), "m" (h), "m" (cFondo)
        : "%rax","%rbx", "%rcx", "%rdx","%r10","%r11","%r12","%r13","%r14","%r15", "%rsi","%rdi", "memory"
    );


}

void painter::pegar(uchar * img, uchar * buffer, int c, int f, int w, int h)
{
    asm volatile(
            "mov %0, %%rsi;" //img
            "mov %2, %%ebx;"
            "movsx %%ebx, %%rbx;" // c
            "mov %3, %%eax;"
            "movsx %%eax, %%rax;" // f
            "mov %%rax, %%r10;"
            "imul $640, %%r10;"
            "add %%r10, %%rsi;" //dirImg
            "mov %1, %%rcx;" //dirBuffer
            "mov %4, %%edx;"
            "movsx %%edx, %%rdx;" // w
            "mov %5, %%edi;"
            "movsx %%edi, %%rdi;" // h
            "mov $0, %%r15;" //cF
            "Bucle1Pegar:"
                    "mov %%rbx, %%r14;" //offC
                    "mov $0, %%r13;" //cC
                    "Bucle2Pegar:"
                        "cmp $640, %%r14;"
                        "jge FueraIF;"
                        "mov (%%rcx), %%r12b;"
                        "mov %%r12b, (%%rsi, %%r14);"
                        "FueraIF:"
                        "inc %%rcx;"
                        "inc %%r14;"
                    "inc %%r13;"
                    "cmp %%rdx, %%r13;"
                    "jl Bucle2Pegar;"
            "add $640, %%rsi;"
            "inc %%r15;"
            "cmp %%rdi, %%r15;"
            "jge FinBucle1Pegar;"
            "mov %%r15, %%r11;"
            "add %%rax, %%r11;"
            "cmp $480, %%r11;"
            "jl Bucle1Pegar;"
            "FinBucle1Pegar:"


        ";"

        :
        : "m" (img), "m" (buffer), "m" (c), "m" (f), "m" (w), "m" (h)
        : "%rax","%rbx", "%rcx", "%rdx","%r10", "%r11","%r12","%r13","%r14","%r15", "%rsi","%rdi", "memory"

    );


}

void painter::pegarEspecial(uchar * img, uchar * buffer, int c, int f, int w, int h, uchar color)
{
    asm volatile(
            "mov %0, %%rsi;" //img
            "mov %2, %%ebx;"
            "movsx %%ebx, %%rbx;" // c
            "mov %3, %%eax;"
            "movsx %%eax, %%rax;" // f
            "mov %%rax, %%r9;"
            "imul $640, %%r9;"
            "add %%r9, %%rsi;" //dirImg
            "mov %1, %%rcx;" //dirBuffer
            "mov %4, %%edx;"
            "movsx %%edx, %%rdx;" // w
            "mov %5, %%edi;"
            "movsx %%edi, %%rdi;" // h
            "mov %6, %%r10b;" //color
            "mov $0, %%r15;" //cF
            "Bucle1PegarEsp:"
                  "mov %%rbx, %%r14;" //offC
                  "mov $0, %%r13;" //cC
                  "Bucle2PegarEsp:"
                        "cmp $640, %%r14;"
                        "jge FueraIF1;"
                        "cmp %%r10b,(%%rcx);"
                       "je FueraIF2;"
                        "mov (%%rcx), %%r12b;"
                        "mov %%r12b, (%%rsi, %%r14);"
                        "FueraIF2:"
                        "FueraIF1:"
                        "inc %%rcx;"
                        "inc %%r14;"
                  "inc %%r13;"
                  "cmp %%rdx, %%r13;"
                  "jl Bucle2PegarEsp;"
          "add $640, %%rsi;"
          "inc %%r15;"
          "cmp %%rdi, %%r15;"
          "jge FinBucle1PegarEsp;"
          "mov %%r15, %%r11;"
          "add %%rax, %%r11;"
          "cmp $480, %%r11;"
          "jl Bucle1PegarEsp;"
          "FinBucle1PegarEsp:"
        ";"

        :
        : "m" (img), "m" (buffer), "m" (c), "m" (f), "m" (w), "m" (h), "m" (color)
        : "%rax","%rbx", "%rcx", "%rdx","%r9","%r10","%r11","%r12","%r13","%r14","%r15", "%rsi","%rdi", "memory"
    );


}




