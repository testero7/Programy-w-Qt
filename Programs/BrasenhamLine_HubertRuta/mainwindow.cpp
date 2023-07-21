#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kolor = qRgb(100,100,100);
    QImage image(1191,581,QImage::Format_RGB32);
    image.fill(Qt::white);
    ui->label->setPixmap(QPixmap::fromImage(image));
    obrazek = image;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::bresenham(){

    // Pobieranie współrzędnych punktów początkowego i końcowego
     int x1 = first.x();
     int x2 = second.x();
     int y1 = first.y();
     int y2 = second.y();
     int dx1,dy1,d;

     // Obliczenie różnicy między współrzędnymi x
     if(x2>x1){
         dx1=x2-x1;
     }
     else{
         dx1=x1-x2;
     }
    // Obliczenie różnicy między współrzędnymi y
     if(y2>y1){
        dy1=y2-y1;
     }
     else{
        dy1=y1-y2;
     }

     //os 1
     if(dx1>=dy1){

        d=2*dy1-dx1;
    // Pętla wykonująca rysowanie od punktu x1 do x2
        while (x1!=x2) {

            if(d>0){
                if(x1<x2){
                    x1++;
                }
                else{
                    x1--;
                }
                if(y1<y2){
                    y1++;
                }
                else{
                    y1--;
                }
                d=d+(2*(dy1-dx1));
                // Ustawienie koloru na współrzędnych (x1, y1)
                obrazek.setPixel(x1,y1,kolor);
                ui->label->setPixmap(QPixmap::fromImage(obrazek));
            }
            else{
                if(x1<x2){
                    x1++;
                }
                else{
                    x1--;
                }
                d=d+(2*dy1);
                obrazek.setPixel(x1,y1,kolor);
                ui->label->setPixmap(QPixmap::fromImage(obrazek));
            }
        }
     }

     if(dx1<dy1){

         d=(2*dx1)-dy1;

         while(y1!=y2){

             if(d>=0){
                if(x1<x2){
                    x1++;
                }
                else{
                    x1--;

                }
                if(y1<y2){
                    y1++;
                }
                else{
                    y1--;
                }
                d=d+(2*(dx1-dy1));
                obrazek.setPixel(x1,y1,kolor);
                ui->label->setPixmap(QPixmap::fromImage(obrazek));
            }
            else{
                if(y1<y2){
                    y1++;
                }
                else{
                    y1--;
                }
                d=d+(2*dx1);
                obrazek.setPixel(x1,y1,kolor);
                ui->label->setPixmap(QPixmap::fromImage(obrazek));
            }
         }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (sprawdzenie % 2 == 0 ) {
        first = e->pos();
        sprawdzenie++;
    }else{
        second = e->pos();
        sprawdzenie++;
        bresenham();
    }
}
