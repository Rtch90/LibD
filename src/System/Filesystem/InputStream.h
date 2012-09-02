#pragma once

#ifndef INCLUDED_BOOST_SHARED_PTR_HPP
#define INCLUDED_BOOST_SHATED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif
#ifndef INCLUDED_STRING
#define INCLUDED_STRING
#include <string>
#endif

namespace saracraft {
namespace filesystem {

class IInputStreamBuffer {
public:
  virtual ~IInputStreamBuffer(void) { }

  virtual unsigned char PopByte(void) = 0;
  virtual bool IsEof(void) const = 0;
  virtual int GetSize(void) const = 0;

  virtual void PopBytes(char* buffer, int bytes) = 0;
};

class InputStream {
  boost::shared_ptr<IInputStreamBuffer> _streamBuffer;
public:
  InputStream(void);
  ~InputStream(void);

  void SetBuffer(boost::shared_ptr<IInputStreamBuffer> streamBuffer);
  bool IsEof(void) const;
  int GetSize(void) const;

private:
  InputStream& Read(std::string& value);
  InputStream& Read(bool& value);

  InputStream& Read(unsigned char& value);
  InputStream& Read(char& value);
  InputStream& Read(signed char& value);

  InputStream& Read(unsigned short& value);
  InputStream& Read(signed  short& value);

  InputStream& Read(unsigned int& value);
  InputStream& Read(signed int& value);

  InputStream& Read(float& value);
  InputStream& Read(double& value);

  friend InputStream& operator >> (InputStream&, std::string&);
  friend InputStream& operator >> (InputStream&, bool&);
  friend InputStream& operator >> (InputStream&, unsigned char&);
  friend InputStream& operator >> (InputStream&, char&);
  friend InputStream& operator >> (InputStream&, signed char&);
  friend InputStream& operator >> (InputStream&, unsigned short&);
  friend InputStream& operator >> (InputStream&, signed short&);
  friend InputStream& operator >> (InputStream&, unsigned int&);
  friend InputStream& operator >> (InputStream&, signed int&);
  friend InputStream& operator >> (InputStream&, float&);
  friend InputStream& operator >> (InputStream&, double&);

public:
  // Optimized readers.
  InputStream& Read(unsigned char* buffer, int elements);
  InputStream& Read(char* buffer, int elements);
  InputStream& Read(unsigned short* buffer, int elements);
};

inline InputStream& operator >> (InputStream& stream, std::string& value)       { return stream.Read(value); }
inline InputStream& operator >> (InputStream& stream, bool& value)              { return stream.Read(value); }

inline InputStream& operator >> (InputStream& stream, unsigned char& value)     { return stream.Read(value); }
inline InputStream& operator >> (InputStream& stream, char& value)              { return stream.Read(value); }
inline InputStream& operator >> (InputStream& stream, signed char& value)       { return stream.Read(value); }

inline InputStream& operator >> (InputStream& stream, unsigned short& value)    { return stream.Read(value); }
inline InputStream& operator >> (InputStream& stream, signed short& value)      { return stream.Read(value); }

inline InputStream& operator >> (InputStream& stream, unsigned int& value)      { return stream.Read(value); }
inline InputStream& operator >> (InputStream& stream, signed int& value)        { return stream.Read(value); }

inline InputStream& operator >> (InputStream& stream, float& value)             { return stream.Read(value); }
inline InputStream& operator >> (InputStream& stream, double& value)            { return stream.Read(value); }

} // Namespace filesystem.
} // Namespace saracraft.

