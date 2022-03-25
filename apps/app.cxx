#include <filesystem>
#include <opencv2/opencv.hpp>
#include <nfd.h>
// #include <stdio.h>

// #include <vesuvianite/HoughLines.hpp>
// #include <vesuvianite/RawLoader.hpp>
// #include <vesuvianite/ConvexHull.hpp>
// #include <vesuvianite/IsolateSubject.hpp>
// #include <vesuvianite/GetScaledIsolationRect.hpp>
#include <vesuvianite/SourceImage.hpp>
#include <vesuvianite/XmpWriter.hpp>

// Placement placementData;

int main()
{
  cv::Mat image;
  cv::RotatedRect isolationSource;

  // std::string path = "/run/media/a/4tb/Downloads/example_t2t_raws/51A/51A/";
  NFD_Init();

  nfdchar_t *outPath;
  nfdresult_t result = NFD_PickFolderN(&outPath, "/Users/ks/proj/t2t_samples/big_batch_o_test_files");
  std::string path;
  char buffer[1000];

  if (result == NFD_OKAY)
  {
    puts("Processing raws in selected directory");
    puts(outPath);
    namedWindow(outPath, cv::WINDOW_NORMAL);

    for (const auto &entry : std::filesystem::directory_iterator(outPath))
    {
      std::string filePath = entry.path();

      Source::ProcessingTarget processingTarget = Source::ProcessingTarget(filePath);
      if (processingTarget.thumbnailIsLikelyIsolated) {
        cv::Mat thumbnailWithRect = processingTarget.drawThumbnailWithRect();
        // imshow(outPath, thumbnailWithRect);
        // cv::waitKey(0);
        if (processingTarget.subjectIsLikelyIsolated) {
          cv::Mat imageWithRect = processingTarget.drawFinalImageWithRect();
          imshow(outPath, imageWithRect);
          cv::waitKey(0);
          std::string path = std::string(outPath);
          XmpTool::XmpWriter xmp = XmpTool::XmpWriter(filePath, processingTarget);
        }
      }
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

  return 0;
}