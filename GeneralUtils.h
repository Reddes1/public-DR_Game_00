#pragma once

#include <string>
#include <math.h>

//
// Suite of generalise functions, should be useful for generic and game programs.
//


//-------------------------------------------------------------------------------
// General Functions & Classes
//-------------------------------------------------------------------------------

template<class T>
inline void FreeSTLContainer(T& container)
{
	T empty;
	std::swap(container, empty);
}

//-------------------------------------------------------------------------------
// Mathematical Functions
//-------------------------------------------------------------------------------

//Calculate the difference between two numbers, with optional flag to return only positive numbers only
float GetPercentageDifference(float val1, float val2, bool retPositiveOnly = false);

/*
	Calculates percentage one number is of another(i.e 25 of 100 = 25 %).
	Optional flag to adjust the decimal position to make it a whole number
*/
float GetPercentageOfNumber(float lowNum, float highNum, bool adjDecimalPos = false);

//Takes a number and check if negative, inverting to positive if so
template<class T>
inline void InvertIfNegative(T& num) { if (num < 0) num = -num; }

template<class T>
inline T GetDistanceBetweenPoints(T point1, T point2, bool makePositive = true)
{
	T dist = point2 - point1;
	if (makePositive)
		InvertIfNegative(dist);
	return dist;
}


//-------------------------------------------------------------------------------
// Manipulations
//-------------------------------------------------------------------------------

/*
	Given a filename that might have a path and an extension, we need to be able to pull
	the path bit and the file extension bit out
*/
void StripPathAndExtension(std::string& fileName, std::string* pPath = nullptr, std::string* pExt = nullptr);

//wstring to string conversion
std::string WStringToString(const std::wstring& s);
//string to wstring conversion
std::wstring StringtoWString(const std::string s);
