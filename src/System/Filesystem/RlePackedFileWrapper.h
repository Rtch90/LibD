#pragma once

// A silly read/write "chunk packed" file.
// *Must* read data in the same sized chunks as it was written!
// Also reading/writing in really small chunks will just result in extremely
// large files..

struct RLE_PACKED_FILE;

// Return true if the file seems to be rle packed.
bool RlePackedDetect(const char* filename);

RLE_PACKED_FILE* RlePacked_fopen(const char* filename, const char* params);

size_t RlePacked_fread(void* buffer, size_t size, size_t count, RLE_PACKED_FILE* stream);

size_t RlePacked_fwrite(void* buffer, size_t size, size_t count, RLE_PACKED_FILE* stream);

long RlePacked_fsize(RLE_PACKED_FILE* stream);

bool RlePackedWasError(RLE_PACKED_FILE* stream);

int RlePacked_fclose(RLE_PACKED_FILE* stream);

