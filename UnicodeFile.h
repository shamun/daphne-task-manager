#pragma once
#include "afx.h"

class CUnicodeFile :
  public CStdioFile
{
public:
  CUnicodeFile(void);
public:
  ~CUnicodeFile(void);
public:
  BOOL ReadUnicodeString(CString& rString);
  BOOL ReadUTF8String(CString& rString);
};
