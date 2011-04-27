// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in irrlicht.h and irrXML.h

#ifndef __FAST_ATOF_H_INCLUDED__
#define __FAST_ATOF_H_INCLUDED__

#include "irrMath.h"

namespace irr
{
namespace core
{

// we write [17] here instead of [] to work around a swig bug
const float fast_atof_table[17] = {
	0.f,
	0.1f,
	0.01f,
	0.001f,
	0.0001f,
	0.00001f,
	0.000001f,
	0.0000001f,
	0.00000001f,
	0.000000001f,
	0.0000000001f,
	0.00000000001f,
	0.000000000001f,
	0.0000000000001f,
	0.00000000000001f,
	0.000000000000001f,
	0.0000000000000001f
};

//! Convert a simple string of base 10 digits into an unsigned 32 bit integer.
/** \param[in] in: The string of digits to convert. No leading chars are
    allowed, only digits 0 to 9. Parsing stops at the first non-digit.
    \param[out] out: (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The unsigned integer value of the digits. If the string specifies
    too many digits to encode in an u32 then INT_MAX will be returned.
*/
inline u32 strtoul10(const char* in, const char** out=0)
{
	if (!in)
	{
		if (out)
			*out = in;
		return 0;
	}

	bool overflow=false;
	u32 unsignedValue = 0;
	while ( ( *in >= '0') && ( *in <= '9' ))
	{
		const u32 tmp = ( unsignedValue * 10 ) + ( *in - '0' );
		if (tmp<unsignedValue)
		{
			unsignedValue=(u32)0xffffffff;
			overflow=true;
		}
		if (!overflow)
			unsignedValue = tmp;
		++in;
	}

	if (out)
		*out = in;

	return unsignedValue;
}

//! Convert a simple string of base 10 digits into a signed 32 bit integer.
/** \param[in] in: The string of digits to convert. Only a leading - or +
    followed by digits 0 to 9 will be considered. Parsing stops at the first
    non-digit.
    \param[out] out: (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The signed integer value of the digits. If the string specifies
    too many digits to encode in an s32 then +INT_MAX or -INT_MAX will be
    returned.
*/
inline s32 strtol10(const char* in, const char** out=0)
{
	if (!in)
	{
		if (out)
			*out = in;
		return 0;
	}

	const bool negative = ('-' == *in);
	if (negative || ('+' == *in))
		++in;

	const u32 unsignedValue = strtoul10(in,out);
	if (unsignedValue > (u32)INT_MAX)
	{
		if (negative)
			return (s32)INT_MIN;
		else
			return (s32)INT_MAX;
	}
	else
	{
		if (negative)
			return -((s32)unsignedValue);
		else
			return (s32)unsignedValue;
	}
}

//! Convert a simple string of base 16 digits into an unsigned 32 bit integer.
/** \param[in] in: The string of digits to convert. No leading chars are
    allowed, only digits 0 to 9 and chars A-F,a-f are allowed. Parsing stops
    at the first illegal char.
    \param[out] out: (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The unsigned integer value of the digits. If the string specifies
    too many digits to encode in an u32 then INT_MAX will be returned.
*/
inline s32 strtoul16(const char* in, const char** out=0)
{
	if (!in)
	{
		if (out)
			*out = in;
		return 0;
	}

	bool overflow=false;
	u32 unsignedValue = 0;
	while (true)
	{
		u32 tmp = 0;
		if ((*in >= '0') && (*in <= '9'))
			tmp = (unsignedValue << 4u) + (*in - '0');
		else if ((*in >= 'A') && (*in <= 'F'))
			tmp = (unsignedValue << 4u) + (*in - 'A') + 10;
		else if ((*in >= 'a') && (*in <= 'f'))
			tmp = (unsignedValue << 4u) + (*in - 'a') + 10;
		else
			break;
		if (tmp<unsignedValue)
		{
			unsignedValue=(u32)INT_MAX;
			overflow=false;
		}
		if (!overflow)
			unsignedValue = tmp;
		++in;
	}

	if (out)
		*out = in;

	return unsignedValue;
}

//! Converts a sequence of digits into a whole positive floating point value.
/** Only digits 0 to 9 are parsed.  Parsing stops at any other character, 
    including sign characters or a decimal point.
    \param in: the sequence of digits to convert.
    \param out: (optional) will be set to point at the first non-converted
    character.
    \return The whole positive floating point representation of the digit
    sequence.
*/
inline f32 strtof10(const char* in, const char** out = 0)
{
	if (!in)
	{
		if (out)
			*out = in;
		return 0.f;
	}

	const u32 MAX_SAFE_U32_VALUE = UINT_MAX / 10 - 10;
	u32 intValue = 0;

	// Use integer arithmetic for as long as possible, for speed
	// and precision.
	while ( ( *in >= '0') && ( *in <= '9' ) )
	{
		// If it looks like we're going to overflow, bail out
		// now and start using floating point.
		if (intValue >= MAX_SAFE_U32_VALUE)
			break;

		intValue = (intValue * 10) + (*in - '0');
		++in;
	}

	f32 floatValue = (f32)intValue;

	// If there are any digits left to parse, then we need to use 
	// floating point arithmetic from here.
	while ( ( *in >= '0') && ( *in <= '9' ) )
	{
		floatValue = (floatValue * 10.f) + (f32)(*in - '0');
		++in;
		if (floatValue > FLT_MAX) // Just give up.
			break;
	}

	if (out)
		*out = in;

	return floatValue;
}

//! Provides a fast function for converting a string into a float.
/** This is not guaranteed to be as accurate as atof(), but is 
    approximately 6 to 8 times as fast.
    \param[in] in: The string to convert. 
    \param[out] out: The resultant float will be written here.
    \return A pointer to the first character in the string that wasn't
    used to create the float value.
*/
inline const char* fast_atof_move(const char * in, f32& result)
{
	// Please run this regression test when making any modifications to this function:
	// https://sourceforge.net/tracker/download.php?group_id=74339&atid=540676&file_id=298968&aid=1865300

	result = 0.f;
	if (!in)
		return 0;

	const bool negative = ('-' == *in);
	if (negative)
		++in;

	f32 value = strtof10(in, &in);

	if ('.' == *in)
	{
		const char* afterDecimal = ++in;
		const f32 decimal = strtof10(in, &afterDecimal);
		value += decimal * fast_atof_table[afterDecimal - in];
		in = afterDecimal;
	}

	if ('e' == *in || 'E' == *in)
	{
		++in;
		// Assume that the exponent is a whole number.
		// strtol10() will deal with both + and - signs,
		// but calculate as f32 to prevent overflow at FLT_MAX
		value *= powf(10.f, (f32)strtol10(in, &in));
	}

	result = negative?-value:value;
	return in;
}

//! Convert a string to a floating point number
/** \param floatAsString: The string to convert. 
*/
inline float fast_atof(const char* floatAsString)
{
	float ret;
	fast_atof_move(floatAsString, ret);
	return ret;
}

} // end namespace core
} // end namespace irr

#endif

