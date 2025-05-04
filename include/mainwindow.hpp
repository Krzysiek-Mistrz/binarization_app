#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
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
    QLabel *labelR, *labelG, *labelB;
    QPushButton *btnManual, *btnAuto, *btnLoad;
    QTimer *throttleTimer;
    QFutureWatcher<cv::Mat> watcher;
    void displayMat(const cv::Mat &mat);
};

#endif