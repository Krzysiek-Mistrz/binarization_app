#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage matToQImage(const cv::Mat &mat);
private slots:
    void loadImage();
    void updateManualThresholds();
    void computeAutoThresholds();
private:
    cv::Mat src, processed;
    QLabel *imageLabel;
    QSlider *sliderR, *sliderG, *sliderB;
    QPushButton *btnManual, *btnAuto;
    void displayMat(const cv::Mat &mat);
};

#endif