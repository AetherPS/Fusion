#include "Common.h"
#include "StringUtils.h"

char* strrchr(char* str, char ch) 
{
	char* lastOccurrence = 0;

	// Iterate through the string
	while (*str != '\0')
	{
		// If the character matches, update the last occurrence pointer
		if (*str == ch)
		{
			lastOccurrence = str;
		}

		// Move to the next character
		str++;
	}

	// Return the pointer to the last occurrence or NULL if not found
	return lastOccurrence;
}

char* FileNameFromPath(char* path)
{
	// Find the position of the last directory separator
	char* lastSeparator = strrchr(path, '/');

	// If no forward slash is found, try backslash for Windows paths
	if (!lastSeparator)
	{
		lastSeparator = strrchr(path, '\\');
	}

	// If no separator is found, the path itself is the file name
	if (!lastSeparator)
	{
		return path;
	}

	// Return the substring after the last separator
	return lastSeparator + 1;
}