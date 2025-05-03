#include "mainwindow.hpp"
#include <QDebug>
#include <QGraphicsView>   


QImage MainWindow::matToQImage(const cv::Mat &mat) {
    cv::Mat rgb;
    // Konwersja BGR→RGB
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    // Tworzymy QImage kopiując dane
    return QImage((const uchar*)rgb.data, rgb.cols, rgb.rows,
                  static_cast<int>(rgb.step),
                  QImage::Format_RGB888).copy();
}

void MainWindow::loadImage() {
    QString file = QFileDialog::getOpenFileName(this, "Wybierz obraz");
    if(file.isEmpty()) return;
    cv::Mat img = cv::imread(file.toStdString());
    if(img.empty()) qWarning() << "Nie udało się wczytać obrazu";
    src = img;
    displayMat(src);
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {
    // 1) Tworzymy widżety
    imageLabel = new QLabel(this);
    sliderR    = new QSlider(Qt::Horizontal, this);
    sliderG    = new QSlider(Qt::Horizontal, this);
    sliderB    = new QSlider(Qt::Horizontal, this);
    btnLoad    = new QPushButton("Load Image", this);
    btnManual  = new QPushButton("Manual", this);
    btnAuto    = new QPushButton("Auto", this);

    // 2) Zakresy suwaków
    sliderR->setRange(0,255);
    sliderG->setRange(0,255);
    sliderB->setRange(0,255);

    // 3) Układ
    auto *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(btnLoad);
    controlsLayout->addWidget(sliderR);
    controlsLayout->addWidget(sliderG);
    controlsLayout->addWidget(sliderB);
    controlsLayout->addWidget(btnManual);
    controlsLayout->addWidget(btnAuto);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(controlsLayout);

    auto *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    //laczenie sygnalow
    connect(btnLoad, &QPushButton::clicked, this, &QMainWindow::loadImage);
    connect(sliderR, &QSlider::valueChanged, this, &MainWindow::updateManualThresholds);
    connect(sliderG, &QSlider::valueChanged, this, &MainWindow::updateManualThresholds);
    connect(sliderB, &QSlider::valueChanged, this, &MainWindow::updateManualThresholds);
    connect(btnAuto, &QPushButton::clicked, this, &MainWindow::computeAutoThresholds);
}

MainWindow::~MainWindow() { }

void MainWindow::updateManualThresholds() {
    std::vector<cv::Mat> ch;
    cv::split(src, ch);
    cv::threshold(ch[2], ch[2], sliderR->value(), 255, cv::THRESH_BINARY);
    cv::threshold(ch[1], ch[1], sliderG->value(), 255, cv::THRESH_BINARY);
    cv::threshold(ch[0], ch[0], sliderB->value(), 255, cv::THRESH_BINARY);
    cv::merge(ch, processed);
    displayMat(processed);
}

void MainWindow::computeAutoThresholds() {
    std::vector<cv::Mat> ch;
    cv::split(src, ch);
    for(int i=0; i<3; ++i)
        cv::threshold(ch[i], ch[i], 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    cv::merge(ch, processed);
    displayMat(processed);
}

void MainWindow::displayMat(const cv::Mat &mat) {
    imageLabel->setPixmap(QPixmap::fromImage(matToQImage(mat)));
}