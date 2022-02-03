#include <string>
#define TXMP_STRING_TYPE	std::string
#define XMP_INCLUDE_XMPFILES 1

#define UNIX_ENV 1
#define XMP_UNIXBuild 1

#include <XMP.hpp>
#include <XMP.incl_cpp>

#include <iostream>
#include <fstream>

#include <vesuvianite/XMPWriter.hpp>

using namespace std;

XmpGenerator::CustomXmp::CustomXmp()
{
  if (!SXMPMeta::Initialize()) exit(1);
  if (!SXMPFiles::Initialize()) exit(1);
}

void XmpGenerator::CustomXmp::createXmpFromProcessedImage()
{

}

