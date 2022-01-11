#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <vesuvianite/IsolateSubject.hpp>

using namespace cv;
using namespace std;

int kernelSize = 13;
Mat shrunken, grey, blurred, blurred_grey, canny_output, contours, convex_hull, dst;
int threshold_value = 51;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
RNG rng(12345);
RotatedRect rect;
cv::Size rawSize;

namespace IsolateSubject
{
  cv::RotatedRect isolate(cv::Mat src)
  {
    rawSize = src.size();
    cv::Mat isolationSource(rawSize.height / 5, rawSize.width / 5, CV_8UC3);
    // cv::Mat blurred(100, 150, CV_8UC3);
    cv::resize(src, shrunken, isolationSource.size(), 0, 0, cv::INTER_CUBIC);
    // blurred = image.clone();
    cvtColor(shrunken, grey, COLOR_BGR2GRAY); // Convert the image to Gray
    GaussianBlur(grey, blurred_grey, Size(kernelSize, kernelSize), 0);
    // threshold( blurred_grey, dst, threshold_value, max_binary_value, threshold_type );
    Canny(blurred_grey, canny_output, threshold_value, threshold_value * 2);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<RotatedRect> minRect(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
      minRect[i] = minAreaRect(contours[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
      if (minRect[i].center.x > 60 && minRect[i].center.x < 90 || (minRect[i].size.width * minRect[i].size.height) >= ((100 * 150) / 4))
      {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        // contour
        drawContours(shrunken, contours, (int)i, color);
        // rotated rectangle
        Point2f rect_points[4];
        minRect[i].points(rect_points);
        for (int j = 0; j < 4; j++)
        {
          line(drawing, rect_points[j], rect_points[(j + 1) % 4], color);
        }
        // imshow("Contours", shrunken);
        // waitKey(0);
        return minRect[i];
      }
    }
    // imshow("Contours", shrunken);
    // waitKey(0);
    // namedWindow("IsolationSource", 1);
    // // imshow("IsolationSource", canny_output);
    // return canny_output;
  }
}
