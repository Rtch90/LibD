#pragma once

#define AREAMAP_DATATYPE unsigned short
#define AREAMAP_MAX_DATA_VALUE 0xffff

class AreaMapImpl {
private:
  AreaMapImpl(int sizeX, int sizeY) {
    _sizeX = sizeX;
    _sizeY = sizeY;
    _areamap = new AREAMAP_DATATYPE[sizeX * sizeY];
    for(int i = 0; i < sizeX * sizeY; i++) {
      _areamap[i] = 0;
    }
  }

  ~AreaMapImpl(void) {
    delete [] _areamap;
  }

  int _sizeX, _sizeY;
  AREAMAP_DATATYPE* _areamap;

  friend class AreaMap;
};

class AreaMap {
public:
  AreaMap(int sizeX, int sizeY);
  ~AreaMap(void);

  // Returns the unshifted value.. You should shift that if you want
  // get it between 0-n.
  int GetAreaValue(int x, int y, int areaMask);

  bool IsAreaValue(int x, int y, int areaMask, int value) const;
  bool IsAreaAnyValue(int x, int y, int areaMask) const;

  // Expects an unshifted value, (Like the one returned by GetAreaValue),
  // if you have a value between 0-n, shift it first before using this..
  void SetAreaValue(int x, int y, int areaMask, int value);

  // Same as above, but this applied to the entire area..
  void FillAreaValue(int areaMask, int value);

  // Mainly for saving and load hacking.
  AREAMAP_DATATYPE* GetInternalBuffer(void);
private:
  AreaMapImpl* _impl;
};

