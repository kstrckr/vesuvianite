#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>
#include <vesuvianite/ConvexHull.hpp>

int main() {
  cv::Mat image;
  image = imageProc::loadRaw("/home/a/proj/vesuvianite/ideal-target-batch-1/51A_8_verso.cr2");
  cvxHull::convexHull(image);
  // hl::houghLines(image);
  // imageProc::findEdges();
  return 0;
}