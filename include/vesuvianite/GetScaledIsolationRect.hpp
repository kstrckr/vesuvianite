#pragma once

#include <opencv4/opencv2/opencv.hpp>
#include <vesuvianite/SubjectPlacement.hpp>

namespace GetScaledIsolationRect
{
  Placement getScaledRectAndBound(cv::Mat &image, cv::RotatedRect);
}