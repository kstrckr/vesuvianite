#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;

int main (int argc, char** argv)
{
  std::string image_path = "/home/a/proj/vesuvianite/ideal-target-batch-1/51A_8_verso.cr2";
  Mat img = imread(image_path, IMREAD_GRAYSCALE);

  if(img.empty())
  {
    std::cout << "Could not read the image: " << image_path << std::endl;
    return 1;
  }

  imshow("Display window", img);
  int k = waitKey(0);
}
