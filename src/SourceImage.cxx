#include <libraw.h>
#include <opencv2/opencv.hpp>

#include <vesuvianite/SourceImage.hpp>

using namespace std;

Source::ProcessingTarget::ProcessingTarget(std::string pathToRaw)
{
  // int i;
  int ret = 0;
  thumbnailIsLikelyIsolated = false;
  subjectIsLikelyIsolated = false;

  LibRaw RawProcessor;

  sourcePath = pathToRaw;

  // pull jpeg thumbnail from raw file
  const char * pathForLibraw = sourcePath.c_str();
  RawProcessor.open_file(pathForLibraw);
  RawProcessor.unpack_thumb();

  // convert in-memory thumbnail into opencv Mat
  uint bufsize = RawProcessor.imgdata.thumbnail.tlength;
  cv::Mat rawData(1, bufsize, CV_8UC1, RawProcessor.imgdata.thumbnail.thumb);
  imdecode(rawData, cv::IMREAD_COLOR, &sourceImage);
  if ( sourceImage.data == NULL )
  {
      puts("Error reading raw file");
  }

  cv::Size sourceSize = sourceImage.size();

  thumbnailScaleFactor = 25;
  thumbnailAreaDivisor = 4;

  thumbnailWidth = sourceSize.width / thumbnailScaleFactor;
  thumbnailHeight = sourceSize.height / thumbnailScaleFactor;
  cv::resize(sourceImage, isolationThumbnail, cv::Size(thumbnailWidth, thumbnailHeight), 0, 0, cv::INTER_CUBIC);

  // namedWindow("isolationThumbnail jpg from buffer", cv::WINDOW_NORMAL);
  // imshow("jpg from buffer", isolationThumbnail);
  // cv::waitKey(0);

  thumbnailGaussianKernelSize = 5;
  thumbnailThresholdValue = 18;

  fullsizeGaussianKernelSize = 15;
  fullsizeThresholdValue = 50;

  lineColor = cv::Scalar(0, 255, 0);

  thumbnailIsolation();
  if (thumbnailIsLikelyIsolated) {
    scaleIsollationRect();
    fullsizeIsolation();
  }
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
    // printf("angle %f\n", minRect[i].angle);
    if ((abs(minRect[i].angle) <= 20 || abs(minRect[i].angle) >= 80)
      && (minRect[i].size.width * minRect[i].size.height) >= ((thumbnailHeight * thumbnailWidth) / thumbnailAreaDivisor))
    {
      cv::Point2f rect_points[4];
      minRect[i].points(rect_points);
      thumbnailSubjectRectangle = minRect[i];
      thumbnailIsLikelyIsolated = true;
      break;
    }
  }
  printf("\nthumbnailIsLikelyIsolated %i\n", thumbnailIsLikelyIsolated);
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
  // cv::Scalar lineColor = cv::Scalar(0, 255, 0);
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
    cv::line(clonedThumbnail, rect_points[j], rect_points[(j + 1) % 4], lineColor);
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
    cv::line(clonedFullImage, rect_points[j], rect_points[(j + 1) % 4], lineColor, 8);
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
    cv::line(clonedFullImage, rect_points[j], rect_points[(j + 1) % 4], lineColor, 8);
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

  cv::Mat isolatedROI(sourceImage, fullIsolationBounding);
  cv::cvtColor(isolatedROI, src_gray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(src_gray, blurred_grey, cv::Size(fullsizeGaussianKernelSize, fullsizeGaussianKernelSize), 0);

  cv::Canny(blurred_grey, canny_output, fullsizeThresholdValue, fullsizeThresholdValue * 2);

  vector<vector<cv::Point>> contours;
  vector<cv::Vec4i> hierarchy;
  findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  vector<vector<cv::Point>> hull(hierarchy.size());
  vector<cv::RotatedRect> minRect(hull.size());

  cv::RotatedRect rotatedRectInROI;

  for (size_t i = 0; i < hierarchy.size(); i++)
  {
    convexHull(contours[i], hull[i]);
    minRect[i] = minAreaRect(hull[i]);
    if ((abs(minRect[i].angle) <= 20 || abs(minRect[i].angle) >= 80)
      && minRect[i].size.area() >= fullIsolationBounding.area() * 0.66)
    {
      cv::Point2f rect_points[4];
      minRect[i].points(rect_points);
      rotatedRectInROI = minRect[i];
      subjectIsLikelyIsolated = true;
      // for (int j = 0; j < 4; j++)
      // {
      //   line(isolatedROI, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(0, 255, 0), 25);
      // }

      break;
    }
  }

  cv::Point2f scaledCenter = cv::Point2f(
    rotatedRectInROI.center.x + fullIsolationBounding.x,
    rotatedRectInROI.center.y + fullIsolationBounding.y
  );

  finalSubjectRectrangle = cv::RotatedRect(scaledCenter, rotatedRectInROI.size, rotatedRectInROI.angle);
  cv::Size cropSize = finalSubjectRectrangle.size;
  cv::Size originalSize = sourceImage.size();
  cv::Point2f vtx[4];
  finalSubjectRectrangle.points(vtx); // bottomLeft, topLeft, topRight, bottomRight
  // topLeft and bottomRight are the only coordinates that matter for xmp
  cv::Point2f topLeft = vtx[1];
  cv::Point2f bottomRight = vtx[3];
  cropTop = topLeft.y / originalSize.height;
  cropLeft = topLeft.x / originalSize.width;

  cropBottom = bottomRight.y / originalSize.height;
  cropRight = bottomRight.x / originalSize.width;
  cropAngle = rotatedRectInROI.angle;
  printf("subjectIsLikelyIsolated   %i\n\n", subjectIsLikelyIsolated);

};
