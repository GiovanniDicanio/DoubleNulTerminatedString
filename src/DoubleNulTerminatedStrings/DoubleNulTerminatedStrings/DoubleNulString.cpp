////////////////////////////////////////////////////////////////////////////////
//
//                      Double-NUL-Terminated String Module
//
// By Giovanni Dicanio <giovanni.dicanio AT gmail.com>
//
// FILE: DoubleNulString.cpp
// DESC: Module implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DoubleNulString.h"    // Module header
#include <string.h>             // For wcslen
#include <atldef.h>             // For ATLASSERT()


namespace 
{

//
// Helper function to calculate the required total length of the double-NUL-terminated string 
// resulting from flattening the input CString vector.
//
// The total length is in wchar_ts, and includes intermediate and terminating NULs.
//
size_t GetTotalLength(const std::vector<CString>& strings)
{
    if (strings.empty())
    {
        //
        // Two NULs for an empty double-NUL-terminated string.
        //
        // However, in theory, just one single NUL could be sufficient,
        // as an empty string (i.e. L'\0') terminates the sequence...
        // But better be safe for software components that assume that
        // an empty double-NUL-terminated string must have two NULs.
        //
        return 2;
    }

    size_t totalLength = 0;
    for (const auto& s : strings)
    {
        //
        // NOTE: We can't have empty strings in the input vector,
        // as an empty string would *terminate* the sequence.
        //
        ATLASSERT(!s.IsEmpty());

        // Add +1 to current string's length to take into account the terminating NUL
        totalLength += (s.GetLength() + 1);
    }

    // Don't forget the final NUL-terminator!
    totalLength++;

    return totalLength;
}

} // anonymous namespace


namespace win32 
{

std::vector<wchar_t> BuildDoubleNulTerminatedString(const std::vector<CString>& strings)
{
    constexpr wchar_t kNul = L'\0';

    // Special case of empty input vector
    if (strings.empty())
    {
        // Return a vector containing just two NULs
        return std::vector<wchar_t>(2, kNul);
    }

    // This will store the double-NUL-terminated string
    std::vector<wchar_t> result;

    // Reserve room in the vector before adding strings, 
    // to avoid reallocations and intermediate copies
    result.reserve(GetTotalLength(strings));

    // Insert each input string (including its NUL terminator!) in the destination vector
    for (const auto& s : strings)
    {
        // Note: The sequence cannot contain an empty string, as an empty string
        // is a terminator for the whole double-NUL-terminated string data structure.
        ATLASSERT(!s.IsEmpty());

        const wchar_t* const start = s.GetString();

        // Add +1 to current string's length to consider also the string's terminating NUL 
        const wchar_t* const end = start + s.GetLength() + 1;

        // Add this new string (*including* its NUL-terminator) to the destination wchar_t array
        result.insert(result.end(), start, end);
    }

    // Add the final (double-)NUL-terminator
    result.push_back(kNul);

    return result;
}


std::vector<CString> ParseDoubleNulTerminatedString(const wchar_t* const pszz)
{
    std::vector<CString> result;

    // Special case of empty input
    if ((pszz == nullptr) || (*pszz == L'\0'))
    {
        // Just return an empty vector
        ATLASSERT(result.empty());
        return result;
    }

    // Traverse the input double-NUL-terminated string
    for (const wchar_t* currString = pszz;
        *currString != L'\0';
        currString += wcslen(currString) + 1)
    {
        // Add (deep-copying) current string to the result vector
        result.push_back(CString(currString));
    }

    return result;
}

} // namespace win32

