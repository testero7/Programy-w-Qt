#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    openButton = new QPushButton(tr("Open Image"), this);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openImage);

    lSlider = new QSlider(Qt::Horizontal, this);
    lSlider->setRange(-10, 10);
    connect(lSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    aSlider = new QSlider(Qt::Horizontal, this);
    aSlider->setRange(-64, 64);
    connect(aSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    bSlider = new QSlider(Qt::Horizontal, this);
    bSlider->setRange(-64, 64);
    connect(bSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);

    QLabel *lLabel = new QLabel("L", this);
    QLabel *aLabel = new QLabel("A", this);
    QLabel *bLabel = new QLabel("B", this);

    QHBoxLayout *lLayout = new QHBoxLayout;
    lLayout->addWidget(lLabel);
    lLayout->addWidget(lSlider);

    QHBoxLayout *aLayout = new QHBoxLayout;
    aLayout->addWidget(aLabel);
    aLayout->addWidget(aSlider);

    QHBoxLayout *bLayout = new QHBoxLayout;
    bLayout->addWidget(bLabel);
    bLayout->addWidget(bSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(openButton);
    mainLayout->addLayout(lLayout);
    mainLayout->addLayout(aLayout);
    mainLayout->addLayout(bLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setWindowTitle(tr("Image Converter"));
    resize(800, 600);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openImage()
{
QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Images (*.png *.xpm *.jpg *.bmp)"));
if (!fileName.isEmpty()) {
originalImage = QImage(fileName);
updateImage();
}
}

void MainWindow::updateImage()
{
if (originalImage.isNull())
return;
QImage modifiedImage = originalImage;
int width = modifiedImage.width();
int height = modifiedImage.height();

for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
        QRgb pixel = modifiedImage.pixel(i, j);
        int r = qRed(pixel);
        int g = qGreen(pixel);
        int b = qBlue(pixel);

        double x, y, z;
        rgbtoxyz(r, g, b, x, y, z);

        double l, a, b_;
        xyztolab(x, y, z, l, a, b_);

        l += lSlider->value();
        a += aSlider->value();
        b_ += bSlider->value();

        labtoxyz(l, a, b_, x, y, z);

        int r_, g_, b_1;
        xyztorgb(x, y, z, r_, g_, b_1);

        QRgb newPixel = qRgb(r_, g_, b_1);
        modifiedImage.setPixel(i, j, newPixel);
    }
}

imageLabel->setPixmap(QPixmap::fromImage(modifiedImage));
}
void MainWindow::rgbtoxyz(int r, int g, int b, double &x, double &y, double &z)
{
// konwersja na zakres [0, 1]
double r_ = r / 255.0;
double g_ = g / 255.0;
double b_ = b / 255.0;
// rgb to xyz macierz
x = r_ * 0.4124 + g_ * 0.3576 + b_ * 0.1805;
y = r_ * 0.2126 + g_ * 0.7152 + b_ * 0.0722;
z = r_ * 0.0193 + g_ * 0.1192 + b_ * 0.9505;
}

void MainWindow::xyztolab(double x, double y, double z, double &l, double &a, double &b)
{
// xyz to lab algorytm konwersji
double x_ = x / 95.047;
double y_ = y / 100.0;
double z_ = z / 108.883;
x_ = (x_ > 0.008856) ? pow(x_, (1.0/3.0)) : (7.787 * x_) + (16.0/116.0);
y_ = (y_ > 0.008856) ? pow(y_, (1.0/3.0)) : (7.787 * y_) + (16.0/116.0);
z_ = (z_ > 0.008856) ? pow(z_, (1.0/3.0)) : (7.787 * z_) + (16.0/116.0);

l = (116.0 * y_) - 16.0;
a = 500.0 * (x_ - y_);
b = 200.0 * (y_ - z_);
}

void MainWindow::labtoxyz(double l, double a, double b, double &x, double &y, double &z)
{
// lab to xyz algorytm konwersji
double y_ = (l + 16.0) / 116.0;
double x_ = a / 500.0 + y_;
double z_ = y_ - b / 200.0;
double x3 = pow(x_, 3);
double y3 = pow(y_, 3);
double z3 = pow(z_, 3);

x = (x3 > 0.008856) ? x3 : (x_ - 16.0/116.0) / 7.787;
y = (y3 > 0.008856) ? y3 : (y_ - 16.0/116.0) / 7.787;
z = (z3 > 0.008856) ? z3 : (z_ - 16.0/116.0) / 7.787;

x *= 95.047;
y *= 100.0;
z *= 108.883;
}

void MainWindow::xyztorgb(double x, double y, double z, int &r, int &g, int &b)
{
// macierz konwersji xyz to rgb
    double r_ = x * 3.2406 + y * -1.5372 + z * -0.4986;
    double g_ = x * -0.9689 + y * 1.8758 + z * 0.0415;
    double b_ = x * 0.0557 + y * -0.2040 + z * 1.0570;

    // clamp do zakresu [0,1]
    r_ = std::max(0.0, std::min(1.0, r_));
    g_ = std::max(0.0, std::min(1.0, g_));
    b_ = std::max(0.0, std::min(1.0, b_));

    // konwersja rgb spowrotem na zakres [0, 255]
    r = static_cast<int>(r_ * 255.0 + 0.5);
    g = static_cast<int>(g_ * 255.0 + 0.5);
    b = static_cast<int>(b_ * 255.0 +0.5);
}
