#ifdef _WIN32
#define "windows.h"
#endif
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "String.h"
#define _CRT_SECURE_NO_WARNINGS

String::String(void) {

}

String::~String(void) {

}

String::String(char letter) {
  // Char to string conversion.
  char temp[1];
  temp[0] = letter;
  strcpy(_string, temp);
}

String::String(char* text) {
  // char* or char array.
  strcpy(_string, text);
}

String::String(String& text) {
  // Copy the String.
  strcpy(_string, text.GetPointer());
}

const char* String::GetPointer(void) {
  // Return a pointer to the memory address of the string.
  return _string;
}

int String::Length(void) {
  // Return the length of the string.
  return strlen(_string);
}

void String::Format(const char* format, ...) {
  char temp[256];
  va_list vlist;

  va_start(vlist, format);
#ifdef _WIN32
  vsprintf_s(&temp[0], 256, format, vlist);
#else
  vsnprintf(&temp[0], 256, format, vlist);
#endif
  va_end(vlist);

  memcpy(_string, temp, strlen(temp)+1);
}

void String::Concatenate(char value) {
  // Concatenate a char on the end of a string.
  assert(strlen(_string) + 1 < MAX_STRING_LEN);
  char temp[1] = "";
  temp[0] = value;
  strncat(_string, temp, 1);
}

void String::Concatenate(const char* value) {
  // Concatenate a char* or array to the end of the string.
  assert(strlen(_string) + strlen(value) < MAX_STRING_LEN);
  strcat(_string, value);
}

void String::Concatenate(String& value) {
  assert(strlen(_string) + strlen(value) < MAX_STRING_LEN);
  strcat(_string, value.GetPointer());
}

// Operator overloads, can't be bothered to comment
// them, use your brain. :)
String& String::operator=(const char* value) {
  assert(strlen(value) < MAX_STRING_LEN);
  strcpy(_string, value);
  return *this;
}

String& String::operator=(String& value) {
  assert(strlen(value) < MAX_STRING_LEN);
  strcpy(_string, value.GetPointer());
  return *this;
}

bool String::operator==(const char* value) const {
  if(strcmp(_string, value) == 0) {
    return true;
  }
  return false;
}

bool String::operator==(String& value) const {
  if(strcmp(_string, value.GetPointer()) == 0) {
    return true;
  }
  return false;
}

bool String::operator!=(String& value) const {
  if(strcmp(_string, value.GetPointer()) != 0) {
    return true;
  }
  return false;
}

bool String::operator!=(const char* value) const {
  if(strcmp(_string, value) != 0) {
    return true;
  }
  return false;
}

String::operator const char*() const {
  return _string;
}
