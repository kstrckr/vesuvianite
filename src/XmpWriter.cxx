#include <string>

#define TXMP_STRING_TYPE std::string
#define XMP_INCLUDE_XMPFILES 1
#define UNIX_ENV 1

#include <XMP.hpp>      //NB: no XMP.incl_cpp here on purpose, gets compiled in main...
#include <XMP.incl_cpp> //include in EXACTLY one source file (i.e. main, in Action gets you trouble...
#include <XMP_Const.h>

#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <vesuvianite/XmpWriter.hpp>

using namespace std;

static FILE *sLogFile = stdout;

int XmpTool::XmpWriter::ProcessFile (const char * pathToRaw)
{
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
  const char * path = pathToRaw.c_str();

  XmpTool::XmpWriter::ProcessFile(path);
}
