#pragma once
#include <cstring>
namespace saracraft {
namespace filesystem {

struct FB_FILE;

FB_FILE* fb_fopen(const char* filename, const char*);
size_t fb_fread(void* buffer, size_t size, size_t count, FB_FILE* stream);
size_t fb_fsize(FB_FILE* stream);
int fb_fclose(FB_FILE* stream);
int fb_feof(FB_FILE* stream);

} // Namespace filesystem.
} // Namespace saracraft.

