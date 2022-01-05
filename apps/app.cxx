#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>

int main() {
  cv::Mat image;
  image = imageProc::loadRaw();
  hl::houghLines(image);
  // imageProc::findEdges();
  return 0;
}