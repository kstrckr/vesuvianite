#include <stdio.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vesuvianite/RawLoader.hpp>
using namespace std;
using namespace cv;

namespace imageProc {
  cv::Mat loadRaw() {

    cv::Mat image;
    cv::Mat downsizedImage(1158, 1736, CV_8UC3);

    image = cv::imread( "/home/a/proj/vesuvianite/ideal-target-batch-1/CD.32.2197.cr2" );
    if ( !image.data )
    {
        printf("No image data \n");
    }

    cv::resize(image, downsizedImage, downsizedImage.size(), 0, 0, cv::INTER_CUBIC);
    namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", downsizedImage);
    cv::waitKey(0);

    return downsizedImage;
  }
} // namespace lib
