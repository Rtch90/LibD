#include <map>
#include <stack>
#include <vector>
// Minizip! -- Included this in libs as it is a bit of a pain.
#include "../../../Libs/MiniZip/unzip.h"

#include "EmptyBuffer.h"
#include "IFileList.h"
#include "../Convert/str2int.h"
#include "../Debug.h"
#include "ZipPackage.h"

namespace saracraft {
namespace filesystem {
namespace {

static const int BUFFER_SIZE = 100*1024;

// --

void ConvertLower(std::string& str) {
  for(unsigned int i = 0; i < str.size(); ++i) {
    str[i] = tolower(str[i]);
    if(str[i] == '\\')
      str[i] = '/';
  }
}

int FindTokenIndex(const std::string& str, const std::string& token, std::string::size_type start) {
  if(token.size() >= str.size() - start)
    return str.npos;

  for(std::string::size_type i = start; i <= str.size() - token.size(); ++i) {
    bool found = true;
    for(std::string::size_type j = 0; j < token.size(); ++j) {
      if(token[j] != str[i+j]) {
        found = false;
        break;
      }
    }
    if(found)
      return i;
  }
  return str.npos;
}

bool ContainsToken(const std::string& string, const std::string& token, std::string::size_type start) {
  if(start + token.size() >= string.size())
    return false;

  for(unsigned int i = 0; i < token.size(); ++i) {
    if(token[i] != string[i+start]) 
      return false;
  }
  return true;
}

void FindTokens(const std::string& string, std::vector<std::string>& result, const std::string& seperator) {
  std::string::size_type start = string.find_first_of(seperator);
  if(start == string.npos) {
    result.push_back(string);
    return;
  }

  for(;;) {
    std::string::size_type end = string.find_first_of(seperator, start + 1);
    if(end == string.npos) {
      std::string token = string.substr(start+1, string.size() - start - 1);
      result.push_back(token);
      break;
    }
    std::string token = string.substr(start,end-start);
    result.push_back(token);

    start = end;
  }
}

/*void AddFile(std::string& file, IFileList& result) {
  std::vector<std::string> dirs;
  FindTokens(file, dirs, "/");

  std::stack<FileList:Dir*> dirsStack;
  dirStack.push(&result.root);

  for(unsigned int i = 0; i < dirs.size() - 1; ++i) {
    const std::string& dirName = dirs[i];

    FileList::Dir* current = dirStack.top();
    int index = current->FindDirIndex(dirName);
    if(index == -1) {
      FileList::Dir newDir;
      newDir.name = dirName;
      current->dirs.push_back(newDir);
      index = current->dirs.size() - 1;
    }
    dirStack.push(&current->dirs[index]);
  }
  FileList::Dir* final = dirStack.top();
  final->files.puch_back(dirs[dirs.size() - 1]);
}*/

struct ZipFileData {
  int size;
  unz_file_pos filePosition;

  ZipFileData(void) : size(0) {
  }
};

typedef std::map<std::string, ZipFileData> ZipFileList;

struct ZipData {
  unzFile fileId;
  ZipFileList fileList;

  ZipData(const std::string& archive) : fileId(0) {
    fileId = unzOpen(archive.c_str());
    if(fileId)
      FindFiles();
  }
  
  ~ZipData(void) {
    if(fileId)
      unzClose(fileId);
  }
  
  void FindFiles(void) {
    unz_global_info globalInfo = { 0 };
    if(unzGetGlobalInfo(fileId, &globalInfo) != UNZ_OK)
      return;

    char file[1024] = { 0 };
    for(unsigned int i = 0; i < globalInfo.number_entry; ++i) {
      if(i != 0 && unzGoToNextFile(fileId) != UNZ_OK)
        break;
      unz_file_info fileInfo = { 0 };
      if(unzGetCurrentFileInfo(fileId, &fileInfo, file, sizeof(file) - 1, 0, 0, 0, 0) != UNZ_OK)
        return;
      if(fileInfo.uncompressed_size <= 0)
        continue;

      ZipFileData zipFile;
      zipFile.size = fileInfo.uncompressed_size;
      unzGetFilePos(fileId, &zipFile.filePosition);

      std::string filename = file;
      ConvertLower(filename);
      fileList[filename] = zipFile;
    }
  }

  void FindFiles(std::string dir, std::string extension, IFileList& result) {
    ConvertLower(dir);
    ConvertLower(extension);

    if(extension.empty())
      return;

    std::vector<std::string> tokens;
    FindTokens(extension, tokens, "*");
    if(tokens.empty() || tokens.size() > 1)
      return;

    ZipFileList::iterator it = fileList.begin();
    for(; it != fileList.end(); ++it) {
      const std::string& file = it->first;
      if(!ContainsToken(file, dir, 0))
        continue;
      //if(ContainsToken(....)
      
      std::string::size_type fileStart  = file.find_last_of("/");
      std::string::size_type index      = FindTokenIndex(file, tokens[0], fileStart);
    
      // This will only detect search tokens begining with "*" -- FIXME
      if(index == file.size() - tokens[0].size())
        result.AddFile(file);
    
      /* Eh.. Just find each token index (if there is one..) and move those forward
      // hierarchically if needed.
      if(!ContainsToken(file, dir, 0))
        continue;
      if*/
    }
  }

