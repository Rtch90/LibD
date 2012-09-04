#pragma once
#include <cstring>
namespace saracraft {
namespace filesystem {

struct SC_FILE;

SC_FILE* sc_fopen(const char* filename, const char*);
size_t sc_fread(void* buffer, size_t size, size_t count, SC_FILE* stream);
size_t sc_fsize(SC_FILE* stream);
int sc_fclose(SC_FILE* stream);
int sc_feof(SC_FILE* stream);

} // Namespace filesystem.
} // Namespace saracraft.

