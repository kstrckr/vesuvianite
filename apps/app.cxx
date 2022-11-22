#include <filesystem>
#include <opencv2/opencv.hpp>
// #include <nfd.h>
// #include <stdio.h>

// #include <vesuvianite/HoughLines.hpp>
// #include <vesuvianite/RawLoader.hpp>
// #include <vesuvianite/ConvexHull.hpp>
// #include <vesuvianite/IsolateSubject.hpp>
// #include <vesuvianite/GetScaledIsolationRect.hpp>
#include <vesuvianite/SourceImage.hpp>
#include <vesuvianite/XmpWriter.hpp>

// Placement placementData;

int main(int argc, char **argv)
{
  cv::Mat image;
  cv::RotatedRect isolationSource;

  // std::string path = "/run/media/a/4tb/Downloads/example_t2t_raws/51A/51A/";
  // NFD_Init();

  // nfdchar_t *outPath;
  // nfdresult_t result = NFD_PickFolderN(&outPath, "/");
  std::string path;
  // char buffer[1000];

  std::string outPath = std::filesystem::u8path(argv[1]);

  printf("Processing raws in selected directory");
  // namedWindow(outPath, cv::WINDOW_NORMAL);

  for (const auto &entry : std::filesystem::directory_iterator(outPath))
  {
    std::string filePath = entry.path();
    int position = filePath.find_last_of(".");
    std::string extension = filePath.substr(position + 1);
    if (extension == "cr2")
    {
      Source::ProcessingTarget processingTarget = Source::ProcessingTarget(filePath);
      if (processingTarget.thumbnailIsLikelyIsolated)
      {
        // cv::Mat thumbnailWithRect = processingTarget.drawThumbnailWithRect();
        // cv::imwrite(filePath + ".jpg", thumbnailWithRect);
        // cv::Mat imageWithRect = processingTarget.drawFinalImageWithRect();
        // cv::imwrite(filePath + "_full.jpg", imageWithRect);
        // imshow(outPath, thumbnailWithRect);
        // cv::waitKey(0);
        if (processingTarget.subjectIsLikelyIsolated)
        {
          cv::Mat imageWithRect = processingTarget.drawFinalImageWithRect();
          imshow(outPath, imageWithRect);
          cv::waitKey(0);
          std::string path = std::string(outPath);
          // XmpTool::XmpWriter xmp = XmpTool::XmpWriter(filePath, processingTarget);
        }
      }
    }
  }

  return 0;
}