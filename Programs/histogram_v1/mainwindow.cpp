#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <cmath>
#include <QPainter>
void brightness(const QImage src, QImage& dst, int v){
    for(int y=0;y<src.height();y++){
        unsigned int* pixel_src = (QRgb*) src.scanLine(y);
        QRgb* pixel_dst = (QRgb*) dst.scanLine(y);
        for(int x=0;x<src.width();x++){
            //pixel_dst[x]=pixel_src[x]+v;
            unsigned char r = qRed(pixel_src[x]);
            unsigned char g = qGreen(pixel_src[x]);
            unsigned char b = qBlue(pixel_src[x]);
            r=clamp<int>(r+v,0,255);
            g=clamp<int>(g+v,0,255);
            b=clamp<int>(b+v,0,255);
            pixel_dst[x]=qRgb(r,g,b);
        }
    }
}
void contrast(QImage src,QImage& dst,int v)
{
    float fVal=(259.0 * (v + 255.0)) / (255.0 * (259.0 - v));
    for(int y=0;y<src.height();y++){
        unsigned int* pixel_src = (QRgb*) src.scanLine(y);
        QRgb* pixel_dst = (QRgb*) dst.scanLine(y);
        for(int x=0;x<src.width();x++){
            //pixel_dst[x]=pixel_src[x]+v;
            uchar r = qRed(pixel_src[x]);
            uchar g = qGreen(pixel_src[x]);
            uchar b = qBlue(pixel_src[x]);

            float fr,fg,fb;
            fr=clamp<float>(fVal*(r-128)+128,0,255);
            fg=clamp<float>(fVal*(g-128)+128,0,255);
            fb=clamp<float>(fVal*(b-128)+128,0,255);


            r = fr;
            g = fg;
            b = fb;
            pixel_dst[x]=qRgb(r,g,b);
    }

}
}
void gamma(const QImage src, QImage& dst, int v){
    float fVal;
    if(v<0){
        fVal = (v+100)/100.0f;
    }else{
        fVal = (v)/10.0f+1;
    }
    for(int y=0;y<src.height();y++){
        unsigned int* pixel_src = (QRgb*) src.scanLine(y);
        QRgb* pixel_dst = (QRgb*) dst.scanLine(y);
        for(int x=0;x<src.width();x++){
            //pixel_dst[x]=pixel_src[x]+v;
            uchar r = qRed(pixel_src[x]);
            uchar g = qGreen(pixel_src[x]);
            uchar b = qBlue(pixel_src[x]);

            float fr,fg,fb;
            fr=pow(r/255.0f,fVal)*255.0f;
            fg=pow(g/255.0f,fVal)*255.0f;
            fb=pow(b/255.0f,fVal)*255.0f;


            r = fr;
            g = fg;
            b = fb;
            pixel_dst[x]=qRgb(r,g,b);
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::openImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.xpm *.jpg *.bmp)"));
    image.load(fileName);
    if(image.isNull())
    {
        qDebug() << "Failed to open image";
        return;
    }
    processImage = image;
       ui->imageLabel->setPixmap(QPixmap::fromImage(processImage));
    QLabel *imageLabel = new QLabel();
    //QPixmap pixmap = QPixmap::fromImage(image);
        //imageLabel->setPixmap(pixmap);
       //ui->scrollArea->setWidget(imageLabel);

    // tworzenie histogramu
    int histogram[256] = {0};
    int width = image.width();
    int height = image.height();

    // normalizacja i konwersja na skale szarosci
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            QRgb pixel = image.pixel(i, j);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            int gray = 0.2989 * red + 0.5870 * green + 0.1140 * blue;
            histogram[gray]++;
        }
    }

    // przeskalowanie histogramu
    int max = 0;
    for(int i = 0; i < 256; i++)
    {
        if(histogram[i] > max)
            max = histogram[i];
    }
    for(int i = 0; i < 256; i++)
    {
        histogram[i] = (histogram[i] * 4);
    }

    // max value histogramu
    max = 0;
    for(int i = 0; i < 256; i++)
    {
        if(histogram[i] > max)
            max = histogram[i];
    }
    // przeskalowanie histogramu
    for(int i = 0; i < 256; i++)
    {
        histogram[i] = (histogram[i] * 400) / max;
    }

    // rysowanie histogramu

    QLabel *histLabel = new QLabel();
    QPixmap histPixmap(1024, 400);
    histPixmap.fill(Qt::transparent);
    QPainter painter(&histPixmap);
    for(int i = 0; i < 256; i++)
    {
        painter.drawLine(i, 400, i, 400 - histogram[i]);
        }
        painter.end();
        histLabel->setPixmap(histPixmap);
        ui->scrollArea_2->setWidget(histLabel);
        }

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    brightness(image,processImage,value);
    //gamma(qimage,processImage,value);
    ui->imageLabel->setPixmap(QPixmap::fromImage(processImage));
}
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    //brightness(qimage,processImage,value);
    gamma(image,processImage,value);
    ui->imageLabel->setPixmap(QPixmap::fromImage(processImage));
}
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    //brightness(qimage,processImage,value);
    contrast(image,processImage,value);
    ui->imageLabel->setPixmap(QPixmap::fromImage(processImage));
}