  bool FindFile(std::string file, ZipFileList::iterator& it) {
    ConvertLower(file);

    it = fileList.find(file);
    if(it == fileList.end())
      return false;
    return true;
  }
};

#if READ_CHUNKS
class ZipInputBuffer: public IInputStreamBuffer {
  boost::shared_ptr<ZipData> zipData;
  unsigned char buffer[BUFFER_SIZE];
  int currentPosition;
  int readBytes;
  unsigned long offset;

  ZipFileData fileData;

  /*void Release(void) const {
    
  }*/
  void FillBuffer(void) {
    beginTime();
    int readSize = BUFFER_SIZE;
    if(readBytes + readSize >= fileData.size)
      readSize = fileData.size - readBytes;
    
    currentPossition = 0;
    int id = unzGoToFilePos(zipData->fileId, &fileData.filePosition);
    if(id != UNZ_OK)
      return;
    if(unzOpenCurrentFile(zipData->fileId) != UNZ_OK)
      return;

    if(offset > 0 && if(unzSetOffset(zipData->fileId, readBytes) != UNZ_OK)
      return;

    unzReadCurrentFile(fileData->fileId, buffer, readSize);
    //offset = unzGetOffser(zipData->fileId);
    unzCloseCurrentFile(zipData->fileId);

    endTime("fileBuffer");
  }
  public:
    ZipInputBuffer(boost::shared_ptr<ZipData>& zipData_, const ZipFileData& fileData_) :
      zipData(zipData_),
      fileData(fileData_) {
    
    currentPosition = -1;
    readBytes = 0;
    offset = 0;
  }

  ~ZipInputBuffer(void) {
  }

  unsigned char PopByte(void) {
    if(!zipData->fileId || readBytes >= fileData.size)
      FillBuffer();

    ++readBytes;
    return buffer[currentPosition++];
  }

  bool IsEof(void) const {
    if(!zipData->fileId || readBytes >= fileData.size)
      return true;
    return false;
  }

  int GetSize(void) const {
    return fileData.size;
  }

  void PopBytes(char* buffer, int bytes) {
    for(int i = 0; i < bytes; ++i)
      buffer[i] = PopByte();
  }  
};

#else

class ZipInputBuffer : public IInputStreamBuffer {
  boost::shared_ptr<ZipData> zipData;
  std::vector<unsigned char> buffer;
  int position;

  ZipFileData fileData;

  void FillBuffer(void) {
    int readSize = fileData.size;

    if(unzGoToFilePos(zipData->fileId, &fileData.filePosition) != UNZ_OK)
      return;
    if(unzOpenCurrentFile(zipData->fileId) != UNZ_OK)
      return;

    int bytes = unzReadCurrentFile(zipData->fileId, &buffer[0], readSize);
    assert(bytes == readSize);

    unzCloseCurrentFile(zipData->fileId);
  }
public:
  ZipInputBuffer(boost::shared_ptr<ZipData>& zipData_, const ZipFileData& fileData_) :
    zipData(zipData_),
    fileData(fileData_) {
  
    position = 0;
  }

  ~ZipInputBuffer(void) {
  
  }

  unsigned char PopByte(void) {
    if(position >= fileData.size)
      return 0;
    if(position == 0) {
      buffer.reserve(fileData.size);
      FillBuffer();
    }
    return buffer[position++];
  }

  bool IsEof(void) const {
    if(position >= fileData.size)
      return true;
    return false;
  }

  int GetSize(void) const {
    return fileData.size;
  }

  void PopBytes(char* buffer, int bytes) {
    for(int i = 0; i < bytes; ++i)
      buffer[i] = PopByte();
  }
};
#endif

} // Namespace unamed.

struct ZipPackageData {
  std::string archiveName;
  boost::shared_ptr<ZipData> zipData;

  ZipPackageData(void) {
  }

  void Load(void) {
    zipData.reset(new ZipData(archiveName));
  }
};

ZipPackage::ZipPackage(const std::string& archiveName) {
  boost::scoped_ptr<ZipPackageData> tempData(new ZipPackageData());
  tempData->archiveName = archiveName;
  tempData->Load();
  _data.swap(tempData);
}

ZipPackage::~ZipPackage(void) {

}

void ZipPackage::FindFiles(const std::string& dir, const std::string& extension, IFileList& result) {
  _data->zipData->FindFiles(dir, extension, result);
}

InputStream ZipPackage::GetFile(const std::string& filename) {
  InputStream inputStream;

  ZipFileList::iterator it;
  if(_data->zipData->FindFile(filename, it)) {
    boost::shared_ptr<ZipInputBuffer> inputBuffer(new ZipInputBuffer(_data->zipData, it->second));
    inputStream.SetBuffer(inputBuffer);
  } else {
    boost::shared_ptr<EmptyBuffer> inputBuffer(new EmptyBuffer());
    inputStream.SetBuffer(inputBuffer);
  }
  return inputStream;
}

} // Namespace filesystem.
} // Namespace saracraft.

