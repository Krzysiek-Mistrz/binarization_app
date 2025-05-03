#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsView>   

MainWindow::MainWindow() {
    // … utworzenie layout, widgetów, połączenie sygnałów …
    connect(sliderR, &QSlider::valueChanged, this, &MainWindow::updateManualThresholds);
    // analogicznie G, B
    connect(btnAuto, &QPushButton::clicked, this, &MainWindow::computeAutoThresholds);
}

void MainWindow::updateManualThresholds() {
    std::vector<cv::Mat> ch;
    cv::split(src, ch);
    int tR = sliderR->value(), tG = sliderG->value(), tB = sliderB->value();
    cv::threshold(ch[2], ch[2], tR, 255, cv::THRESH_BINARY);
    cv::threshold(ch[1], ch[1], tG, 255, cv::THRESH_BINARY);
    cv::threshold(ch[0], ch[0], tB, 255, cv::THRESH_BINARY);
    cv::merge(ch, processed);
    displayMat(processed);
}

void MainWindow::computeAutoThresholds() {
    std::vector<cv::Mat> ch;
    cv::split(src, ch);
    for(int i=0;i<3;i++){
        cv::threshold(ch[i], ch[i], 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    }
    cv::merge(ch, processed);
    displayMat(processed);
}

void MainWindow::displayMat(const cv::Mat &mat) {
    imageLabel->setPixmap(QPixmap::fromImage(matToQImage(mat)));
}
