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
  std::string path = "/run/media/a/4tb/Downloads/example_t2t_raws/drive-download-20211206T062405Z-001/samples_uncropped_RAW/originlabs_alignment_app_testin2_20190723";
  for (const auto &entry : std::filesystem::directory_iterator(path))
  {
    std::string filePath = entry.path();
    image = imageProc::loadRaw(filePath);
    isolationSource = IsolateSubject::isolate(image);
    placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
    cvxHull::convexHull(placementData, image);

  // image = imageProc::loadRaw("/home/a/proj/vesuvianite/ideal-target-batch-1/51A_46_recto.cr2");
  // isolationSource = IsolateSubject::isolate(image);
  // placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
  // cvxHull::convexHull(placementData, image);
  
  return 0;
}