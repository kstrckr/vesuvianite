#include <opencv4/opencv2/opencv.hpp>

// #ifndef Source
// #define Source

namespace Source
{
  class ProcessingTarget
  {
  public:
    std::string sourcePath;
    cv::Mat sourceImage;
    cv::Mat isolationThumbnail;

    cv::RotatedRect thumbnailSubjectRectangle;
    cv::Rect finalBoundingBox;

    ProcessingTarget(std::string pathToRaw);

  // private:
  //   int thumbnailGaussianKernelSize;
  //   int thumbnailThresholdValue;

  //   int fullsizeGaussianKernelSize;
  //   int fullsizeThresholdValue;

  //   void thumbnailIsolation();
  //   void fullsieIsolation();
  };
}

// #endif
