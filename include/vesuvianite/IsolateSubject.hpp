#pragma once

#include <opencv4/opencv2/opencv.hpp>

namespace IsolateSubject
{
  cv::RotatedRect isolate(cv::Mat image);
}
