#pragma once

namespace saracraft {
namespace util {

extern char* int2str(int value);
extern int   str2int(const char* string);
extern int   str2int_errno(void);
extern char* time2str(int secs); // hh:mm::ss.

} // Namespace util.
} // Namespace saracraft.
