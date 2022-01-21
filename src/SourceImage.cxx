#include <opencv4/opencv2/opencv.hpp>

#include <vesuvianite/SourceImage.hpp>

using namespace std;

Source::ProcessingTarget::ProcessingTarget(std::string pathToRaw)
{
  thumbnailIsLikelyIsolated = false;
  subjectIsLikelyIsolated = false;

  thumbnailScaleFactor = 50;
  sourcePath = pathToRaw;
  sourceImage = cv::imread(sourcePath);
  cv::Size sourceSize = sourceImage.size();
  thumbnailWidth = sourceSize.width / thumbnailScaleFactor;
  thumbnailHeight = sourceSize.height / thumbnailScaleFactor;
  cv::resize(sourceImage, isolationThumbnail, cv::Size(thumbnailWidth, thumbnailHeight), 0, 0, cv::INTER_CUBIC);

  thumbnailGaussianKernelSize = 9;
  thumbnailThresholdValue = 18;

  fullsizeGaussianKernelSize = 15;
  fullsizeThresholdValue = 50;

  thumbnailAreaDivisor = 4;

  color = cv::Scalar(0, 255, 0);

  thumbnailIsolation();
}

void Source::ProcessingTarget::thumbnailIsolation()
{
  cv::Mat grey, blurred_grey, canny_output;
  cv::cvtColor(isolationThumbnail, grey, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(grey, blurred_grey, cv::Size(thumbnailGaussianKernelSize, thumbnailGaussianKernelSize), 0);
  cv::Canny(blurred_grey, canny_output, thumbnailThresholdValue, thumbnailThresholdValue * 2, 3, true);
  vector<vector<cv::Point>> contours;
  vector<cv::Vec4i> hierarchy;
  findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  vector<cv::RotatedRect> minRect(contours.size());
  for (size_t i = 0; i < contours.size(); i++)
  {
    minRect[i] = minAreaRect(contours[i]);
  }
  for (size_t i = 0; i < contours.size(); i++)
  {
    if ((minRect[i].size.width * minRect[i].size.height) >= ((thumbnailHeight * thumbnailWidth) / thumbnailAreaDivisor))
    {
      cv::Point2f rect_points[4];
      minRect[i].points(rect_points);
      thumbnailSubjectRectangle = minRect[i];
      thumbnailIsLikelyIsolated = true;
      break;
    }
  }
};

cv::Mat Source::ProcessingTarget::drawRect()
{
  cv::Mat clonedThumbnail = isolationThumbnail.clone();
  cv::Point2f rect_points[4];
  thumbnailSubjectRectangle.points(rect_points);
  for (int j = 0; j < 4; j++)
  {
    cv::line(clonedThumbnail, rect_points[j], rect_points[(j + 1) % 4], color);
  }
  return clonedThumbnail;
}

void Source::ProcessingTarget::fullsieIsolation(){

};
