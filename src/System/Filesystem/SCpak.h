#pragma once

// Size reflects the size of the unpacked source buffer. Destination should be
// slightly over allocated to be save. There is no predefined rule for the destination
// buffer over allocation, +16 bytes will be nice ;)
//
// If using 16 bit packing, buffer sizes must be padded to 2 byte size (pas source with zeroes!)
// returns 0 if packed data would have been larger than source, in which case you need to handle it
// differently..
extern int scpak_pack(int size, unsigned const char* src, unsigned char* dest);

// Size reflects the size of the unpacked source buffer, destination buffer should be large
// enough to fin the unpacked data (size of unpacked data should be written in the
// packfile or somewhere...) maxDestSize gives the maximum allowed size for unpacked data
// or if 0, no limit (could cause a buffer overflow...)
extern int scpak_unpack(int size, unsigned const char* src, unsigned char* dest, int maxDestSize = 0);

// 8 or 16 bit supported.
extern void scpak_set_bits(unsigned char bits);

extern void scpak_set_8bit_params(unsigned char mark1, unsigned char mark2, unsigned char mark3);
extern void scpak_set_16bit_params(unsigned short mark1, unsigned short mark2, unsigned short mark3);

