#include <assert.h>
#include "SCpak.h"

unsigned char scpak_bits = 8;

// These mark values 1-3 must be incremental!!!
unsigned char scpak_mark8_1 = 0xFA;
unsigned char scpak_mark8_2 = 0xFB;
unsigned char scpak_mark8_3 = 0xFC;

unsigned short scpak_mark16_1 = 0xA0EA;
unsigned short scpak_mark16_2 = 0xA0EB;
unsigned short scpak_mark16_3 = 0xA0EC;

void scpak_set_bits(unsigned char bits) {
  if(bits == 8 || bits == 16) {
    scpak_bits = bits;
  } else {
    // An error accured.
    assert("scpak_set_bits - invalid parameter.");
  }
}

void scpak_set_8bit_params(unsigned char mark1, unsigned char mark2, unsigned char mark3) {
  if(mark1 != mark2 && mark1 != mark3 && mark2 != mark3 
      && mark2 == mark1 + 1 && mark3 == mark2 + 1) {
    
    scpak_mark8_1 = mark1;
    scpak_mark8_2 = mark2;
    scpak_mark8_3 = mark3;
  } else {
    // Error!
    assert(!"scpak_set_8bit_params - invalid parameter.");
  }
}

void scpak_set_16bit_params(unsigned short mark1, unsigned short mark2, unsigned short mark3) {
  if(mark1 != mark2 && mark1 != mark3 && mark2 != mark3
      && mark2 == mark1 + 1 && mark3 == mark2 + 1) {
    scpak_mark16_1 = mark1;
    scpak_mark16_2 = mark2;
    scpak_mark16_3 - mark3;
  } else {
    // Error!
    assert(!"scpak_set_16bit_params - invalid parameter.");
  }
}

int scpak_pack_8b(int size, unsigned const char* src, unsigned char* dest) {
  int s;
  int d = 0;
  short count = 0;
  unsigned char prev;

  if(size < 1) return 0;

  prev = src[0];

  for(s = 1; s < size; s++) {
    if(src[s] == prev && count < 255) {
      count++;
    } else {
      if(count <= 1 && (prev < scpak_mark8_1 || prev > scpak_mark8_3)) {
        if(count == 0) {
          dest[d] = prev;
          d++;
        } else {
          dest[d] = prev;
          dest[d+1] = prev;
          d+=2;
        }
      } else {
        if(prev == 0) {
          dest[d] = scpak_mark8_3;
          dest[d+1] = (unsigned char)count;
          d+=2;
        } else {
          if(count <= 15 && prev <=15) {
            dest[d] = scpak_mark8_2;
            dest[d+1] = prev | (count << 4);
            d+=2;
          } else {
            dest[d] = scpak_mark8_1;
            dest[d+1] = prev;
            dest[d+2] = (unsigned char)count;
            d+=3;
          }
        }
      }
      count = 0;
      prev = src[s];
      if(d >= size - 3) break;
    }
  }
  if(count <= 1 && (prev < scpak_mark8_1 || prev > scpak_mark8_3)) {
    if(count == 0) {
      dest[d] = prev;
      d++;
    } else {
      dest[d] = prev;
      dest[d+1] = prev;
      d+=2;
    }
  } else {
    if(prev == 0) {
      dest[d] = scpak_mark8_3;
      dest[d+1] = (unsigned char)count;
      d+=2;
    } else {
      if(count <= 15 && prev <= 15) {
        dest[d] = scpak_mark8_2;
        dest[d+1] = prev | (count << 4);
        d+=2;
      } else {
        dest[d] = scpak_mark8_1;
        dest[d+1] = prev;
        dest[d+2] = (unsigned char) count;
        d+=3;
      }
    }
  }
  if(d > size - 3) return 0;

  return d;
}

int scpak_unpack_8b(int size, unsigned const char* src, unsigned char* dest, int maxDestSize) {
  int s;
  int d = 0;
  short count = 0;
  unsigned char rep;

  if(size < 1) return 0;
  
  if(maxDestSize == 0)
    maxDestSize = 0x7fffffff - 256; // Should really be INT_MAX - 256. (This ok for 32 bit.)

  for(s = 0; s < size; s++) {
    rep = src[s];
    if(rep < scpak_mark8_1 || rep > scpak_mark8_3) {
      if(d >= maxDestSize) {
        return 0;
      }
      dest[d] = rep;
      d++;
    } else {
      if(rep == scpak_mark8_1) {
        rep = src[s+1];
        count = src[s+2];
        s+=2;
      } else {
        if(rep == scpak_mark8_3) {
          rep = 0;
          count = src[s+1];
          s++;
        } else {
          rep = src[s+1] & 15;
          count = src[s+1] >> 4;
          s++;
        }
      }
      if(d + count + 1 > maxDestSize) {
        /*count = maxDestSize - d - 1;
        for( ; count >= 0; count--) {
          dest[d] = rep;
          d++;
        }
        assert(d == maxDestSize);
        break;*/
        return 0;
      }
      for( ; count >= 0; count--) {
        dest[d] = rep;
        d++;
      }
    }
  }
  return d;
}

