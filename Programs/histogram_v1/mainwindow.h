#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
template<class T>
T clamp(T v,T minv,T maxv){
    if(v>=maxv)return maxv;
    if(v<=minv) return minv;
    return v;
}
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
explicit MainWindow(QWidget *parent = nullptr);
~MainWindow();

private slots:
    void openImage();
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);

private:
Ui::MainWindow *ui;
QImage image;
QImage processImage;
QImage hist_image;
};

#endif // MAINWINDOW_H
