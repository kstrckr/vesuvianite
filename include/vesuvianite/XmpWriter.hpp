#pragma once

#include <vesuvianite/SourceImage.hpp>

namespace XmpTool
{
  class XmpWriter
  {
    public:
      XmpWriter(std::string pathToRaw, Source::ProcessingTarget processedSource);
      int ProcessFile (const std::string pathToRaw);
      int WriteXmp();
    private:
      float cropLeft;
      float cropRight;
      float cropTop;
      float cropBottom;
      float cropAngle;
      std::string outPathString;
  };
}
