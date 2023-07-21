#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenImageButtonClicked();
    void onHSLSliderChanged();

private:
    Ui::MainWindow *ui;

    QLabel *imageLabel;
    QSlider *hSlider;
    QSlider *sSlider;
    QSlider *lSlider;
    QPushButton *openImageButton;
    QImage image;
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;

    void updateImage();
    void rgbToHsl(int r, int g, int b, double &h, double &s, double &l);
    void hslToRgb(double h, double s, double l, int &r, int &g, int &b);
    double hslToRgbHelper(double p, double q, double t);
};

#endif // MAINWINDOW_H
