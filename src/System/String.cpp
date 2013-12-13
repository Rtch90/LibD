#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "String.h"
#define _CRT_SECURE_NO_WARNINGS

String::String(void) {
  _string = new char[1];
  _string[0] = 0;
  _length = 0;
}

String::~String(void) {
  if(_string) {
    delete[] _string;
    _string = NULL;
    _length = 0;
  }
}

String::String(char letter) {
  // Char to string conversion.
  _string = new char[2];
  _string[0] = letter;
  _string[1] = 0;
  _length = 1;
}

String::String(const char* text) {
  _length = strlen(text);
  _string = new char[_length + 1];
  memcpy(_string, text, _length);
  _string[_length] = 0;
}

String::String(const String& text) {
  _length = text._length;
  _string = new char[_length + 1];
  memcpy(_string, text._string, _length);
  _string[_length] = 0;
}

const char* String::GetPointer(void) const {
  // Return a pointer to the memory address of the string.
  return _string;
}

int String::Length(void) {
  // Return the length of the string.
  return _length;
}

void String::Format(const char* format, ...) {
  if(_string) {
    delete[] _string;
  }

  _length = 4095;
  _string = new char[_length + 1];
  memset(_string, 0, _length + 1);

  va_list vlist;

  va_start(vlist, format);
#ifdef _WIN32
  vsprintf_s(_string, _length, format, vlist);
#else
  vsnprintf(_string, _length, format, vlist);
#endif
  va_end(vlist);
}

void String::Concatenate(char value) {
  char* oldString = _string;
  int oldLength = _length;
  _length = oldLength + 1;
  _string = new char[_length + 1];
  _string[_length] = 0;
  memcpy(_string, oldString, oldLength);
  _string[oldLength] = value;
  delete[] oldString;
}

void String::Concatenate(const char* value) {
  char* oldString = _string;
  int oldLength = _length;
  int valueLength = strlen(value);
  _length = oldLength + valueLength;
  _string = new char[_length + 1];
  _string[_length] = 0;
  memcpy(_string, oldString, oldLength);
  memcpy(_string + oldLength, value, valueLength);
  delete[] oldString;
}

void String::Concatenate(const String& value) {
  Concatenate(value.GetPointer());
}

// Operator overloads, can't be bothered to comment
// them, use your brain. :)
String& String::operator=(const char* value) {
  _length = strlen(value);
  _string = new char[_length + 1];
  _string[_length] = 0;
  memcpy(_string, value, _length);
  return *this;
}

String& String::operator=(const String& value) {
  *this = value.GetPointer();
  return *this;
}

bool String::operator==(const char* value) const {
  if(strcmp(_string, value) == 0) {
    return true;
  }
  return false;
}

bool String::operator==(const String& value) const {
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

String String::operator+(const String& value) const {
  String copy(*this);
  copy.Concatenate(value);
  return copy;
}

String::operator const char*() const {
  return _string;
}
