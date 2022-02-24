#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#define TXMP_STRING_TYPE std::string
#define XMP_INCLUDE_XMPFILES 1
#define UNIX_ENV 1

#include <XMP.incl_cpp> //include in EXACTLY one source file (i.e. main, in Action gets you trouble...
#include <XMP.hpp>      //NB: no XMP.incl_cpp here on purpose, gets compiled in main...
#include <XMP_Const.h>

#include <vesuvianite/XmpWriter.hpp>

using namespace std;

static FILE *sLogFile = stdout;

int XmpTool::XmpWriter::WriteXmp()
{
  return 0;
}

int XmpTool::XmpWriter::ProcessFile(std::string pathToRaw)
{
  int generalCloseFlags = 0;
  XMP_FileFormat xmpFileFormat = kXMP_UnknownFile;

  SXMPFiles xmpFile;
  std::string xmpDump;
  xmpFile.OpenFile(pathToRaw,xmpFileFormat,kXMPFiles_OpenForRead);
  printf("file opened");
  SXMPMeta xmpMeta;
  if (!xmpFile.GetXMP(&xmpMeta, 0, 0))
    printf("file contains no XMP");
  // Log::warn("file contains no XMP. - says xmpFile.GetXMP()");
  else
  {
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropTop", std::to_string(cropTop));
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropLeft", std::to_string(cropLeft));
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropBottom", std::to_string(cropBottom));
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropRight", std::to_string(cropRight));
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropAngle", std::to_string(cropAngle));
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropConstrainToWarp", "0");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "HasCrop", "True");

    xmpMeta.SerializeToBuffer(&xmpDump, 0, 0);
    // printf("%s",xmpDump.c_str());
  }


  xmpFile.CloseFile(generalCloseFlags); //(enabled again now that bug 1352603 is fixed)
  return 0;
}

XmpTool::XmpWriter::XmpWriter(std::string pathToRaw, Source::ProcessingTarget processedSource)
{
  if (!SXMPMeta::Initialize())
  {
    printf("## SXMPMeta::Initialize failed!\n");
    exit(1);
  }

  XMP_OptionBits options = 0;
#if UNIX_ENV
  options |= kXMPFiles_ServerMode;
#endif

  if (!SXMPFiles::Initialize(options))
  {

    printf("## SXMPFiles::Initialize failed!\n");
    exit(1);
  }
  const char *path = pathToRaw.c_str();

  cropLeft = processedSource.cropLeft;
  cropRight = processedSource.cropRight;
  cropTop = processedSource.cropTop;
  cropBottom = processedSource.cropBottom;
  cropAngle = processedSource.cropAngle;

  int len = strlen(path);
  outPathString = pathToRaw.substr(0, len - 4);

  XmpTool::XmpWriter::ProcessFile(path);

  SXMPFiles::Terminate();
  SXMPMeta::Terminate();
}
