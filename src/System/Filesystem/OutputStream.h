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

class IOutputStreamBuffer {
public:
  virtual ~IOutputStreamBuffer(void) { }
	
	virtual void PutByte(unsigned char byte) = 0;
};

class OutputStream {
  boost::shared_ptr<IOutputStreamBuffer> _streamBuffer;
  bool _textStrings;
public:
  OutputStream(void);
  ~OutputStream(void);

  void SetBuffer(boost::shared_ptr<IOutputStreamBuffer> streamBuffer);
  void UseTextStrings(void); // Insert pure strings. Put std::endl manually!!!

private:
  OutputStream& Write(std::string& value);
  OutputStream& Write(bool& value);

  OutputStream& Write(unsigned char& value);
  OutputStream& Write(char& value);
  OutputStream& Write(signed char& value);

  OutputStream& Write(unsigned short& value);
  OutputStream& Write(signed  short& value);

  OutputStream& Write(unsigned int& value);
  OutputStream& Write(signed int& value);

  OutputStream& Write(float& value);
  OutputStream& Write(double& value);

  friend OutputStream& operator >> (OutputStream&, std::string&);
  friend OutputStream& operator >> (OutputStream&, bool&);
  friend OutputStream& operator >> (OutputStream&, unsigned char&);
  friend OutputStream& operator >> (OutputStream&, char&);
  friend OutputStream& operator >> (OutputStream&, signed char&);
  friend OutputStream& operator >> (OutputStream&, unsigned short&);
  friend OutputStream& operator >> (OutputStream&, signed short&);
  friend OutputStream& operator >> (OutputStream&, unsigned int&);
  friend OutputStream& operator >> (OutputStream&, signed int&);
  friend OutputStream& operator >> (OutputStream&, float&);
  friend OutputStream& operator >> (OutputStream&, double&);
};

inline OutputStream& operator >> (OutputStream& stream, std::string& value)       { return stream.Write(value); }
inline OutputStream& operator >> (OutputStream& stream, bool& value)              { return stream.Write(value); }

inline OutputStream& operator >> (OutputStream& stream, unsigned char& value)     { return stream.Write(value); }
inline OutputStream& operator >> (OutputStream& stream, char& value)              { return stream.Write(value); }
inline OutputStream& operator >> (OutputStream& stream, signed char& value)       { return stream.Write(value); }

inline OutputStream& operator >> (OutputStream& stream, unsigned short& value)    { return stream.Write(value); }
inline OutputStream& operator >> (OutputStream& stream, signed short& value)      { return stream.Write(value); }

inline OutputStream& operator >> (OutputStream& stream, unsigned int& value)      { return stream.Write(value); }
inline OutputStream& operator >> (OutputStream& stream, signed int& value)        { return stream.Write(value); }

inline OutputStream& operator >> (OutputStream& stream, float& value)             { return stream.Write(value); }
inline OutputStream& operator >> (OutputStream& stream, double& value)            { return stream.Write(value); }

} // Namespace filesystem.
} // Namespace saracraft.

