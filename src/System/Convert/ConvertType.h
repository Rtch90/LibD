#pragma once

#ifndef INCLUDED_CASSERT
#define INCLUSED_CASSERT
#include <cassert>
#endif

namespace saracraft {
namespace filesystem {

template<class T>
struct ConvertBase {
  enum { charCount = sizeof(T) };
  union Values {
    unsigned char c[charCount];
    T t;
  };

  Values value;

  int GetSize(void) const { return charCount; }
};

template<class T>
struct ConvertFrom: private ConvertBase<T> {
  explicit ConvertFrom(const T& t) {
    this->value.t = t;
  }
  using ConvertBase<T>::GetSize;

  unsigned char GetByte(int index) const  {
    assert((index >= 0) && (index < GetSize()));
    return this->value.c[index];
  }
};

template<class T>
struct ConvertTo: private ConvertBase<T> {
  using ConvertBase<T>::GetSize;

  void SetByte(int index, unsigned char c) {
    assert((index >= 0) && (index < GetSize()));
    this->value.c[index] = c;
  }

  const T& GetValue(void) const {
    return this->value.t;
  }
};

typedef unsigned short uint16_t;

} // Namespaces....
}

