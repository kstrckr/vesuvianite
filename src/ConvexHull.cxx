#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vesuvianite/ConvexHull.hpp>

using namespace cv;
using namespace std;

namespace cvxHull
{
  int kernelSize = 11;
  Mat shrunken, grey, src_gray, blurred, blurred_grey, canny_output, contours, convex_hull, dst;
  int threshold_value = 45;
  int threshold_type = 0;
  int const max_value = 255;
  int const max_type = 4;
  int const max_binary_value = 255;
  RNG rng(12345);

  void convexHull(Placement pd, cv::Mat &image)
  {
    Mat isolatedROI(image, pd.isolationBoundingRect);
    cvtColor(isolatedROI, src_gray, COLOR_BGR2GRAY);
    GaussianBlur(src_gray, blurred_grey, Size(kernelSize, kernelSize), 0);
    const char *source_window = "Source";
    // namedWindow(source_window);
    // imshow(source_window, isolatedROI);
    waitKey(0);
    const int max_thresh = 255;

    Mat canny_output;
    Canny(blurred_grey, canny_output, threshold_value, threshold_value * 2);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> hull(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
      convexHull(contours[i], hull[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
      Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
      // Scalar color = Scalar(0, 255, 0, 128);

      // drawContours(isolatedROI, contours, (int)i, color, 5);
      // convexHull(contours[i], hull[i]);
      // drawContours(isolatedROI, hull, (int)i, color, 2);
    }
    // imshow("Hull demo", isolatedROI);

    // waitKey(0);
    // find the one without any parents
    for (size_t i = 0; i < hierarchy.size(); i++)
    {
      if (hierarchy[i][3] == -1)
      {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        // drawContours(isolatedROI, contours, (int)i, color, 5);
        convexHull(contours[i], hull[i]);
        RotatedRect rotatedRect = minAreaRect(hull[i]);

        // drawContours(isolatedROI, hull, (int)i, Scalar(0, 0, 255), 5);
        if (rotatedRect.size.area() > pd.isolationBoundingRect.area() * 0.75)
        {
          Point2f rect_points[4];
          rotatedRect.points(rect_points);
          for (int j = 0; j < 4; j++)
          {
            line(isolatedROI, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 2);
          }
          auto now = std::chrono::system_clock::now();
          auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
          auto epoch = now_ms.time_since_epoch();
          auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
          long duration = value.count();
          std::string timeString = std::to_string(duration);
          std::string outputPath = "/home/a/proj/vesuvianite/output_images/";
          imwrite(outputPath + timeString + ".jpg", isolatedROI);
          return;
        }
      }
    }
  }
}
