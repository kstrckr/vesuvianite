#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>
#include <vesuvianite/ConvexHull.hpp>
#include <vesuvianite/IsolateSubject.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>
// #include <vesuvianite/SubjectPlacement.hpp.hpp>

Placement placementData;

int main() {
  cv::Mat image;
  cv::RotatedRect isolationSource;
  image = imageProc::loadRaw("/home/a/proj/vesuvianite/ideal-target-batch-1/CD.32.2197.cr2");
  isolationSource = IsolateSubject::isolate(image);
  placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
  
  return 0;
}