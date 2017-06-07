#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv/highgui.h>
#include <QFileDialog>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels,map<int,string>& corr , char separator);
    Mat lbp(Mat src);
    cv::Mat histogram(cv::Mat1b const& image);
    ~MainWindow();

protected:

    QString   FileOpName; //declare FileOpName as IplImage
    cv::Mat   imagerd;
    cv::Mat   imagerdlpb;
    QString   FichierCsv;
    Ptr<FaceRecognizer> model;
    vector<Mat> images;
    vector<int> labels;
    map<int,string> corresp;


private slots:
    void on_pushButton_clicked();
    void openImage();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
