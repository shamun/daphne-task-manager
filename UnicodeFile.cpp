#include "stdafx.h"
#include "UnicodeFile.h"

CUnicodeFile::CUnicodeFile(void)
{
}

CUnicodeFile::~CUnicodeFile(void)
{
}

BOOL CUnicodeFile::ReadUnicodeString(CString& rString)
{
  WCHAR buffer;
  rString.Empty();
  while(Read(&buffer, sizeof(buffer)) == sizeof(buffer) && (buffer != '\r' && buffer != '\n'))
    rString.AppendChar(buffer);
  return !rString.IsEmpty();
}

BOOL CUnicodeFile::ReadUTF8String(CString& rString)
{
  CStringA tmpString; // 1 byte char
  char buffer;
  tmpString.Empty();
  while(Read(&buffer, sizeof(buffer)) == sizeof(buffer) && (buffer != '\r' && buffer != '\n'))
    tmpString.AppendChar(buffer);

  wchar_t *pwcs = NULL;
	unsigned int size = 0;

  size = MultiByteToWideChar( CP_UTF8, 0, tmpString.GetBuffer(), -1, NULL, 0);
	if ( size > 0)
	{
		pwcs = new wchar_t[ size+1];
		size = MultiByteToWideChar( CP_UTF8, 0, tmpString.GetBuffer(), -1, pwcs, size + 1);

		rString = pwcs;
		delete [] pwcs;
	}

  return !rString.IsEmpty();
}