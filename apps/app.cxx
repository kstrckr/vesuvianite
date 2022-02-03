#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>
#include <nfd.h>
// #include <stdio.h>

#include <vesuvianite/HoughLines.hpp>
#include <vesuvianite/RawLoader.hpp>
#include <vesuvianite/ConvexHull.hpp>
#include <vesuvianite/IsolateSubject.hpp>
#include <vesuvianite/GetScaledIsolationRect.hpp>
#include <vesuvianite/SourceImage.hpp>

Placement placementData;

int main()
{
  cv::Mat image;
  cv::RotatedRect isolationSource;

  // std::string path = "/run/media/a/4tb/Downloads/example_t2t_raws/51A/51A/";
  NFD_Init();

  nfdchar_t *outPath;
  nfdresult_t result = NFD_PickFolderN(&outPath, NULL);

  if (result == NFD_OKAY)
  {
    puts("Processing raws in selected directory");
    puts(outPath);

    for (const auto &entry : std::filesystem::directory_iterator(outPath))
    {
      std::string filePath = entry.path();
      // image = imageProc::loadRaw(filePath);
      // isolationSource = IsolateSubject::isolate(image);
      // placementData = GetScaledIsolationRect::getScaledRectAndBound(image, isolationSource);
      // cvxHull::convexHull(placementData, image);
      // Source::ProcessingTarget processingTarget = Source::ProcessingTarget(filePath);

      Source::ProcessingTarget processingTarget = Source::ProcessingTarget(filePath);
      cv::Mat imageWithRect = processingTarget.drawFinalImageWithRect();
      // namedWindow(outPath, cv::WINDOW_NORMAL);
      // imshow(outPath, imageWithRect);

      auto now = std::chrono::system_clock::now();
      auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
      auto epoch = now_ms.time_since_epoch();
      auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
      long duration = value.count();
      std::string timeString = std::to_string(duration);
      imwrite(outPath + timeString + ".jpg", imageWithRect);
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