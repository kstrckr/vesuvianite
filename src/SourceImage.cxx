#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/SourceImage.hpp>

Source::ProcessingTarget::ProcessingTarget(std::string pathToRaw)
{
  sourcePath = pathToRaw;

  sourceImage = cv::imread(sourcePath);

  cv::resize(sourceImage, isolationThumbnail, cv::Size(100, 150), 0, 0, cv::INTER_CUBIC);
}
