namespace XmpTool
{
  class XmpWriter
  {
    public:
      XmpWriter(std::string pathToRaw);
      void WriteMinorLabel ( FILE * log, const char * title );
      int ProcessFile (const char * pathToRaw);
  };
}
