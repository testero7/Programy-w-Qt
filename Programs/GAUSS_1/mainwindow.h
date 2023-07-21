#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QLabel>

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
    void on_browseButton_clicked();
    void on_gaussianButton_clicked();
    void on_unsharpButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage image;
    QPixmap pixmap;
    QLabel *imageLabel;

    void applyGaussianFilter();
    void applyUnsharpMask();
};

#endif // MAINWINDOW_H
