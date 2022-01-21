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
    bool thumbnailIsLikelyIsolated;
    bool subjectIsLikelyIsolated;
    cv::Rect finalBoundingBox;

    ProcessingTarget(std::string pathToRaw);

    void displaySourceImage();
    void displayThumbnailSource();
    cv::Mat drawRect();
    cv::Mat drawContours();

  private:
    cv::Scalar color;
    int thumbnailScaleFactor;
    int thumbnailWidth;
    int thumbnailHeight;
    int thumbnailGaussianKernelSize;
    int thumbnailThresholdValue;
    int thumbnailAreaDivisor;

    int fullsizeGaussianKernelSize;
    int fullsizeThresholdValue;

    void thumbnailIsolation();
    void fullsieIsolation();
  };
}

// #endif