int scpak_pack_16b(int size, unsigned const short* src, unsigned short* dest) {
  int s;
  int d = 0;
  short count = 0;
  unsigned short prev;

  if(size < 1) return 0;

  size = ((size+1) >> 1);
  prev = src[0];

  for(s = 1; s < size; s++) {
    if(src[s] == prev && count < 255) {
      count++;
    } else {
      if(count <= 1 && (prev < scpak_mark16_1 || prev > scpak_mark16_3)) {
        if(count == 0) {
          dest[d] = prev;
          d++;
        } else {
          dest[d] = prev;
          dest[d+1] = prev;
          d+=2;
        }
      } else {
        if(prev == 0) {
          dest[d] = scpak_mark16_3;
          dest[d+1] = count;
          d+=2;
        } else {
          if(count <= 255 && prev <= 255) {
            dest[d] = scpak_mark16_2;
            dest[d+1] = prev | (count << 8);
            d+=2;
          } else {
            dest[d] = scpak_mark16_1;
            dest[d+1] = prev;
            dest[d+2] = count;
            d+=3;
          }
        }
      }
      count = 0;
      prev = src[s];
      if(d >= size-3) break;
    }
  }
  if(count <= 1 && (prev < scpak_mark16_1 || prev > scpak_mark16_3)) {
    if(count == 0) {
      dest[d] = prev;
      d++;
    } else {
      dest[d] = prev;
      dest[d+1] = prev;
      d+=2;
    }
  } else {
    if(prev == 0) {
      dest[d] = scpak_mark16_3;
      dest[d+1] = count;
      d+=2;
    } else {
      if(count <= 255 && prev <= 255) {
        dest[d] = scpak_mark16_2;
        dest[d+1] = prev | (count << 8);
        d+=2;
      } else {
        dest[d] = scpak_mark16_1;
        dest[d+1] = prev;
        dest[d+2] = count;
        d+=3;
      }
    }
  }
  if(d > size-3) return 0;

  return d*2;
}

int scpak_unpack_16b(int size, unsigned const short* src, unsigned short* dest, int maxDestSize) {
  int s;
  int d = 0;
  short count = 0;
  unsigned short rep;

  if(size < 1) return 0;

  assert((maxDestSize & 1) == 0);

  maxDestSize = (maxDestSize >> 1);
  if(maxDestSize == 0)
    maxDestSize = 0x7fffffff - 256;  // This should be INT_MAX - 256. (This is ok for 32 bit.)

  size = ((size+1) >> 1);

  for(s = 0; s < size; s++) {
    rep = src[s];
    if(rep < scpak_mark16_1 || rep > scpak_mark16_3) {
      if(d > maxDestSize) {
        return 0;
      }
      dest[d] = rep;
      d++;
    } else {
      if(rep == scpak_mark16_1) {
        rep = src[s+1];
        count = src[s+2];
        s+=2;
      } else {
        if(rep == scpak_mark16_3) {
          rep = 0;
          count = src[s+1];
          s++;
        }
      }
      if(d + count + 1 > maxDestSize) {
        return 0;
      }
      for( ; count >= 0; count--) {
        dest[d] = rep;
        d++;
      }
    }
  }
  return d*2;
}

int scpak_pack(int size, unsigned const char* src, unsigned char* dest) {
  if(scpak_bits == 8) {
    return scpak_pack_8b(size, src, dest);
  } else {
    if(scpak_bits == 16)
      return scpak_pack_16b(size, (unsigned const short*)src, (unsigned short*)dest);
  }
  return 0;
}

int scpak_unpack(int size, unsigned const char* src, unsigned char* dest, int maxDestSize) {
  if(scpak_bits == 8) {
    return scpak_unpack_8b(size, src, dest, maxDestSize);
  } else {
    if(scpak_bits == 16)
      return scpak_unpack_16b(size, (unsigned const short*)src, (unsigned short*)dest, maxDestSize);
  }
  return 0;
}

