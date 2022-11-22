#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

#define TXMP_STRING_TYPE std::string
#define XMP_INCLUDE_XMPFILES 1
#define XMP_MacBuild 1
#define MAC_ENV 1
// #define UNIX_ENV 1

#include <XMP.incl_cpp>
#include <XMP.hpp>
#include <XMP_Const.h>

#include <vesuvianite/XmpWriter.hpp>

using namespace std;

static FILE *sLogFile = stdout;

int XmpTool::XmpWriter::WriteXmp(std::string xmpRawText)
{
  ofstream o;
  std::string outFilePath = outPathString + ".xmp";
  cout << outFilePath << endl;
  o.open(outFilePath, ios::out);
  o << xmpRawText;
  o.close();
  return 0;
}

int XmpTool::XmpWriter::ProcessFile(std::string pathToRaw)
{
  const char * rdf =
     "<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#'>"
  "<rdf:Description rdf:about='' xmlns:crs='http://ns.adobe.com/camera-raw-settings/1.0/' crs:Version='14.5' crs:ProcessVersion='11.0' crs:WhiteBalance='As Shot' crs:Exposure2012='0.00' crs:Contrast2012='0' crs:Highlights2012='0' crs:Shadows2012='0' crs:Whites2012='0' crs:Blacks2012='0' crs:Texture='0' crs:Clarity2012='0' crs:Dehaze='0' crs:Vibrance='0' crs:Saturation='0' crs:ParametricShadows='0' crs:ParametricDarks='0' crs:ParametricLights='0' crs:ParametricHighlights='0' crs:ParametricShadowSplit='25' crs:ParametricMidtoneSplit='50' crs:ParametricHighlightSplit='75' crs:Sharpness='0' crs:LuminanceSmoothing='0' crs:ColorNoiseReduction='0' crs:HueAdjustmentRed='0' crs:HueAdjustmentOrange='0' crs:HueAdjustmentYellow='0' crs:HueAdjustmentGreen='0' crs:HueAdjustmentAqua='0' crs:HueAdjustmentBlue='0' crs:HueAdjustmentPurple='0' crs:HueAdjustmentMagenta='0' crs:SaturationAdjustmentRed='0' crs:SaturationAdjustmentOrange='0' crs:SaturationAdjustmentYellow='0' crs:SaturationAdjustmentGreen='0' crs:SaturationAdjustmentAqua='0' crs:SaturationAdjustmentBlue='0' crs:SaturationAdjustmentPurple='0' crs:SaturationAdjustmentMagenta='0' crs:LuminanceAdjustmentRed='0' crs:LuminanceAdjustmentOrange='0' crs:LuminanceAdjustmentYellow='0' crs:LuminanceAdjustmentGreen='0' crs:LuminanceAdjustmentAqua='0' crs:LuminanceAdjustmentBlue='0' crs:LuminanceAdjustmentPurple='0' crs:LuminanceAdjustmentMagenta='0' crs:SplitToningShadowHue='0' crs:SplitToningShadowSaturation='0' crs:SplitToningHighlightHue='0' crs:SplitToningHighlightSaturation='0' crs:SplitToningBalance='0' crs:ColorGradeMidtoneHue='0' crs:ColorGradeMidtoneSat='0' crs:ColorGradeShadowLum='0' crs:ColorGradeMidtoneLum='0' crs:ColorGradeHighlightLum='0' crs:ColorGradeBlending='50' crs:ColorGradeGlobalHue='0' crs:ColorGradeGlobalSat='0' crs:ColorGradeGlobalLum='0' crs:AutoLateralCA='1' crs:LensProfileEnable='0' crs:LensManualDistortionAmount='0' crs:VignetteAmount='0' crs:DefringePurpleAmount='0' crs:DefringePurpleHueLo='30' crs:DefringePurpleHueHi='70' crs:DefringeGreenAmount='0' crs:DefringeGreenHueLo='40' crs:DefringeGreenHueHi='60' crs:PerspectiveUpright='0' crs:PerspectiveVertical='0' crs:PerspectiveHorizontal='0' crs:PerspectiveRotate='0.0' crs:PerspectiveAspect='0' crs:PerspectiveScale='100' crs:PerspectiveX='0.00' crs:PerspectiveY='0.00' crs:GrainAmount='0' crs:PostCropVignetteAmount='0' crs:ShadowTint='0' crs:RedHue='0' crs:RedSaturation='0' crs:GreenHue='0' crs:GreenSaturation='0' crs:BlueHue='0' crs:BlueSaturation='0' crs:ConvertToGrayscale='False' crs:OverrideLookVignette='False' crs:ToneCurveName2012='Custom' crs:CameraProfile='T2T_CanonEOS_5DSR_20220930' crs:CameraProfileDigest='D654C7266F2508362AE30062F58FBCEB' crs:HasSettings='True' crs:HasCrop='False'>"
   "<crs:ToneCurvePV2012>"
    "<rdf:Seq>"
     "<rdf:li>0, 0</rdf:li>"
     "<rdf:li>26, 52</rdf:li>"
     "<rdf:li>64, 85</rdf:li>"
     "<rdf:li>106, 120</rdf:li>"
     "<rdf:li>155, 163</rdf:li>"
     "<rdf:li>190, 211</rdf:li>"
     "<rdf:li>216, 241</rdf:li>"
     "<rdf:li>255, 255</rdf:li>"
    "</rdf:Seq>"
   "</crs:ToneCurvePV2012>"
   "<crs:ToneCurvePV2012Red>"
    "<rdf:Seq>"
     "<rdf:li>0, 0</rdf:li>"
     "<rdf:li>255, 255</rdf:li>"
    "</rdf:Seq>"
   "</crs:ToneCurvePV2012Red>"
   "<crs:ToneCurvePV2012Green>"
    "<rdf:Seq>"
     "<rdf:li>0, 0</rdf:li>"
     "<rdf:li>255, 255</rdf:li>"
    "</rdf:Seq>"
   "</crs:ToneCurvePV2012Green>"
   "<crs:ToneCurvePV2012Blue>"
    "<rdf:Seq>"
     "<rdf:li>0, 0</rdf:li>"
     "<rdf:li>255, 255</rdf:li>"
    "</rdf:Seq>"
   "</crs:ToneCurvePV2012Blue>"
  "</rdf:Description>"
 "</rdf:RDF>";

  SXMPMeta top_profile_meta;;
  // Loop over the rdf string and create the XMP object
  // 10 characters at a time
  int i;
  for (i = 0; i < (long)strlen(rdf) - 10; i += 10 )
  {
    top_profile_meta.ParseFromBuffer ( &rdf[i], 10, kXMP_ParseMoreBuffers );
  }
  top_profile_meta.ParseFromBuffer ( &rdf[i], (XMP_StringLen) strlen(rdf) - i );

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
    SXMPUtils::ApplyTemplate(&xmpMeta, top_profile_meta, kXMPTemplate_AddNewProperties | kXMPTemplate_ReplaceExistingProperties | kXMPTemplate_IncludeInternalProperties);
    if (hasCrop) {
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropTop", std::to_string(cropTop));
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropLeft", std::to_string(cropLeft));
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropBottom", std::to_string(cropBottom));
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropRight", std::to_string(cropRight));
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropAngle", std::to_string(cropAngle));
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "CropConstrainToWarp", "0");
      xmpMeta.SetProperty("http://ns.adobe.com/camera-raw-settings/1.0/", "HasCrop", "True");
    } else {
      xmpMeta.SetProperty("http://ns.adobe.com/xap/1.0/", "Label", "Red");
    }


    XMP_OptionBits outOpts = kXMP_OmitPacketWrapper | kXMP_UseCanonicalFormat;
    xmpMeta.SerializeToBuffer(&xmpDump, outOpts);
    XmpTool::XmpWriter::WriteXmp(xmpDump);

    std::string top_dump;
    top_profile_meta.SerializeToBuffer(&top_dump, outOpts);
    printf("%s",top_dump.c_str());

    // printf("%s",xmpDump.c_str());
  }


  xmpFile.CloseFile(generalCloseFlags);
  return 0;
}

XmpTool::XmpWriter::XmpWriter(std::string pathToRaw, Source::ProcessingTarget processedSource, std::string profileName)
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
  if (processedSource.thumbnailIsLikelyIsolated && processedSource.subjectIsLikelyIsolated) {
    cropLeft = processedSource.cropLeft;
    cropRight = processedSource.cropRight;
    cropTop = processedSource.cropTop;
    cropBottom = processedSource.cropBottom;
    cropAngle = processedSource.cropAngle;
    hasCrop = true;
  } else {
    label = "Red";
    hasCrop = false;
  }


  int len = strlen(path);
  outPathString = pathToRaw.substr(0, len - 4);

  XmpTool::XmpWriter::ProcessFile(path);

  SXMPFiles::Terminate();
  SXMPMeta::Terminate();
}
