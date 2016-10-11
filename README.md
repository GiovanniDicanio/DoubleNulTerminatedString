# DoubleNulTerminatedString
## C++ helper functions to manage double-NUL-terminated strings

by Giovanni Dicanio

The double-NUL-terminated string is a data structure used in several places on the Win32 platform.

For example, the Windows registry can store double-NUL-terminated strings using the `REG_MULTI_SZ` type.  
As another example, the `lpstrFilter` of the [`OPENFILENAME` structure][MSDN_OFN] points to a buffer 
containing a double-NUL-terminated string.

Basically, given a set of strings, a double-NUL-terminated string is built attaching all the strings together, 
separating them with their own embedded `NUL`, and terminating the whole sequence with an additional `NUL`.

For example, given the two strings `"Hello"` and `"Connie"`, this is how a double-NUL-terminated string 
generated from them looks like: 

```
   <----- "Hello" ----->  |   <----- "Connie" ----->  |NUL  
  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  | H | e | l | l | o |\0 | C | o | n | n | i | e |\0 |\0 |
  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                                                   *******
  Note the double-NUL termination --------------------^
```

Note that, since the `NUL` character is used to terminate the whole sequence, you _cannot_ have empty strings
in a double-NUL-termianted string.

And, while in theory the format of an _empty_ double-NUL-termianted string _could_ be a single `NUL`,
in practice it's much better (_safer_) to build empty double-NUL-termianted strings using _two_ `NUL`s.

Once you have a pointer to a double-NUL-termianted string, you can traverse this data structure with code like this:

```c++
  //
  // Traverse a double-NUL-termianted string,
  // given a pointer to the beginning of this data structure.
  //
  for (const wchar_t* currString = doubleNulTerminatedString; // Start from the first string
       *currString != L'\0';                  // L'\0' (NUL) is the sequence terminator
       currString += wcslen(currString) + 1)  // Move forward to the next string in the sequence
  {
      // Do something with currString
  }
```

The C++ code in this repo contains a couple of functions: one to flatten a `std::vector<CString>` in a double-NUL-terminated string
(stored in a `vector<wchar_t>`), and the inverse one to parse a double-NUL-terminated string into a `vector<CString>`.
There is also some simple test code.

I used the `CString` class to store the strings, as `CString` is well integrated in Windows C++ specific frameworks like ATL/WTL and MFC.
The code is easily adaptable to use the `std::wstring` class as well.

This code was developed using Visual Studio 2015 with Update 3.

[MSDN_OFN]: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646839(v=vs.85).aspx
