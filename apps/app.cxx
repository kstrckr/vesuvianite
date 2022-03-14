#include <filesystem>
#include <opencv2/opencv.hpp>
#include <nfd.h>
// #include <stdio.h>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>
#include <vesuvianite/ConvexHull.hpp>
#include <vesuvianite/IsolateSubject.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>
#include <vesuvianite/SourceImage.hpp>
#include <vesuvianite/XmpWriter.hpp>

Placement placementData;

int main()
{
  cv::Mat image;
  cv::RotatedRect isolationSource;

  // std::string path = "/run/media/a/4tb/Downloads/example_t2t_raws/51A/51A/";
  NFD_Init();

  nfdchar_t *outPath;
  nfdresult_t result = NFD_PickFolderN(&outPath, "/home/a/proj/vesuvianite/ideal-target-batch-1");
  std::string path;
  char buffer[1000];

  if (result == NFD_OKAY)
  {
    puts("Processing raws in selected directory");
    puts(outPath);

    for (const auto &entry : std::filesystem::directory_iterator(outPath))
    {
      std::string filePath = entry.path();

      Source::ProcessingTarget processingTarget = Source::ProcessingTarget(filePath);
      cv::Mat imageWithRect = processingTarget.drawFinalImageWithRect();
      namedWindow(outPath, cv::WINDOW_NORMAL);
      imshow(outPath, imageWithRect);
      std::string path = std::string(outPath);
      XmpTool::XmpWriter xmp = XmpTool::XmpWriter(filePath, processingTarget);
      cv::waitKey(0);
    }

    NFD_FreePath(outPath);
  }
  else if (result == NFD_CANCEL)
  {
    puts("User pressed cancel.");
  }
  else
  {
    printf("Error: %s\n", NFD_GetError());
  }

  NFD_Quit();

  // image = imageProc::loadRaw("/home/a/proj/vesuvianite/ideal-target-batch-1/CD.32.2197.cr2");

  // Source::ProcessingTarget processingTarget = Source::ProcessingTarget("/home/a/proj/vesuvianite/ideal-target-batch-1/CD.32.2197.cr2");
  // cv::Mat thumbnailWithRect = processingTarget.drawRect();
  // namedWindow("IsolateSubject", cv::WINDOW_NORMAL);
  // imshow("IsolateSubject", thumbnailWithRect);
  // cv::waitKey(0);

  // isolationSource = IsolateSubject::isolate(image);
  // placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
  // cvxHull::convexHull(placementData, image);

  return 0;
}