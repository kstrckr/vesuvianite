#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace cv;

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
  CV_Assert(I.depth() == CV_8U);

  int channels = I.channels();

  int nRows = I.rows;
  int nCols = I.cols * channels;

  if (I.isContinuous())
  {
    nCols *= nRows;
    nRows = 1;
  }

  int i,j;
  uchar* p;
  for (i = 0; i < nRows; ++i)
  {
    p = I.ptr<uchar>(i);
    for (j = 0; j < nCols; ++j)
    {
      p[j] = table[p[j]];
    }
  }
  return I;
}

int main (int argc, char** argv)
{
  int divideWidth = 0;
  std::stringstream s;
  s << argv[2];
  s >> divideWidth;

  if (!s || !divideWidth)
  {
    std::cout << "Invalid number entered for dividing. " << std::endl;
  }

  uchar table[256];
  for (int i = 0; i < 256; ++i)
  {
    table[i] = (uchar)(divideWidth * (i/divideWidth));
  }

  double t = (double)getTickCount();
  std::string src = argv[1];
  Mat img = imread(src, IMREAD_GRAYSCALE);

  ScanImageAndReduceC(img, table);
  t = ((double)getTickCount() - t)/getTickFrequency();

  namedWindow("image", WINDOW_AUTOSIZE);
  imshow("image", img);
  waitKey(0);


  std::cout << "Times passed in seconds: " << t << std::endl;
}
