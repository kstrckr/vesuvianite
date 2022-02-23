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

int XmpTool::XmpWriter::ProcessFile(std::string pathToRaw)
{
  int generalCloseFlags = 0;
  XMP_FileFormat xmpFileFormat = kXMP_UnknownFile;

  SXMPFiles xmpFile;
  std::string xmpDump; // really just the raw string here
  xmpFile.OpenFile(pathToRaw,xmpFileFormat,kXMPFiles_OpenForRead);
  printf("file opened");
  SXMPMeta xmpMeta; // get meta-object first, then get serialization
  if (!xmpFile.GetXMP(&xmpMeta, 0, 0))
    printf("file contains no XMP");
  // Log::warn("file contains no XMP. - says xmpFile.GetXMP()");
  else
  {
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropTop", "0.24");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropLeft", "0.25");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropBottom", "0.75");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropRight", "0.75");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropAngle", "5");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropConstrainToWarp", "0");
    xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "HasCrop", "True");
  // xmlns:crs="http://ns.adobe.com/camera-raw-settings/1.0/"
  //   crs:CropTop="0.050845"
  //  crs:CropLeft="0.24177"
  //  crs:CropBottom="0.861565"
  //  crs:CropRight="0.821008"
  //  crs:CropAngle="4.346749"
  //  crs:CropConstrainToWarp="0"
  //  crs:HasCrop="True"
    xmpMeta.SerializeToBuffer(&xmpDump,
                              0,  // NOT using kXMP_UseCompactFormat - Use a highly compact RDF syntax and layout.
                              0); // receiving string, options, 0(default) padding
    printf("%s",xmpDump.c_str());
  }


  xmpFile.CloseFile(generalCloseFlags); //(enabled again now that bug 1352603 is fixed)
  return 0;
}

XmpTool::XmpWriter::XmpWriter(std::string pathToRaw)
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

  XmpTool::XmpWriter::ProcessFile(path);

  SXMPFiles::Terminate();
  SXMPMeta::Terminate();
}
