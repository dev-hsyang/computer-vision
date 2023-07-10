#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
 
using namespace cv;
using namespace std;
int min_dst = 30; //검출할 원의 최소거리
int parameter1 = 200; //Canny edge detection에서 높은 threshold의 값 180,100:7:::::200,65
int parameter2 = 56; //원 검출을 위한 정보, accumulator의 threshold 값
int min_Radius = 17;
int max_Radius = 90;

int main()
{
    std::cout << "Hello World!\n";
    Mat coins0 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/2차과제/coins0.jpg", 0);
    Mat coins1 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/2차과제/coins1.jpg", 0);
    Mat coins2 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/2차과제/coins2.jpg", 0);
    Mat coins3 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/2차과제/coins3.jpg", 0);
    Mat coins4 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/2차과제/coins4.jpg", 0);
    Mat coins5 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/2차과제/coins5.jpg", 0);
    GaussianBlur(coins0, coins0, Size(5, 5), 0);
    GaussianBlur(coins1, coins1, Size(5, 5), 0);
    GaussianBlur(coins2, coins2, Size(5, 5), 0);
    GaussianBlur(coins3, coins3, Size(5, 5), 0);
    GaussianBlur(coins4, coins4, Size(5, 5), 0);
    GaussianBlur(coins5, coins5, Size(5, 5), 0);
    //Canny(coins0, coins0, 130, 170);
    
    Mat coins0_houghC, coins1_houghC, coins2_houghC, coins3_houghC, coins4_houghC, coins5_houghC;
    
    coins0.copyTo(coins0_houghC);
    coins1.copyTo(coins1_houghC);
    coins2.copyTo(coins2_houghC);
    coins3.copyTo(coins3_houghC);
    coins4.copyTo(coins4_houghC);
    coins5.copyTo(coins5_houghC);
    vector<Vec3f> circles, circles2, circles3, circles4, circles5, circles6;
    HoughCircles(coins0_houghC, circles, HOUGH_GRADIENT, 1, min_dst, parameter1, parameter2, min_Radius, max_Radius);
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        Point center(c[0], c[1]);
        int radius = c[2];

        circle(coins0_houghC, center, radius, Scalar(0, 255, 0), 2);
        circle(coins0_houghC, center, 2, Scalar(0, 0, 255), 3);
    }
    HoughCircles(coins1_houghC, circles2, HOUGH_GRADIENT, 1, min_dst, parameter1, parameter2, min_Radius, max_Radius);
    for (size_t i = 0; i < circles2.size(); i++) {
        Vec3i c = circles2[i];
        Point center(c[0], c[1]);
        int radius = c[2];

        circle(coins1_houghC, center, radius, Scalar(0, 255, 0), 2);
        circle(coins1_houghC, center, 2, Scalar(0, 0, 255), 3);
    }
    HoughCircles(coins2_houghC, circles3, HOUGH_GRADIENT, 1, min_dst, parameter1, parameter2, min_Radius, max_Radius);
    for (size_t i = 0; i < circles3.size(); i++) {
        Vec3i c = circles3[i];
        Point center(c[0], c[1]);
        int radius = c[2];

        circle(coins2_houghC, center, radius, Scalar(0, 255, 0), 2);
        circle(coins2_houghC, center, 2, Scalar(0, 0, 255), 3);
    }
    HoughCircles(coins3_houghC, circles4, HOUGH_GRADIENT, 1, min_dst, parameter1, parameter2, min_Radius, max_Radius);
    for (size_t i = 0; i < circles4.size(); i++) {
        Vec3i c = circles4[i];
        Point center(c[0], c[1]);
        int radius = c[2];

        circle(coins3_houghC, center, radius, Scalar(0, 255, 0), 2);
        circle(coins3_houghC, center, 2, Scalar(0, 0, 255), 3);
    }
    HoughCircles(coins4_houghC, circles5, HOUGH_GRADIENT, 1, min_dst, parameter1, parameter2, min_Radius, max_Radius);
    HoughCircles(coins5_houghC, circles6, HOUGH_GRADIENT, 1, min_dst, parameter1, parameter2, min_Radius, max_Radius);

    imshow("coins0_hough", coins0_houghC);
    imshow("coins1_hough", coins1_houghC);
    imshow("coins2_hough", coins2_houghC);
    imshow("coins3_hough", coins3_houghC);
//    imshow("coins4_hough", coins4_houghC);
//    imshow("coins5_hough", coins5_houghC);
    //imshow("coins1", coins1);
    // imshow("coins1_hough", coins1_houghC);
    cout << "coins0:" << circles.size() << "    expected:10" << endl;
    cout << "coins1:" << circles2.size() << "   expected:13" << endl;
    cout << "coins2:" << circles3.size() << "   expected:9" << endl;
    cout << "coins3:" << circles4.size() << "   expected:15" << endl;
    cout << "coins4:" << circles5.size() << "   expected:16" << endl;
    cout << "coins5:" << circles6.size() << "   expected:8" << endl;
    waitKey();
    return 0;
}
