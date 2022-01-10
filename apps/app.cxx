#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>
#include <vesuvianite/ConvexHull.hpp>
#include <vesuvianite/IsolateSubject.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>

int main() {
  cv::Mat image;
  cv::RotatedRect isolationSource;
  image = imageProc::loadRaw("/home/a/proj/vesuvianite/ideal-target-batch-1/51A_82_recto.cr2");
  isolationSource = IsolateSubject::isolate(image);
  GetScaledIsolationRect::saveWithPreviewRectangle(image, isolationSource);
  // cvxHull::convexHull(isolationSource);
  // hl::houghLines(isolationSource);
  // imageProc::findEdges();
  return 0;
}