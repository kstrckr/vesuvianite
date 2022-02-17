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

static XMP_Status DumpCallback(void *refCon, XMP_StringPtr outStr, XMP_StringLen outLen)
{
  XMP_Status status = 0;
  size_t count;
  FILE *outFile = static_cast<FILE *>(refCon);

  count = fwrite(outStr, 1, outLen, outFile);
  if (count != outLen)
    status = errno;
  return status;
}

void XmpTool::XmpWriter::WriteMinorLabel(FILE *log, const char *title)
{

  fprintf(log, "\n// ");
  for (size_t i = 0; i < strlen(title); ++i)
    fprintf(log, "-");
  fprintf(log, "--\n// %s :\n\n", title);
  fflush(log);
}

int XmpTool::XmpWriter::ProcessFile (const char * pathToRaw)
{
  bool ok;
  char buffer[1000];

  SXMPMeta xmpMeta;
  SXMPFiles xmpFile;
  XMP_FileFormat format;
  XMP_OptionBits openFlags, handlerFlags;
  XMP_PacketInfo xmpPacket;

  sprintf(buffer, "Dumping main XMP for %s", pathToRaw);
  XmpTool::XmpWriter::WriteMinorLabel(sLogFile, buffer);

  xmpFile.OpenFile(pathToRaw, kXMP_UnknownFile, kXMPFiles_OpenForRead);
  ok = xmpFile.GetFileInfo(0, &openFlags, &format, &handlerFlags);
  if (!ok)
    return 1;

  fprintf(sLogFile, "File info : format = %.8X, handler flags = %.8X\n", format, handlerFlags);
  fflush(sLogFile);

  ok = xmpFile.GetXMP(&xmpMeta, 0, &xmpPacket);
  if (!ok)
    return 1;

  XMP_Int32 offset = (XMP_Int32)xmpPacket.offset;
  XMP_Int32 length = xmpPacket.length;
  fprintf(sLogFile, "Packet info : offset = %d, length = %d\n", offset, length);
  fflush(sLogFile);

  fprintf(sLogFile, "\nInitial XMP from %s\n", pathToRaw);
  xmpMeta.DumpObject(DumpCallback, sLogFile);

  xmpFile.CloseFile();
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
