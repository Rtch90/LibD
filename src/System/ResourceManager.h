#pragma once

#include <string>
#include <map>

#include "../System/String.h"

namespace saracraft {
namespace util {

class Resource {
public:
  virtual bool Load(const std::string& filename) = 0;

protected:
  Resource() : refs(0) {}
  int refs;
};

template<class T>
class ResourceManager
{
public:
  ResourceManager() { }

  typedef std::map<std::string, T*> ResourceMap;

  T* Load(const std::string& name) {
    typename ResourceManager<T>::ResourceMap::iterator i = m_resources.find(name);
    if(i == m_resources.end()) {
      T* resource = new T();

    if(!resource->Load(name)) {
        delete resource;
        return NULL;
    }

      resource->refs = 1;

      m_resources.insert(std::pair<std::string, T*>(name, resource));

      return resource;
  }

    i->second->refs++;

    return i->second;
  }

  T* Find(const std::string& name) {
    typename ResourceManager<T>::ResourceMap::iterator i = m_resources.find(name);
    if(i != m_resources.end()) {
      return i->second;
    }
    return NULL;
  }

  void Add(const std::string& name, T* resource) {
    typename ResourceManager<T>::ResourceMap::iterator i = m_resources.find(name);
    if(i == m_resources.end()) {
      resource->refs++;
      m_resources.insert(std::pair<std::string, T*>(name, resource));
    } else {
     i->second->refs++;
    }
  }

  void Destroy(const std::string& name) {
    if(name.empty())
      return;

    typename ResourceManager<T>::ResourceMap::iterator i = m_resources.find(name);
    if(i != m_resources.end()) {
      T* resource = i->second;

      resource->refs--;

      if(resource->m_refs < 1) {
        delete resource;
        m_resources.erase(i);
      }
    }
  }

  void Destroy(T* resource) {
    typename ResourceManager<T>::ResourceMap::iterator i;
    for(i = m_resources.begin(); i != m_resources.end(); ++i) {
      if(i->second == resource){
        resource->refs--;
        if(resource->refs < 1) {
          m_resources.erase(i);
          delete resource;
        }
        return;
      }
    }
  }

private:
  ResourceMap m_resources;
};

} // Namespace util.
} // Namespace saracraft.
