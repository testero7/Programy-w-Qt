#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QColor>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(600, 600);
    hSlider = new QSlider(Qt::Horizontal, this);
    sSlider = new QSlider(Qt::Horizontal, this);
    lSlider = new QSlider(Qt::Horizontal, this);
    openImageButton = new QPushButton("Open Image", this);
    openImageButton->setFixedSize(100, 30);
    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(600, 600);
    graphicsScene = new QGraphicsScene(this);
    graphicsView->setScene(graphicsScene);

    ui->mainLayout->addWidget(imageLabel, 0, 0);
    ui->mainLayout->addWidget(hSlider, 1, 0);
    ui->mainLayout->addWidget(sSlider, 2, 0);
    ui->mainLayout->addWidget(lSlider, 3, 0);
    ui->mainLayout->addWidget(openImageButton, 4, 0);
    ui->mainLayout->addWidget(graphicsView, 0, 1, 4, 1);

    connect(openImageButton, &QPushButton::clicked, this, &MainWindow::onOpenImageButtonClicked);
}
void MainWindow::onOpenImageButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.xpm *.jpg *.bmp)");
    if (fileName != "")
    {
        image.load(fileName);
        imageLabel->setPixmap(QPixmap::fromImage(image));
        graphicsScene->clear();
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        graphicsScene->addItem(item);

        hSlider->setMinimum(-180);
        hSlider->setMaximum(180);
        hSlider->setValue(0);
        sSlider->setValue(0);
        sSlider->setMinimum(-100);
        sSlider->setMaximum(100);
        lSlider->setValue(0);
        lSlider->setMinimum(-100);
        lSlider->setMaximum(100);
        connect(hSlider, &QSlider::valueChanged, this, &MainWindow::onHSLSliderChanged);
        connect(sSlider, &QSlider::valueChanged, this, &MainWindow::onHSLSliderChanged);
        connect(lSlider, &QSlider::valueChanged, this, &MainWindow::onHSLSliderChanged);
    }
}

void MainWindow::onHSLSliderChanged()
{
    updateImage();
}

void MainWindow::updateImage()
{
    QImage modifiedImage = image;
    int width = image.width();
    int height = image.height();
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            QColor color(image.pixel(x, y));
            int r = color.red();
            int g = color.green();
            int b = color.blue();
            double h, s, l;
            rgbToHsl(r, g, b, h, s, l);
            h += hSlider->value() / 360.0;
            s += sSlider->value() / 100.0;
            l += lSlider->value() / 100.0;
            h = fmod(h, 1.0);
            if (h < 0)
                h += 1.0;
            s = qBound(0.0, s, 1.0);
            l = qBound(0.0, l, 1.0);
            hslToRgb(h, s, l, r, g, b);
            modifiedImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    graphicsScene->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(modifiedImage));
    graphicsScene->addItem(item);
}
void MainWindow::rgbToHsl(int r, int g, int b, double &h, double &s, double &l)
{
    double rr = r / 255.0;
    double gg = g / 255.0;
    double bb = b / 255.0;
    double cmax = std::max({ rr, gg, bb });
    double cmin = std::min({ rr, gg, bb });
    double delta = cmax - cmin;
    if (delta == 0)
        h = 0;
    else if (cmax == rr)
        h = fmod((gg - bb) / delta, 6.0);
    else if (cmax == gg)
        h = (bb - rr) / delta + 2;
    else
        h = (rr - gg) / delta + 4;
    if (h < 0)
        h += 6;
    h /= 6;
    l = (cmax + cmin) / 2;
    if (delta == 0)
        s = 0;
    else
        s = delta / (1 - fabs(2 * l - 1));
}

void MainWindow::hslToRgb(double h, double s, double l, int &r, int &g, int &b)
{
    if (s == 0)
    {
        r = l * 255;
        g = l * 255;
        b = l * 255;
    }
    else
    {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hslToRgbHelper(p, q, h + 1.0 / 3.0) * 255;
        g = hslToRgbHelper(p, q, h) * 255;
        b = hslToRgbHelper(p, q, h - 1.0 / 3.0) * 255;
    }
}

double MainWindow::hslToRgbHelper(double p, double q, double t)
{
    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if (t < 1.0 / 6.0)
        return p + (q - p) * 6 * t;
    if (t < 1.0 / 2.0)
        return q;
    if (t < 2.0 / 3.0)
        return p + (q - p) * (2.0 / 3.0 - t) * 6;
    return p;
}
MainWindow::~MainWindow()
{
    delete ui;
}
