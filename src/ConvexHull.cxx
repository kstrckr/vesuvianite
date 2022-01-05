#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <iostream>
#include <vesuvianite/ConvexHull.hpp>
using namespace cv;
using namespace std;
int thresh = 100;
RNG rng(12345);
Mat src_gray;

namespace cvxHull
{
  void convexHull(cv::Mat &image)
  {

    cvtColor(image, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));
    const char *source_window = "Source";
    namedWindow(source_window);
    imshow(source_window, image);
    const int max_thresh = 255;
    createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);
    waitKey();
  }

  void thresh_callback(int, void *)
  {
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point>> contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> hull(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
      convexHull(contours[i], hull[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
      Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
      // drawContours(drawing, contours, (int)i, color);
      drawContours(drawing, hull, (int)i, color);
    }
    imshow("Hull demo", drawing);
  }
}