////////////////////////////////////////////////////////////////////////////////
//
// Testing the Double-NUL-Terminated String Module
//
// By Giovanni Dicanio <giovanni.dicanio AT gmail.com>
//
////////////////////////////////////////////////////////////////////////////////

#include "DoubleNulString.h"    // Module to test
#include <exception>            // For std::exception
#include <iostream>             // For std::cout
#include <stdexcept>            // For std::logic_error


//------------------------------------------------------------------------------
//                      Simple Test Infrastructure
//------------------------------------------------------------------------------

class TestError : public std::logic_error
{
public:
    explicit TestError(const char* message) 
        : std::logic_error(message) 
    {}

    explicit TestError(const std::string& message)
        : std::logic_error(message)
    {}
};


inline void Verify(bool expression, const char* errorMessage)
{
    if (! expression) 
    {
        throw TestError(errorMessage);
    }
}


inline void Verify(bool expression, const std::string& errorMessage)
{
    if (!expression)
    {
        throw TestError(errorMessage);
    }
}


//------------------------------------------------------------------------------
//                                  Test Code
//------------------------------------------------------------------------------

int main()
{
    constexpr int kExitOk = 0;
    constexpr int kExitError = 1;
    int exitCode = kExitOk;

    try
    {
        std::cout << "*** Testing Double-NUL-Terminated String Module ***\n";
        std::cout << "              By Giovanni Dicanio\n\n";

        //
        // Try Build + Parse: the final result should be the same of the initial SZZ
        //
        std::vector<CString> strings{ L"Hello", "World", "Ciao", L"Hi", L"Connie" };
        std::vector<wchar_t> szz = win32::BuildDoubleNulTerminatedString(strings);

        std::vector<CString> reconstructedStrings = win32::ParseDoubleNulTerminatedString(szz.data());
        Verify(reconstructedStrings == strings, 
            "Build + Parse returned different result than the original.");

        //
        // Try building an SZZ from an empty CString vector
        //
        std::vector<CString> emptyInput;
        std::vector<wchar_t> szzEmpty = win32::BuildDoubleNulTerminatedString(emptyInput);
        Verify(szzEmpty.size() == 2, "Empty SZZ should be made by *2* NULs.");
        Verify((szzEmpty[0] == L'\0') && (szzEmpty[1] == L'\0'), 
            "Empty SZZ should be made by 2 *NULs*." );

        //
        // Try parsing an empty SZZ: should get an empty CString vector
        //
        reconstructedStrings = win32::ParseDoubleNulTerminatedString(szzEmpty.data());
        Verify(reconstructedStrings.empty(), "Parsing an empty SZZ should return an empty vector.");
        
        std::cout << "\nAll right!\n";
    }
    catch (const TestError& e)
    {
        std::cout << "\n*** TEST FAILED:\n" << e.what() << '\n';
        exitCode = kExitError;
    }
    catch (const std::exception& e)
    {
        std::cout << "\n*** ERROR: " << e.what() << '\n';
        exitCode = kExitError;
    }

    return exitCode;
}

