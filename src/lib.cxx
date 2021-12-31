#include <stdio.h>
#include <opencv4/opencv2/opencv.hpp>
#include <vesuvianite/lib.hpp>
using namespace std;

namespace lib {
  int loadRaw() {

    cv::Mat image;
    image = cv::imread( "/home/a/proj/vesuvianite/ideal-target-batch-1/51A_8_verso.cr2" );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
  }
} // namespace lib
