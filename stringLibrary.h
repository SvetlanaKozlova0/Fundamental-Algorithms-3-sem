#ifndef STRINGLIBRARY_H
#define STRINGLIBRARY_H

typedef struct String {
	char* symbols;
	int length;
} String;

int StringLength(const char* str);
char* StringCopy(char* newString, const char* oldString);
String CreateString(char* str);
void DestroyString(String* currentString);
int CompareChars(const char* firstChars, const char* secondChars);
int OrderString(const String* firstString, const String* secondString);
int EqualStrings(const String* firstString, const String* secondString);
int CopyStringsOld(String * copy, const String* old);
String* CopyStringsNew(const String* old);
int ConcatenateStrings(String* firstString, const String* secondString);
int CompareCharsOpt(const char* firstChars, const char* secondChars);
char ToLower(char c);
int IsAlpha(char c);
int IsDigit(char c);
int EqualOptions(const String* firstString, const String* secondString);
#endif