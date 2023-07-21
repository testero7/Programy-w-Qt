#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QSlider>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QSlider *lSlider;
    QSlider *aSlider;
    QSlider *bSlider;
    QPushButton *openButton;
    QImage originalImage;
    void rgbtoxyz(int r, int g, int b, double &x, double &y, double &z);
    void xyztolab(double x, double y, double z, double &l, double &a, double &b);
    void labtoxyz(double l, double a, double b, double &x, double &y, double &z);
    void xyztorgb(double x, double y, double z, int &r, int &g, int &b);
private slots:
void openImage();
void updateImage();
};
#endif // MAINWINDOW_H
