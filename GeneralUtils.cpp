#include "GeneralUtils.h"

#include <assert.h>

#include <vector>

#include <codecvt>          //For wstring conversion
#include <comdef.h>         //_com_error, ::Wide..


//-------------------------------------------------------------------------------
// Mathematical Functions
//-------------------------------------------------------------------------------

float GetPercentageDifference(float val1, float val2, bool retPositiveOnly)
{
    float percentage = ((val1 - val2) / ((val1 + val2) / 2)) * 100;

    if (retPositiveOnly && percentage < 0)
        return -percentage;
    else
        return percentage;
}

float GetPercentageOfNumber(float lowNum, float highNum, bool adjDecimalPos)
{
    float percentage = (lowNum / highNum);
    if (adjDecimalPos)
        return percentage * 100;
    else
        return percentage;
}

//-------------------------------------------------------------------------------
// Manipulations
//-------------------------------------------------------------------------------


void StripPathAndExtension(std::string& fileName, std::string* pPath, std::string* pExt)
{
    if (pPath)
        *pPath = "";
    if (pExt)
        *pExt = "";
    //filename only
    std::string::size_type n = fileName.find_last_of("\\/");
    if (n != std::string::npos)
    {
        if (pPath)
            pPath->append(fileName.c_str(), n + 1);
        fileName.erase(0, n + 1);
    }
    n = fileName.find_last_of(".");
    assert(n != std::string::npos);
    if (pExt)
        *pExt = fileName.substr(n);
    fileName.erase(n, fileName.length());
}


std::string WStringToString(const std::wstring& s)
{
    const int size = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL);

    std::vector<char> buf(size);
    ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL);

    return std::string(&buf[0]);
}

std::wstring StringtoWString(const std::string s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring newStr = converter.from_bytes(s);

    return newStr;
}

