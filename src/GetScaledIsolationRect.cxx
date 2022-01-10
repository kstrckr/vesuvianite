#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>

using namespace cv;

float px;
float py;
cv::Size baseSize;
cv::RotatedRect scaledRotatedRect;
namespace GetScaledIsolationRect
{
  void saveWithPreviewRectangle(cv::Mat &image, cv::RotatedRect isolationRect)
  {
    baseSize = image.size();
    px = baseSize.width / 10;
    py = baseSize.height / 10;
    Point2f scaledCenter = Point2f(
      isolationRect.center.x / px * baseSize.width,
      isolationRect.center.y / py * baseSize.height
    );
    Size2f scaledSize = Size2f(
      (isolationRect.size.width / px * baseSize.width) + 0.15 * px,
      (isolationRect.size.height / py * baseSize.height) + 0.15 * py
    );
    scaledRotatedRect = RotatedRect(scaledCenter, scaledSize, isolationRect.angle);
    Scalar color = Scalar(255, 0, 0);
    Point2f scaled_points[4];
    scaledRotatedRect.points(scaled_points);
    for (int j = 0; j < 4; j++)
    {
      line(image, scaled_points[j], scaled_points[(j + 1) % 4], color);
    }
    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
  }
}