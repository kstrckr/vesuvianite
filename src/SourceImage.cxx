#include <libraw.h>
#include <opencv2/opencv.hpp>

#include <vesuvianite/SourceImage.hpp>

using namespace std;

Source::ProcessingTarget::ProcessingTarget(std::string pathToRaw)
{
  int ret = 0;
  int i;
  LibRaw RawProcessor;

  thumbnailIsLikelyIsolated = false;
  subjectIsLikelyIsolated = false;

  thumbnailScaleFactor = 50;
  sourcePath = pathToRaw;
  const char * pathForLibraw = sourcePath.c_str();
  RawProcessor.open_file(pathForLibraw);
  RawProcessor.unpack_thumb();

  uint bufsize = RawProcessor.imgdata.thumbnail.tlength;
  cv::Mat rawData(1, bufsize, CV_8UC1, RawProcessor.imgdata.thumbnail.thumb);
  cv::Mat sourceImage = imdecode( rawData, cv::IMREAD_COLOR);
  if ( sourceImage.data == NULL )   
  {
      puts("Error reading raw file");
  }



  cv::Size sourceSize = sourceImage.size();
  thumbnailWidth = sourceSize.width / thumbnailScaleFactor;
  thumbnailHeight = sourceSize.height / thumbnailScaleFactor;
  cv::resize(sourceImage, isolationThumbnail, cv::Size(thumbnailWidth, thumbnailHeight), 0, 0, cv::INTER_CUBIC);

  // namedWindow("jpg from buffer", cv::WINDOW_NORMAL);
  // imshow("jpg from buffer", isolationThumbnail);
  // cv::waitKey(0);

  thumbnailGaussianKernelSize = 9;
  thumbnailThresholdValue = 18;

  fullsizeGaussianKernelSize = 15;
  fullsizeThresholdValue = 50;

  thumbnailAreaDivisor = 4;

  color = cv::Scalar(0, 255, 0);

  thumbnailIsolation();
  scaleIsollationRect();
  fullsizeIsolation();
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

void Source::ProcessingTarget::scaleIsollationRect()
{
  cv::Size baseSize;
  cv::RotatedRect scaledRotatedRect;

  baseSize = sourceImage.size();
  cv::Point2f scaledCenter = cv::Point2f(
      thumbnailSubjectRectangle.center.x / thumbnailWidth * baseSize.width,
      thumbnailSubjectRectangle.center.y / thumbnailHeight * baseSize.height);
  cv::Size2f scaledSize = cv::Size2f(
      (thumbnailSubjectRectangle.size.width / thumbnailWidth * baseSize.width) + 0.005 * baseSize.width,
      (thumbnailSubjectRectangle.size.height / thumbnailHeight * baseSize.height) + 0.005 * baseSize.width);
  scaledRotatedRect = cv::RotatedRect(scaledCenter, scaledSize, thumbnailSubjectRectangle.angle);
  cv::Scalar color = cv::Scalar(0, 255, 0);
  cv::Point2f scaled_points[4];
  // scaledRotatedRect.points(scaled_points);
  cv::Rect boundingRect = scaledRotatedRect.boundingRect();
  fullIsolationRotatedRect = scaledRotatedRect;
  fullIsolationBounding = boundingRect;
}

cv::Mat Source::ProcessingTarget::drawThumbnailWithRect()
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

cv::Mat Source::ProcessingTarget::drawFullImageWithRect()
{
  cv::Mat clonedFullImage = sourceImage.clone();
  cv::Point2f rect_points[4];
  fullIsolationRotatedRect.points(rect_points);
  for (int j = 0; j < 4; j++)
  {
    cv::line(clonedFullImage, rect_points[j], rect_points[(j + 1) % 4], color, 8);
  }
  return clonedFullImage;
}

cv::Mat Source::ProcessingTarget::drawFinalImageWithRect()
{
  cv::Mat clonedFullImage = sourceImage.clone();
  cv::Point2f rect_points[4];
  finalSubjectRectrangle.points(rect_points);
  for (int j = 0; j < 4; j++)
  {
    cv::line(clonedFullImage, rect_points[j], rect_points[(j + 1) % 4], color, 8);
  }
  return clonedFullImage;
}

void Source::ProcessingTarget::fullsizeIsolation()
{
  cv::Mat grey, src_gray, blurred, blurred_grey, canny_output, convex_hull, dst;

  cv::Size rawSize = sourceImage.size();
  if (fullIsolationBounding.x < 0)
  {
    fullIsolationBounding.x = 0;
  }
  if (fullIsolationBounding.y < 0)
  {
    fullIsolationBounding.y = 0;
  }
  if (fullIsolationBounding.x + fullIsolationBounding.width > rawSize.width)
  {
    fullIsolationBounding.width = rawSize.width - fullIsolationBounding.x;
  }
  if (fullIsolationBounding.y + fullIsolationBounding.height > rawSize.height)
  {
    fullIsolationBounding.height = rawSize.height - fullIsolationBounding.y;
  }

  printf("%i x %i", fullIsolationBounding.width, fullIsolationBounding.height);
  cv::Mat isolatedROI(sourceImage, fullIsolationBounding);
  // cv::cvtColor(isolatedROI, src_gray, cv::COLOR_BGR2GRAY);
  // cv::GaussianBlur(src_gray, blurred_grey, cv::Size(fullsizeGaussianKernelSize, fullsizeGaussianKernelSize), 0);

  // cv::Canny(blurred_grey, canny_output, fullsizeThresholdValue, fullsizeThresholdValue * 2);

  // vector<vector<cv::Point>> contours;
  // vector<cv::Vec4i> hierarchy;
  // findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  // vector<vector<cv::Point>> hull(hierarchy.size());
  // vector<cv::RotatedRect> minRect(hull.size());

  // cv::RotatedRect rotatedRectInROI;

  // for (size_t i = 0; i < hierarchy.size(); i++)
  // {
  //   convexHull(contours[i], hull[i]);
  //   minRect[i] = minAreaRect(hull[i]);
  //   if (minRect[i].size.area() >= fullIsolationBounding.area() * 0.66)
  //   {
  //     cv::Point2f rect_points[4];
  //     minRect[i].points(rect_points);
  //     rotatedRectInROI = minRect[i];
  //     // for (int j = 0; j < 4; j++)
  //     // {
  //     //   line(isolatedROI, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(0, 255, 0), 25);
  //     // }

  //     break;
  //   }
  // }

  //   cv::Point2f scaledCenter = cv::Point2f(
  //     rotatedRectInROI.center.x + fullIsolationBounding.x,
  //     rotatedRectInROI.center.y + fullIsolationBounding.y
  //   );

  // finalSubjectRectrangle = cv::RotatedRect(scaledCenter, rotatedRectInROI.size, rotatedRectInROI.angle);
  // cv::Size cropSize = finalSubjectRectrangle.size;
  // cv::Size originalSize = sourceImage.size();
  // cropLeft = (finalSubjectRectrangle.center.x - (0.5 *  cropSize.width)) / originalSize.width;
  // cropTop = (finalSubjectRectrangle.center.y - (0.5 *  cropSize.height)) / originalSize.height;
  // cropRight = (finalSubjectRectrangle.center.x + (0.5 *  cropSize.width)) / originalSize.width;
  // cropBottom = (finalSubjectRectrangle.center.y + (0.5 *  cropSize.height)) / originalSize.height;
  // cropAngle = rotatedRectInROI.angle;
};
