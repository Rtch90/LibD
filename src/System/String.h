#pragma once
#define _CRT_SECURE_NO_WARNINGS

class String {
public:
  String(void);
  String(char letter);
  String(const char* text);
  String(const String& text);
  ~String(void);

  const char* GetPointer(void) const;
  void Concatenate(const char* value);
  void Concatenate(const String& value);
  void Concatenate(char value);
  int Length(void);

  void Format(const char* format, ...);

  // Operator overloads.
  String& operator=(const char* value);
  String& operator=(const String& value);

  bool operator==(const char* value) const;
  bool operator==(const String& value) const;
  bool operator!=(String& value) const;
  bool operator!=(const char* value) const;

  String operator+(const String& value) const;

  operator const char*() const;

private:
  char* _string;
  int _length;
};
