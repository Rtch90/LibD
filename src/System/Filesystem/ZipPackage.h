#include <boost/scoped_ptr.hpp>
#include "IFilePackage.h"

namespace saracraft {
namespace filesystem {

class IFilePackage;
struct ZipPackageData;

class ZipPackage : public IFilePackage {
  boost::scoped_ptr<ZipPackageData> _data;
public:
  ZipPackage(const std::string& archiveName);
  ~ZipPackage(void);

  void FindFiles(const std::string& dir, const std::string& extension, IFileList& result);
  InputStream GetFile(const std::string& filename);
};

} // Namespace saracraft.
} // Namespace filesystme.

