#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED


#include <windows.h>



class WinException
{
public:
    WinException (char* msg)
    : _err (GetLastError()), _msg(msg)
    {}
    DWORD GetError() const { return _err; }
    char* GetMessage () const { return _msg; }
private:
    DWORD _err;
    char* _msg;
};

// out of memory handler that throws WinException
int NewHandler (size_t size);



#endif // _MAIN_H_INCLUDED
