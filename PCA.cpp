#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include<conio.h>
using namespace std;
using namespace cv;

void drw_xs(Mat&, Point, Point, Scalar, const float);
double get_orient(const vector<Point> &, Mat&);
void drw_xs(Mat& img, Point p, Point q, Scalar colour, const float scale = 0.2)
{
    double ang;
    double hypo;
    ang = atan2( (double) p.y - q.y, (double) p.x - q.x ); 
    hypo = sqrt( (double) (p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));


    q.x = (int) (p.x - scale * hypo * cos(ang));
    q.y = (int) (p.y - scale * hypo * sin(ang));
    line(img, p, q, colour, 1, LINE_AA);
    

    p.x = (int)(q.x + 9 * cos(ang + CV_PI/4));
    p.y = (int)(q.y + 9 * sin(ang + CV_PI/4));
    line(img, p, q, colour, 1, LINE_AA);
    p.x = (int)(q.x + 9 * cos(ang - CV_PI/4));
    p.y = (int)(q.y + 9 * sin(ang - CV_PI/4));
    line(img, p, q, colour, 1, LINE_AA);
}
double get_orient(const vector<Point> &pts, Mat &img)
{
    
    int sz = static_cast<int>(pts.size());
    Mat data_pts = Mat(sz, 2, CV_64FC1);
    for (int i = 0; i < data_pts.rows; ++i)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }
     
    PCA pca(data_pts, Mat(), PCA::DATA_AS_ROW);
     
    Point cntr = Point(static_cast<int>(pca.mean.at<double>(0, 0)),
                      static_cast<int>(pca.mean.at<double>(0, 1)));
     
    vector<Point2d> eigen_vecs(2);
    vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i)
    {
        eigen_vecs[i] = Point2d(pca.eigenvectors.at<double>(i, 0),
                                pca.eigenvectors.at<double>(i, 1));
        eigen_val[i] = pca.eigenvalues.at<double>(i);
    }
     
    circle(img, cntr, 3, Scalar(255, 0, 255), 2);
    Point p1 = cntr + 0.02 * Point(static_cast<int>(eigen_vecs[0].x * eigen_val[0]), static_cast<int>(eigen_vecs[0].y * eigen_val[0]));
    Point p2 = cntr - 0.02 * Point(static_cast<int>(eigen_vecs[1].x * eigen_val[1]), static_cast<int>(eigen_vecs[1].y * eigen_val[1]));
    drw_xs(img, cntr, p1, Scalar(0, 255, 0), 1);
    drw_xs(img, cntr, p2, Scalar(255, 255, 0), 5);
    double ang = atan2(eigen_vecs[0].y, eigen_vecs[0].x);  
    return ang;
}
int main()
{
    
	cv::Mat imgOriginal;

	imgOriginal = cv::imread("hr.jpg");

	Mat src = imgOriginal;
    

    if(src.empty())
    {
        cout << "The image cannot be  loaded" << endl;
        return EXIT_FAILURE;
    }

    imshow("src", src);
    
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    
    Mat bw;
    threshold(gray, bw, 50, 255, THRESH_BINARY | THRESH_OTSU);
     
    vector<vector<Point> > contours;
    findContours(bw, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for (size_t i = 0; i < contours.size(); ++i)
    {
         
        double area = contourArea(contours[i]);
        
        if (area < 1e2 || 1e5 < area) 
			continue;
        
        drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 2, LINE_8);
        
        get_orient(contours[i], src);
    }
    imshow("result", src);
    waitKey(0);
    return 0;
}