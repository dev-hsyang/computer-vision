#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main( int argc, char* argv[] ){

    int descriptorSize = 250;
    int imSize = 2000;
    double pointRematchThreshold = 3.9;
    double pDistance = 0;
    double minDist = 1;
    double maxDist = 0;

    Mat im1 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/3차과제/201721094양현승_3차과제ImageStitching/im1.jpg");
    Mat im2 = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/3차과제/201721094양현승_3차과제ImageStitching/im2.jpg");
    Mat featurePoints_matched;
    Mat descriptors1, descriptors2;
    Mat rematchedPoints;
    Mat H;
    Mat im_warp;
    Mat im_pan;
    Mat im_fin;

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    for (int i = 0; i < 4; i++) {
        if (im1.rows > imSize || im2.cols > imSize)
            pyrDown(im1, im1);
        if (im2.rows > imSize || im2.cols > imSize)
            pyrDown(im2, im2);
    }
    //IPHONE12PROMAX 기준 사진이 너무 크므로 PYRDOWN

    Ptr<SURF> detector = SURF::create( descriptorSize );
    vector<KeyPoint> featurePoints1, featurePoints2;
    detector->detectAndCompute( im1, noArray(), featurePoints1, descriptors1 );
    detector->detectAndCompute( im2, noArray(), featurePoints2, descriptors2 );
    //***********1.featurepoints detection, descriptor 생성
    
    
    //-- Step 2: Matching descriptor vectors with a brute force matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    vector< DMatch > matches;
    matcher->match( descriptors1, descriptors2, matches );
    //-- Draw matches
    //***********2.descriptor matching

    drawMatches( im1, featurePoints1, im2, featurePoints2, matches, featurePoints_matched );
    imshow("Matched points", featurePoints_matched );
    //waitKey();
    //-- Show detected matches
    //***********3.drawMatches

 
    for (int i = 0; i < descriptors1.rows; i++) {
        pDistance = matches[i].distance;
        if (pDistance < minDist)  minDist = pDistance;
        if (pDistance > maxDist)  maxDist = pDistance;
    }
    cout << "Max Distance is " << maxDist << endl;
    cout << "Min Distance is " << minDist << endl;
    //***********4.feature points간 거리 계산

    vector<DMatch> reMatch;
    for (size_t i = 0; i < descriptors1.rows; i++) {
        if (matches[i].distance < pointRematchThreshold * minDist)
            reMatch.push_back(matches[i]);
    }
    drawMatches(im1, featurePoints1, im2, featurePoints2, reMatch, rematchedPoints);
    //***********5.보다 정확한 feature points 찾아냄
    imshow("rematched points", rematchedPoints);

    vector<Point2f> src;
    vector<Point2f> dst;
    for (size_t i = 0; i < reMatch.size(); i++) {
        src.push_back(featurePoints1[reMatch[i].queryIdx].pt);
        dst.push_back(featurePoints2[reMatch[i].trainIdx].pt);
    }
    H = findHomography(dst, src, RANSAC);
    //***********6.Find H

    warpPerspective(im2, im_warp, H, Size(im1.cols + im2.cols, im2.rows));
    im_pan = im_warp.clone();
    im_fin = im_pan(Rect(0, 0, im1.cols, im1.rows));
    im1.copyTo(im_fin);
    //**********7.image하나는 warping, warp안한 나머지 image copy
    imshow("warped image", im_warp);

    vector<Point> Fin(im_warp.rows * im_warp.cols);
    for (size_t i = 0; i < im_pan.rows; i++) {
        for (size_t j = 0; j < im_pan.cols; j++) {
            if (im_pan.at<Vec3b>(i, j) != Vec3b(0, 0, 0))
                Fin.push_back(Point(j, i));
        }
    }
    //***********8.warped된 image와 나머지 image를 stitching
    imshow("panorama", im_pan);
    //결과출력
    waitKey();
    return 0;
}
