#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_imageLabel = new QLabel(this);
    setCentralWidget(m_imageLabel);

    // Tworzenie przycisków
    QPushButton *openImageButton = new QPushButton("Open Image", this);
    QPushButton *maskButton1 = new QPushButton("Maska 1", this);
    QPushButton *maskButton2 = new QPushButton("Maska 2", this);

    // Położenie przycisków na ekranie
    openImageButton->setGeometry(10, 10, 100, 30);
    maskButton1->setGeometry(120, 10, 100, 30);
    maskButton2->setGeometry(230, 10, 100, 30);

    // Połączenie sygnałów przycisków z odpowiednimi slotami
    connect(openImageButton, &QPushButton::clicked, this, &MainWindow::onOpenImage);
    connect(maskButton1, &QPushButton::clicked, this, &MainWindow::maska1);
    connect(maskButton2, &QPushButton::clicked, this, &MainWindow::maska2);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onOpenImage()
{
    // Otwieranie okna do wyboru pliku
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.xpm *.jpg *.bmp)");
    if (fileName.isEmpty())
        return;

    // Wczytanie obrazu do QImage
    m_image.load(fileName);
    m_pixmap = QPixmap::fromImage(m_image);
    m_imageLabel->setPixmap(m_pixmap);
}

void MainWindow::maska1()
{
    int mask[9] = { -1, -1, -1, -1, 9,-1, -1, -1, -1 };
    // Tworzenie tymczasowego obiektu QImage
    QImage tempImage = m_image;

    for (int x = 1; x < m_image.width()-1; x++) {
        for (int y = 1; y < m_image.height()-1; y++) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    QRgb pixel = m_image.pixel(x+i, y+j);
                    sumR += qRed(pixel) * mask[(i+1)*3 + (j+1)];
                    sumG += qGreen(pixel) * mask[(i+1)*3 + (j+1)];
                    sumB += qBlue(pixel) * mask[(i+1)*3 + (j+1)];
                }
            }

            // Ustawienie nowych wartości pikseli po przekształceniu
            tempImage.setPixel(x, y, qRgb(qBound(0, sumR, 255), qBound(0, sumG, 255), qBound(0, sumB, 255)));
        }
    }

    m_image = tempImage;
    m_pixmap = QPixmap::fromImage(m_image);
    m_imageLabel->setPixmap(m_pixmap);
}

void MainWindow::maska2()
{
int mask[9] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
// Tworzenie tymczasowego obiektu QImage
QImage tempImage = m_image;

for (int x = 1; x < m_image.width()-1; x++) {
    for (int y = 1; y < m_image.height()-1; y++) {
        int sumR = 0, sumG = 0, sumB = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                QRgb pixel = m_image.pixel(x+i, y+j);
                sumR += qRed(pixel) * mask[(i+1)*3 + (j+1)];
                sumG += qGreen(pixel) * mask[(i+1)*3 + (j+1)];
                sumB += qBlue(pixel) * mask[(i+1)*3 + (j+1)];
            }
        }

        // Ustawienie nowych wartości pikseli po przekształceniu
        tempImage.setPixel(x, y, qRgb(qBound(0, sumR, 255), qBound(0, sumG, 255), qBound(0, sumB, 255)));
    }
}

m_image = tempImage;
m_pixmap = QPixmap::fromImage(m_image);
m_imageLabel->setPixmap(m_pixmap);
}

