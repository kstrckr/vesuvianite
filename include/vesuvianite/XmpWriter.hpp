namespace XmpTool
{
  class XmpWriter
  {
    public:
      XmpWriter(std::string pathToRaw);
      int ProcessFile (const std::string pathToRaw);
  };
}
