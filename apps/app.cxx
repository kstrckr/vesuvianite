#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>
#include <vesuvianite/ConvexHull.hpp>
#include <vesuvianite/IsolateSubject.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>

Placement placementData;

int main()
{
  cv::Mat image;
  cv::RotatedRect isolationSource;

  std::string path = "/run/media/a/4tb/Downloads/example_t2t_raws/51A/51A/";
  for (const auto &entry : std::filesystem::directory_iterator(path))
  {
    std::string filePath = entry.path();
    image = imageProc::loadRaw(filePath);
    isolationSource = IsolateSubject::isolate(image);
    placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
    cvxHull::convexHull(placementData, image);
  }

  // image = imageProc::loadRaw("/home/a/proj/vesuvianite/ideal-target-batch-1/CD.32.2197.cr2");
  // isolationSource = IsolateSubject::isolate(image);
  // placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
  // cvxHull::convexHull(placementData, image);
  
  return 0;
}