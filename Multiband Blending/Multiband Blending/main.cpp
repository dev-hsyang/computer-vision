
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
const int PYR_LEVEL = 5;

std::vector<Mat> buildGaussianPyramid(const Mat& src) {
    std::vector<Mat> GaussianPyramid(PYR_LEVEL);

    GaussianPyramid[0] = src;
    for (auto i = 1; i < PYR_LEVEL; i++) {
        pyrDown(GaussianPyramid[i - 1], GaussianPyramid[i]);
    }
    return GaussianPyramid;
}

std::vector<Mat> buildLaplacianPyramid(const Mat& src) {
    std::vector<Mat> LaplacianPyramid(PYR_LEVEL);

    LaplacianPyramid[0] = src.clone();

    for (auto i = 1; i < PYR_LEVEL; i++) {
        pyrDown(LaplacianPyramid[i - 1], LaplacianPyramid[i]);
        Mat temp;
        pyrUp(LaplacianPyramid[i], temp, LaplacianPyramid[i - 1].size());
        LaplacianPyramid[i - 1] -= temp;
    }
    return LaplacianPyramid;
}

Mat reconstruct(const std::vector<Mat>& pyramid) {
    Mat out = pyramid.back();

    for (int i = int(pyramid.size()) - 2; i >= 0; i--) {
        pyrUp(out, out, pyramid[i].size());
        out += pyramid[i];
    }
    return out;
}


int main(int argc, const char* argv[]) {

    Mat inputmask = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/1차과제/burt_mask.png");
    Mat inputapple = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/1차과제/burt_apple.png");
    Mat inputorange = imread("/Users/hyunseung/포트폴리오/컴퓨터비전/과제/1차과제/burt_orange.png");

    Mat mask, apple, orange, maskO;
    
    inputmask.convertTo(mask, CV_32F, 1 / 255.f); inputapple.convertTo(apple, CV_32F, 1 / 255.f); inputorange.convertTo(orange, CV_32F, 1 / 255.f);
    
    std::vector<Mat> maskGP = buildGaussianPyramid(mask);
    std::vector<Mat> appleLP = buildLaplacianPyramid(apple);
    std::vector<Mat> appleLpxmaskGp(PYR_LEVEL);
 
    for (int i = 0; i < PYR_LEVEL; i++) {
        multiply(maskGP[i], appleLP[i], appleLpxmaskGp[i]);
    }

    Mat appleMasked = reconstruct(appleLpxmaskGp);

    subtract(1, mask, maskO);
    std::vector<Mat> maskOGP = buildGaussianPyramid(maskO);
    std::vector<Mat> orangeLP = buildLaplacianPyramid(orange);
    std::vector<Mat> orangeLpxmaskOGp(PYR_LEVEL);

    for (int i = 0; i < PYR_LEVEL; i++) {
        multiply(maskOGP[i], orangeLP[i], orangeLpxmaskOGp[i]);
    }
    Mat orangedMasked = reconstruct(orangeLpxmaskOGp);

    std::vector<Mat> MergedImage(PYR_LEVEL);

    for (int i = 0; i < PYR_LEVEL; i++) {
        add(appleLpxmaskGp[i], orangeLpxmaskOGp[i], MergedImage[i]);
    }
    
    Mat output = reconstruct(MergedImage);

    //imshow("maskO", maskO);
    //imshow("mask", appleMasked);
    //imshow("mask2", orangedMasked);
    imshow("output", output);
    waitKey();
    return 0;
}
