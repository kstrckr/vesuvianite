#include <string>
#define TXMP_STRING_TYPE std::string

#define XMP_INCLUDE_XMPFILES 1
#define UNIX_ENV 1

#include "XMP.incl_cpp"
#include "XMP.hpp"

#include <iostream>
#include <fstream>

using namespace std;

#include <vesuvianite/XmpWriter.hpp>

XmpTool::XmpWriter::XmpWriter()
{
  if (!SXMPMeta::Initialize())
    exit(1);
  if (!SXMPFiles::Initialize())
    exit(1);
  XMP_OptionBits opts = kXMPFiles_OpenForRead | kXMPFiles_OpenUseSmartHandler;

  std::string status = "";
  SXMPFiles myFile;

  string filename = "/home/a/proj/vesuvianite/ideal-target-batch-1/51A_8_verso.cr2";

  bool ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);

  if (!ok)
  {
    status += "No smart handler available for " + filename + "\n";
    status += "Trying packet scanning.\n";
    // Now try using packet scanning
    opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
    ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
  }

  if (ok)
  {
    SXMPMeta meta;
    myFile.GetXMP(&meta);

    bool exists;
    string simpleValue;
    exists = meta.GetProperty(kXMP_NS_XMP, "CreatorTool", &simpleValue, NULL);
    if (exists)
      cout << "CreatorTool = " << simpleValue << endl;
    else
      simpleValue.clear();

    string elementValue;
    exists = meta.GetArrayItem(kXMP_NS_DC, "creator", 1, &elementValue, NULL);
    if (exists)
      cout << "dc:creator = " << elementValue << endl;
    else
      elementValue.clear();

    string propValue;
    int arrSize = meta.CountArrayItems(kXMP_NS_DC, "subject");
    for (int i = 1; i <= arrSize; i++)
    {
      meta.GetArrayItem(kXMP_NS_DC, "subject", i, &propValue, NULL);
      cout << "dc:subject[" << i << "] = " << propValue << endl;
    }

    string itemValue;
    meta.GetLocalizedText(kXMP_NS_DC, "title", "en", "en-US", NULL,
                          &itemValue, NULL);
    cout << "dc:title in English = " << itemValue << endl;

    meta.GetLocalizedText(kXMP_NS_DC, "title", "fr", "fr-FR", NULL,
                          &itemValue, NULL);
    cout << "dc:title in French = " << itemValue << endl;

    XMP_DateTime myDate;
    if (meta.GetProperty_Date(kXMP_NS_XMP, "MetadataDate", &myDate, NULL))
    {
      string myDateStr;
      SXMPUtils::ConvertFromDate(myDate, &myDateStr);
      cout << "meta:MetadataDate = " << myDateStr << endl;
    }

    bool exist;
    string path, value;
    exist = meta.DoesStructFieldExist(kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF, "Fired");
    if (exist)
    {
      bool flashFired;
      SXMPUtils::ComposeStructFieldPath(kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF,
                                        "Fired", &path);
      meta.GetProperty_Bool(kXMP_NS_EXIF, path.c_str(), &flashFired, NULL);
      string flash = (flashFired) ? "True" : "False";
      cout << "Flash Used = " << flash << endl;
    }

    myFile.CloseFile();
  }

  SXMPFiles::Terminate();
  SXMPMeta::Terminate();
}