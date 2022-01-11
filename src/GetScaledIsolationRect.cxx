#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>
// #include <vesuvianite/SubjectPlacement.hpp>

using namespace cv;

float px;
float py;
cv::Size baseSize;
cv::RotatedRect scaledRotatedRect;
Placement scaledRects;
namespace GetScaledIsolationRect
{
  Placement getScaledRectAndBound(cv::Mat &image, cv::RotatedRect isolationRect)
  {
    baseSize = image.size();
    px = baseSize.width / 5;
    py = baseSize.height / 5;
    Point2f scaledCenter = Point2f(
      isolationRect.center.x / px * baseSize.width,
      isolationRect.center.y / py * baseSize.height
    );
    Size2f scaledSize = Size2f(
      (isolationRect.size.width / px * baseSize.width) + 0.05 * px,
      (isolationRect.size.height / py * baseSize.height) + 0.05 * py
    );
    scaledRotatedRect = RotatedRect(scaledCenter, scaledSize, isolationRect.angle);
    Scalar color = Scalar(255, 0, 0);
    Point2f scaled_points[4];
    scaledRotatedRect.points(scaled_points);
    for (int j = 0; j < 4; j++)
    {
      line(image, scaled_points[j], scaled_points[(j + 1) % 4], color);
    }
    Rect boundingRect = scaledRotatedRect.boundingRect();
    rectangle(image, boundingRect, Scalar(0, 255, 0), 2);
    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
    
    scaledRects.isolationRect = scaledRotatedRect;
    scaledRects.isolationBoundingRect = boundingRect;
    
    return scaledRects;
  }
}
