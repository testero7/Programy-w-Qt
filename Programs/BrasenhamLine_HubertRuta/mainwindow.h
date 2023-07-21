#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void bresenham();


private:
    Ui::MainWindow *ui;
    MainWindow *img;
    QRgb kolor;
    QPoint first;
    QPoint second;
    QImage obrazek;
    void mousePressEvent(QMouseEvent*);
    int sprawdzenie;

};
#endif // MAINWINDOW_H
