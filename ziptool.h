#ifndef ZIPTOOL_H
#define ZIPTOOL_H
#include <string>
class ZipTool
{
public:
	ZipTool();
	// 解压文件到文件夹
	static bool extractDir(std::string fileCompressed, std::string dir);
	// 压缩目录
	static bool compressDir(std::string fileCompressed,
		std::string dir, bool deleteDir = false);
	// 删除目录
	static void deleteFolder(const std::string& folderFullPath);
	// 将文件名按照一定规则转换成文件夹名
	std::string FilePathToFolderPath(const std::string& filePath);
};

#endif // ZIPTOOL_H
