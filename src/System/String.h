#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAX_STRING_LEN 255

class String {
public:
  String(void);
  String(char letter);
  String(char text[]);
  String(String& text);
  ~String(void);

  const char* GetPointer(void);
  void Concatenate(const char* value);
  void Concatenate(String& value);
  void Concatenate(char value);
  int Length(void);

  // Operator overloads.
  String& operator=(const char* value);
  String& operator=(String& value);

  bool operator==(const char* value) const;
  bool operator==(String& value) const;
  bool operator!=(String& value) const;
  bool operator!=(const char* value) const;

  operator const char*() const;

private:
  char _string[MAX_STRING_LEN];
};
