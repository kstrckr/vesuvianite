#ifndef SubjectPlacement
#define SubjectPlacement

#include <opencv4/opencv2/opencv.hpp>

struct Placement {
  cv::RotatedRect isolationRect;
  cv::Rect isolationBoundingRect;
  cv::RotatedRect finalCropData;
};

#endif
