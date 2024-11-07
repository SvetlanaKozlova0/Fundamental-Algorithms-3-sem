#include "stringLibrary.h"

#include <stdlib.h>

String CreateString(char* str) {
	String newString;
	int strLength = StringLength(str);
	newString.length = strLength;
	newString.symbols = (char*)malloc(sizeof(char) * (strLength + 1));
	if (newString.symbols == NULL) {
		newString.length = 0;
		newString.symbols = "";
		return newString;
	}
	StringCopy(newString.symbols, str);
	return newString;
}

void DestroyString(String* currentString) {
	currentString->length = 0;
	if (currentString->symbols != NULL) {
		free(currentString->symbols);
		currentString->symbols = NULL;
	}
}

int OrderString(const String* firstString, const String* secondString) {
	if (firstString->length > secondString->length) {
		return 1;
	} else if (firstString->length < secondString->length) {
		return -1;
	}
	return CompareChars(firstString->symbols, secondString->symbols);
}

int EqualStrings(const String* firstString, const String* secondString) {
	if (firstString->length != secondString->length) {
		return 0;
	}
	if (CompareChars(firstString->symbols, secondString->symbols) == 0) {
		return 1;
	}
	return 0;
}

int EqualOptions(const String* firstString, const String* secondString) {
	if (CompareCharsOpt(secondString->symbols, firstString->symbols) != 0) {
		return 0;
	}
	for (int i = StringLength(secondString->symbols); i < StringLength(firstString->symbols); i++) {
		if (firstString->symbols[i] != ' ') return 0;
	}
	return 1;
}

int CompareChars(const char* firstChars, const char* secondChars) {
	size_t firstLen = StringLength(firstChars);
	for (int i = 0; i < firstLen; i++) {
		char first = firstChars[i];
		char second = secondChars[i];
		if (first == second) continue;
		if (first > second)
			return 1;
		else
			return -1;
	}
	return 0;
}

char ToLower(char c) { return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }

int CompareCharsOpt(const char* firstChars, const char* secondChars) {
	size_t firstLen = StringLength(firstChars);
	for (int i = 0; i < firstLen; i++) {
		char first = ToLower(firstChars[i]);
		char second = ToLower(secondChars[i]);
		if (first == second) continue;
		if (first > second)
			return 1;
		else
			return -1;
	}
	return 0;
}

int CopyStringsOld(String* copy, const String* old) {
	char* oldSymbols = copy->symbols;
	if (oldSymbols != NULL) {
		free(oldSymbols);
	}
	copy->length = old->length;
	copy->symbols = (char*)malloc(sizeof(char) * (old->length + 1));
	if (copy->symbols == NULL) {
		copy->symbols = oldSymbols;
		return -1;
	}
	for (size_t i = 0; i < copy->length; i++) {
		copy->symbols[i] = old->symbols[i];
	}
	copy->symbols[copy->length] = '\0';
	return 1;
}

String* CopyStringsNew(const String* old) {
	String* copy = (String*)malloc(sizeof(String));
	if (copy == NULL) {
		return NULL;
	}
	copy->length = old->length;
	copy->symbols = (char*)malloc(sizeof(char) * (old->length + 1));
	if (copy->symbols == NULL) {
		free(copy);
		return NULL;
	}
	for (size_t i = 0; i < copy->length; i++) {
		copy->symbols[i] = old->symbols[i];
	}
	copy->symbols[copy->length] = '\0';
	return copy;
}

int ConcatenateStrings(String* firstString, const String* secondString) {
	char* tempChars = (char*)realloc(firstString->symbols, (firstString->length + secondString->length + 1));
	if (tempChars == NULL) {
		return -1;
	}
	firstString->symbols = tempChars;
	for (int i = 0; i < secondString->length; i++) {
		firstString->symbols[firstString->length + i] = secondString->symbols[i];
	}
	firstString->length += secondString->length;
	firstString->symbols[firstString->length] = '\0';
	return 1;
}

int StringLength(const char* str) {
	int length = 0;
	while (*str != '\0') {
		length++;
		str++;
	}
	return length;
}

char* StringCopy(char* newString, const char* oldString) {
	if (newString == NULL || oldString == NULL) {
		return NULL;
	}
	char* pointerNewString = newString;
	while (*oldString) {
		*newString = *oldString;
		newString++;
		oldString++;
	}
	*newString = '\0';
	return pointerNewString;
}

int IsAlpha(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }

int IsDigit(char c) {
	if (c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

