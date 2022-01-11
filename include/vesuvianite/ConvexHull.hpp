#include <opencv4/opencv2/opencv.hpp>
#include <vesuvianite/SubjectPlacement.hpp>

namespace cvxHull {
  void convexHull(Placement pd, cv::Mat& image);
  void thresh_callback(int, void *);
}
