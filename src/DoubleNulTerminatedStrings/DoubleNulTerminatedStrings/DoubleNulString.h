////////////////////////////////////////////////////////////////////////////////
//
//                      Double-NUL-Terminated String Module
//
// By Giovanni Dicanio <giovanni.dicanio AT gmail.com>
//
// FILE: DoubleNulString.h
// DESC: Public interface
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GIOVANNI_DICANIO_DOUBLENULSTRING_H
#define GIOVANNI_DICANIO_DOUBLENULSTRING_H

#include <vector>       // For std::vector
#include <atlstr.h>     // For CString

namespace win32
{

//
// Given a vector of CStrings, "flatten" it to a double-NUL-terminated string.
// This can come in handy e.g. to store several strings in the registry 
// in the REG_MULTI_SZ format.
//
// NOTE: An empty input vector is translated to a sequence of two NULs.
//
std::vector<wchar_t> BuildDoubleNulTerminatedString(const std::vector<CString>& strings);


//
// Given a pointer to a double-NUL-terminated string, traverses this data structure
// and parses it, extracting the single strings, and returning them as a nice
// vector of CStrings.
//
// NOTE: If the input pointer is nullptr, or the input double-NUL-terminated string structure 
// is empty, returns an empty vector.
//
std::vector<CString> ParseDoubleNulTerminatedString(const wchar_t* doubleNulTerminatedString);

} // namespace win32


#endif // GIOVANNI_DICANIO_DOUBLENULSTRING_H
