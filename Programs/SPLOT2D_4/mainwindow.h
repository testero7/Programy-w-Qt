#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
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
    void onOpenImage();
    void maska1();
    void maska2();

private:
    Ui::MainWindow *ui;
    QLabel *m_imageLabel;
    QImage m_image;
    QPixmap m_pixmap;
    };
    #endif // MAINWINDOW_H

