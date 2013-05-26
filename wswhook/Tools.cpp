#include "Main.h"

CWarsow wsw;

static const vec4_t colorCyan = { 0, 1, 1, 1 };
static const vec4_t colorYellow = { 1, 1, 0, 1 };
static const vec4_t colorBlue = { 0, 0, 1, 1 };
static const vec4_t colorWhite = { 1, 1, 1, 1 };
static const vec4_t colorOrange = { 1, 0.5, 0, 1 };

int Q_vsnprintfz( char *dest, size_t size, const char *format, va_list argptr )
{
	int len;

	assert( dest );
	assert( size );

	len = vsnprintf( dest, size, format, argptr );
	dest[size-1] = 0;

	return len;
}

char *va( const char *format, ... )
{
	va_list	argptr;
	static int str_index;
	static char string[8][2048];

	str_index = ( str_index+1 ) & 7;
	va_start( argptr, format );
	Q_vsnprintfz( string[str_index], sizeof( string[str_index] ), format, argptr );
	va_end( argptr );

	return string[str_index];
}