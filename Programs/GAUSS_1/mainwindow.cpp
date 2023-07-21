#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    imageLabel = new QLabel(this);
    imageLabel->setGeometry(10, 10, 600, 600);
    imageLabel->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.xpm *.jpg *.bmp)"));
    image.load(fileName);
    pixmap = QPixmap::fromImage(image);
    imageLabel->setPixmap(pixmap);
}

void MainWindow::on_gaussianButton_clicked()
{
    applyGaussianFilter();
}

void MainWindow::on_unsharpButton_clicked()
{
    applyUnsharpMask();
}

void MainWindow::applyUnsharpMask()
{
    int kernel[3][3] = {{0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0}};
    int kernelSize = 3;
    int width = image.width();
    int height = image.height();
    QImage newImage(width, height, QImage::Format_ARGB32);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int r = 0, g = 0, b = 0;
            for (int ky = -kernelSize/2; ky <= kernelSize/2; ky++)
            {
                for (int kx = -kernelSize/2; kx <= kernelSize/2; kx++)
                {
                    int posX = x + kx;
                    int posY = y + ky;
                    if (posX < 0 || posX >= width || posY < 0 || posY >= height)
                        continue;

                    QRgb pixel = image.pixel(posX, posY);
                    r += qRed(pixel) * kernel[ky + kernelSize/2][kx + kernelSize/2];
                    g += qGreen(pixel) * kernel[ky + kernelSize/2][kx + kernelSize/2];
                    b += qBlue(pixel) * kernel[ky + kernelSize/2][kx + kernelSize/2];
                }
            }

            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    image = newImage;
    pixmap = QPixmap::fromImage(image);
    imageLabel->setPixmap(pixmap);
}

void MainWindow::applyGaussianFilter()
{
    int kernel[3][3] = {{1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}};
    int kernelSize = 3;
    int kernelWeight = 16;
    int width = image.width();
    int height = image.height();
    QImage newImage(width, height, QImage::Format_ARGB32);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int r = 0, g = 0, b = 0;
            for (int ky = -kernelSize/2; ky <= kernelSize/2; ky++)
            {
                for (int kx = -kernelSize/2; kx <= kernelSize/2; kx++)
                {
                    int posX = x + kx;
                    int posY = y + ky;
                    if (posX < 0 || posX >= width || posY < 0 || posY >= height)
                        continue;

                    QRgb pixel = image.pixel(posX, posY);
                    r += qRed(pixel) * kernel[ky + kernelSize/2][kx + kernelSize/2];
                    g += qGreen(pixel) * kernel[ky + kernelSize/2][kx + kernelSize/2];
                    b += qBlue(pixel) * kernel[ky + kernelSize/2][kx + kernelSize/2];
                }
            }

            r /= kernelWeight;
            g /= kernelWeight;
            b /= kernelWeight;

            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    image = newImage;
    pixmap = QPixmap::fromImage(image);
    imageLabel->setPixmap(pixmap);
}

