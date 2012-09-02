#include "AreaMap.h"


// Pragma warnings give me warnings............. -- Allanis.
//#pragma warning(disable:4103)
//#pragma warning(disable:4786)

AreaMap::AreaMap(int sizeX, int sizeY) {
  _impl = new AreaMapImpl(sizeX, sizeY);
}

AreaMap::~AreaMap(void) {
  delete _impl;
}

int AreaMap::GetAreaValue(int x, int y, int areaMask) {
  return (_impl->_areamap[x + y * _impl->_sizeX] & areaMask);
}

bool AreaMap::IsAreaValue(int x, int y, int areaMask, int value) const {
  if((_impl->_areamap[x + y * _impl->_sizeX] & areaMask) == value)
    return true;
  else
    return false;
}

bool AreaMap::IsAreaAnyValue(int x, int y, int areaMask) const {
  if((_impl->_areamap[x + y * _impl->_sizeX] & areaMask) != 0)
    return true;
  else
    return false;
}

void AreaMap::SetAreaValue(int x, int y, int areaMask, int value) {
  _impl->_areamap[x + y * _impl->_sizeX] = ((_impl->_areamap[x + y * _impl->_sizeX] & (~areaMask)) | value);
}

void AreaMap::FillAreaValue(int areaMask, int value) {
  unsigned int size = _impl->_sizeX * _impl->_sizeY;
  for(unsigned int i = 0; i < size; i++) {
    _impl->_areamap[i] = ((_impl->_areamap[i] & (~areaMask)) | value);
  }
}

AREAMAP_DATATYPE* AreaMap::GetInternalBuffer(void) {
  return _impl->_areamap;
}

