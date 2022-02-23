namespace XmpTool
{
  class XmpWriter
  {
    public:
      XmpWriter(std::string pathToRaw);
      int ProcessFile (const char * pathToRaw);
  };
}
