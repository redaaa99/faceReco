#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QPixmap>
#include <QString>
#include <QDebug>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("C:/Users/mohammed/Desktop/marrakech.png"));
    this->setWindowTitle("Reconaissance Faciale");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{

    //declare FileOpName as the choosen opened file name
    FileOpName = QFileDialog::getOpenFileName(this,tr("Open File"), QDir::currentPath(),tr("Image Files (*.png *.jpg *.jpeg *.bmp *.pgm)"));
    qDebug() << FileOpName;
    //Check if FileOpName exist or not
    //function to load the image whenever fName is not empty
        if( FileOpName.size() )
        {
            imagerd = cv::imread(FileOpName.toUtf8().constData(),1);
            cv::resize(imagerd,imagerd,cv::Size(230,270));
            cv::cvtColor(imagerd,imagerd,CV_BGR2RGB);
            ui->label->setPixmap(QPixmap::fromImage(QImage(imagerd.data, 230, 270, imagerd.step, QImage::Format_RGB888)));
            imagerdlpb = lbp(imagerd);
            ui->label_2->setPixmap(QPixmap::fromImage(QImage(imagerdlpb.data, 230, 270, imagerdlpb.step, QImage::Format_Indexed8)));
            Mat histo = histogram(imagerdlpb);
            ui->label_3->setPixmap(QPixmap::fromImage(QImage(histo.data, 256, 256, histo.step, QImage::Format_RGB888)));
        }
}

void MainWindow::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels,map<int,string>& corr , char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        cout << "Erreur sur le fichier";
        exit(-1);
    }
    string line, path, classlabel,nameofperson;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel,separator);
        getline(liness, nameofperson);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path,0));
            labels.push_back(atoi(classlabel.c_str()));
            corr.insert ( std::pair<int,string>(atoi(classlabel.c_str()),nameofperson) );
        }
    }
}

Mat MainWindow::lbp(Mat src) {
    // get matrices
    cvtColor(src, src, CV_BGR2GRAY);
    Mat dst(src.rows,src.cols,CV_8UC1);
    for(int i=1;i<src.rows-1;i++) {
        for(int j=1;j<src.cols-1;j++) {
            uchar center = src.at<uchar>(i,j);
            unsigned char code = 0;
            code = code |(src.at<uchar>(i-1,j-1) >= center) << 7;
            code = code |(src.at<uchar>(i-1,j) >= center) << 6;
            code = code |(src.at<uchar>(i-1,j+1) >= center) << 5;
            code = code |(src.at<uchar>(i,j+1) >= center) << 4;
            code = code |(src.at<uchar>(i+1,j+1) >= center) << 3;
            code = code |(src.at<uchar>(i+1,j) >= center) << 2;
            code = code |(src.at<uchar>(i+1,j-1) >= center) << 1;
            code = code |(src.at<uchar>(i,j-1) >= center) << 0;
            dst.at<unsigned char>(i-1,j-1) = code;
        }
    }
    return dst;
}

cv::Mat MainWindow::histogram(cv::Mat1b const& image)
{
    // Set histogram bins count
    int bins = 256;
    int histSize[] = {bins};
    // Set ranges for histogram bins
    float lranges[] = {0, 256};
    const float* ranges[] = {lranges};
    // create matrix for histogram
    cv::Mat hist;
    int channels[] = {0};

    // create matrix for histogram visualization
    int const hist_height = 256;
    cv::Mat3b hist_image = cv::Mat3b::zeros(hist_height, bins);

    cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    double max_val=0;
    minMaxLoc(hist, 0, &max_val);

    // visualize each bin
    for(int b = 0; b < bins; b++) {
        float const binVal = hist.at<float>(b);
        int   const height = cvRound(binVal*hist_height/max_val);
        cv::line
            ( hist_image
            , cv::Point(b, hist_height-height), cv::Point(b, hist_height)
            , cv::Scalar::all(255)
            );
    }
    return hist_image;
}

void MainWindow::on_pushButton_clicked()
{
    openImage();
}

void MainWindow::on_pushButton_2_clicked()
{

    FichierCsv= QFileDialog::getOpenFileName(this,tr("Open File"), QDir::currentPath(),tr("Image Files (*.csv)"));
    string help = FichierCsv.toUtf8().constData();
    qDebug() << "Ca marche !! ";
    read_csv(help,this->images, this->labels,this->corresp,';');

    this->model = createLBPHFaceRecognizer();
    cv::cvtColor(imagerd,imagerd,CV_RGB2GRAY);
    qDebug() << "Ca marche !!222 ";
    this->model->train(images, labels);
}

void MainWindow::on_pushButton_3_clicked()
{
    int predictedLabel = model->predict(cv::imread(FileOpName.toUtf8().constData(),0));
    ui->label_4->setText("Resultat : "+QString::fromStdString(corresp[predictedLabel]));

}
