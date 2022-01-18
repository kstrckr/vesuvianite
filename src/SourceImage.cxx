#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/SourceImage.hpp>

Source::ProcessingTarget::ProcessingTarget(std::string pathToRaw)
{
  sourcePath = pathToRaw;
  sourceImage = cv::imread(sourcePath);
  cv::resize(sourceImage, isolationThumbnail, cv::Size(100, 150), 0, 0, cv::INTER_CUBIC);

  thumbnailGaussianKernelSize = 9;
  thumbnailThresholdValue = 18;

  fullsizeGaussianKernelSize = 15;
  fullsizeThresholdValue = 50;
}

  void Source::ProcessingTarget::thumbnailIsolation()
  {
    cv::Mat grey, blurred_grey, canny_output;
    cv::cvtColor(isolationThumbnail, grey, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(grey, blurred_grey, cv::Size(thumbnailGaussianKernelSize, thumbnailGaussianKernelSize), 0);
    cv::Canny(blurred_grey, canny_output, thumbnailThresholdValue, thumbnailThresholdValue * 2, 3, true);
  };

  void Source::ProcessingTarget::fullsieIsolation()
  {

  };
