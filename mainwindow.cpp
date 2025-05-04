#include "mainwindow.hpp"
#include <QDebug>
#include <QGraphicsView>


QImage MainWindow::matToQImage(const cv::Mat &mat) {
    cv::Mat rgb;
    //konwersja i tworzenie qimage
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage((const uchar*)rgb.data, rgb.cols, rgb.rows,
                  static_cast<int>(rgb.step),
                  QImage::Format_RGB888).copy();
}

void MainWindow::loadImage() {
    QString file = QFileDialog::getOpenFileName(this, "Select your image");
    if(file.isEmpty()) return;
    cv::Mat img = cv::imread(file.toStdString());
    if(img.empty()) qWarning() << "Couldn't load image";
    src = img;
    sliderR->setEnabled(true);
    sliderG->setEnabled(true);
    sliderB->setEnabled(true);
    btnManual->setEnabled(true);
    btnAuto->setEnabled(true);
    displayMat(src);
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {
    //instancje widgetow
    imageLabel = new QLabel(this);
    sliderR    = new QSlider(Qt::Horizontal, this);
    sliderG    = new QSlider(Qt::Horizontal, this);
    sliderB    = new QSlider(Qt::Horizontal, this);
    labelR     = new QLabel("R: 0", this);
    labelG     = new QLabel("G: 0", this);
    labelB     = new QLabel("B: 0", this);
    btnManual  = new QPushButton("Manual", this);
    btnAuto    = new QPushButton("Auto", this);
    btnLoad    = new QPushButton("Load Image", this);

    sliderR->setRange(0,255);
    sliderG->setRange(0,255);
    sliderB->setRange(0,255);

    //ukl poziomy
    auto *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(btnLoad);
    controlsLayout->addWidget(sliderR);
    controlsLayout->addWidget(labelR);
    controlsLayout->addWidget(sliderG);
    controlsLayout->addWidget(labelG);
    controlsLayout->addWidget(sliderB);
    controlsLayout->addWidget(labelB);
    controlsLayout->addWidget(btnManual);
    controlsLayout->addWidget(btnAuto);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(controlsLayout);

    auto *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    sliderR->setEnabled(false);
    sliderG->setEnabled(false);
    sliderB->setEnabled(false);
    btnManual->setEnabled(false);
    btnAuto->setEnabled(false);

    //laczenie sygnalow
    throttleTimer = new QTimer(this);
    throttleTimer->setSingleShot(true);
    throttleTimer->setInterval(100);
    connect(throttleTimer, &QTimer::timeout, this, &MainWindow::updateManualThresholds);
    connect(btnLoad,  &QPushButton::clicked, this, &MainWindow::loadImage);           // :contentReference[oaicite:2]{index=2}
    connect(sliderR, &QSlider::valueChanged, throttleTimer, static_cast<void(QTimer::*)()>(&QTimer::start));
    connect(sliderG, &QSlider::valueChanged, throttleTimer, static_cast<void(QTimer::*)()>(&QTimer::start));
    connect(sliderB, &QSlider::valueChanged, throttleTimer, static_cast<void(QTimer::*)()>(&QTimer::start));
    connect(btnAuto,  &QPushButton::clicked, this, &MainWindow::computeAutoThresholds);
    connect(&watcher, &QFutureWatcher<cv::Mat>::finished, this, [this]() {
        processed = watcher.result();
        displayMat(processed);
    });
}

MainWindow::~MainWindow() { }

void MainWindow::updateManualThresholds() {
    if (src.empty()) return;
    int r=sliderR->value(), g=sliderG->value(), b=sliderB->value();
    labelR->setText(QString("R:%1").arg(r));
    labelG->setText(QString("G:%1").arg(g));
    labelB->setText(QString("B:%1").arg(b));
    cv::Mat input = src.clone();
    watcher.setFuture(QtConcurrent::run([=]() {
        cv::Mat mask, out;
        cv::inRange(input, cv::Scalar(b,g,r), cv::Scalar(255,255,255), mask);
        cv::bitwise_and(input, input, out, mask);
        return out;
    }));
}


void MainWindow::computeAutoThresholds() {
    if (src.empty()) {
        return;
    }    
    std::vector<cv::Mat> ch(3);
    cv::split(src, ch);
    int otsuR = cv::threshold(ch[2], ch[2], 0,255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    int otsuG = cv::threshold(ch[1], ch[1], 0,255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    int otsuB = cv::threshold(ch[0], ch[0], 0,255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    sliderR->setValue(otsuR);  sliderG->setValue(otsuG);  sliderB->setValue(otsuB);
    labelR->setText(QString("R: %1").arg(otsuR));
    labelG->setText(QString("G: %1").arg(otsuG));
    labelB->setText(QString("B: %1").arg(otsuB));
    cv::Mat mask;
    cv::inRange(src, cv::Scalar(otsuB,otsuG,otsuR), cv::Scalar(255,255,255), mask);
    cv::bitwise_and(src, src, processed, mask);
    displayMat(processed);
}

void MainWindow::displayMat(const cv::Mat &mat) {
    imageLabel->setPixmap(QPixmap::fromImage(matToQImage(mat)));
}