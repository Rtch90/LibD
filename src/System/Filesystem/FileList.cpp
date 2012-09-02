#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "FileList.h"

using namespace std;
using namespace boost;

namespace saracraft {
namespace filesystem {
namespace {

struct Dir;
typedef vector<string> StringList;
typedef map<string, Dir> DirList;
typedef vector<DirList::iterator> IteratorList;

struct Dir {
  StringList files;
  IteratorList subDirs;

  void AddFile(const string& file) {
    for(unsigned int i = 0; i < files.size(); ++i) {
      if(file == files[i])
        return;
    }
    files.push_back(file);
  }
};

void HaxFile(string& str) {
  for(unsigned int i = 0; i < str.size(); ++i) {
    if(str[i] == '\\')
      str[i] = '/';
  }
}

void ConvertLower(string& str) {
  for(unsigned int i = 0; i < str.size(); ++i) {
    str[i] = tolower(str[i]);
  }
}

void GetDirPart(const string& file, string& dir) {
  string::size_type index = file.find_last_of("\\/");
  if(index == file.npos) {
    dir = string();
    return;
  }
  dir = file.substr(0, index);
}

int GetDirAmountImpl(const IteratorList& list) {
  int result = list.size();
  /*for(IteratorList::const_iterator it = list.begin(); it != list.end(); ++i) {
    const DirList::const_iterator& i = *it;
    result += GetDirAmountImpl(i->second.subDirs);
  }*/
  return result;
}

int GetDirNameImpl(const IteratorList& list, string& result, int currentIndex, int findIndex) {
  int original = currentIndex;
  for(IteratorList::const_iterator it = list.begin(); it != list.end(); ++it) {
    const DirList::const_iterator& i = *it;
    if(currentIndex++ == findIndex) {
      result = i->first;
      break;
    }
    //currentIndex += GetDirNameImpl(i->second.subDirs
  }
  return currentIndex - original;
}
string empty;

} // Namespace unamed.
struct FileList::Data {
  bool caseSensitive;
  DirList dirs;

  Data(void) : caseSensitive(false) {}

  // Might want to put some directory hierarchy stuff here?

  void AddDir(const string& dir) {
    DirList::iterator it;
    if(!FindDir(it, dir)) {
      dirs[dir] = Dir();
      FindDir(it, dir);

      // Add a parent to the list.
      string current = dir;
      DirList::iterator currentIt = it;

      for(int i = 0; ; ++i) {
        string parent;
        GetDirPart(current, parent);
        if(parent.empty())
          break;

        bool needAdd = false;

        DirList::iterator parentIt;
        if(!FindDir(parentIt, parent)) {
          dirs[parent] = Dir();
          FindDir(parentIt, parent);
          needAdd = true;
        }
        parentIt->second.subDirs.push_back(currentIt);
        if(!needAdd)
          break;
        current = parent;
        currentIt = parentIt;
      }
    }
  }

  bool FindDir(DirList::iterator& it, const string& dir) {
    it = dirs.find(dir);
    if(it == dirs.end())
      return false;

    return true;
  }
};

FileList::FileList(void) {
 scoped_ptr<Data> tempData(new Data());
 _data.swap(tempData);
}

FileList::~FileList(void) {

}

void FileList::SetCaseSensitivity(bool enable) {
  _data->caseSensitive = enable;
}

void FileList::AddDir(const string& dir_) {
  string dir = dir_;
  if(!_data->caseSensitive)
    ConvertLower(dir);

  HaxFile(dir);
  _data->AddDir(dir);
}

void FileList::AddFile(const string& file_) {
  string file = file_;
  if(!_data->caseSensitive)
    ConvertLower(file);
 HaxFile(file);

 string dir;
 GetDirPart(file, dir);
 _data->AddDir(dir);

 DirList::iterator it;
 _data->FindDir(it, dir);
 it->second.AddFile(file);
}

int FileList::GetDirAmount(const string& root_) const {
  string root = root_;
  if(!_data->caseSensitive)
    ConvertLower(root);
  HaxFile(root);
  DirList::iterator it;

  if(!_data->FindDir(it, root))
    return 0;
  return GetDirAmountImpl(it->second.subDirs);
}

string FileList::GetDirName(const string& root_, int index) const {
  string root = root_;
  if(!_data->caseSensitive)
    ConvertLower(root);
  
  HaxFile(root);
  DirList::iterator it;
  if(!_data->FindDir(it, root))
    return empty;

  string result;
  GetDirNameImpl(it->second.subDirs, result, 0, index);

  return result;
}

int FileList::GetFileAmount(const string& root_) const {
  string root = root_;
  if(!_data->caseSensitive)
    ConvertLower(root);
  HaxFile(root);

  DirList::iterator it;
  if(!_data->FindDir(it, root))
    return 0;
  return it->second.files.size();
}

const string& FileList::GetFileName(const string& root_, int index) const {
  string root = root_;
  if(!_data->caseSensitive)
    ConvertLower(root);
  HaxFile(root);

  DirList::iterator it;
  if(!_data->FindDir(it, root))
    return empty;
  return it->second.files[index];
}

// ============================================================
namespace {
  void GetAllFilesImpl(IFileList& list, const string& root, vector<string>& result, bool recurse) {
    //if(root.find("

    // The hell is your problem?!
    int fileAmount = list.GetFileAmount(root);
    for(int i = 0; i < fileAmount; ++i) {
      const string& file = list.GetFileName(root, i);
      result.push_back(file);
    }
    if(recurse) {
      int dirAmount = list.GetDirAmount(root);
      for(int i = 0; i < dirAmount; ++i) {
        const string& dir = list.GetDirName(root, i);
        if(dir.length() > 4 && dir.substr(dir.length() - 4, 4) == ".svn") {
          // We should never even get here..
        } else {
          GetAllFilesImpl(list, dir, result, recurse);
        }
      }
    }
  }

  void GetAllDirsImpl(IFileList& list, const string& root, vector<string>& result, bool recurse) {
    int dirAmount = list.GetDirAmount(root);
    for(int i = 0; i < dirAmount; ++i) {
      const string& dir = list.GetDirName(root, i);
      if(dir.length() > 4 && dir.substr(dir.length() - 4, 4) == ".svn") {
        // We should never even get here..
      } else {
        result.push_back(dir);

        if(recurse)
          GetAllDirsImpl(list, dir, result, recurse);
      }
    }
  }
} // Unamed namespace.

void GetAllFiles(IFileList& list, const string& root_, vector<string>& result, bool recurse, bool caseSensitive) {
  string root = root_;
  if(!caseSensitive)
    ConvertLower(root);
  HaxFile(root);

  GetAllFilesImpl(list, root, result, recurse);
  std::sort(result.begin(), result.end());
}


void GetAllDirs(IFileList& list, const string& root_, vector<string>& result, bool recurse, bool caseSensitive) {
  string root = root_;
  if(!caseSensitive)
    ConvertLower(root);
  HaxFile(root);

  GetAllDirsImpl(list, root, result, recurse);
  std::sort(result.begin(), result.end());
}

} // Namespace filesystem.
} // Namespace saracraft.

