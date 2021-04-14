#pragma once
#include "stdafx.h"
#include <string>
#include "ZipTool.h"
#include <direct.h>
#include "ZLibWrap.h"
#ifdef _DEBUG
#pragma comment(lib, "ZlibWraplibd.lib")
#else
#pragma comment(lib, "ZlibWraplib.lib")
#endif


bool ZipTool::extractDir(std::string fileCompressed, std::string dir)
{
	return ZipExtract(fileCompressed.c_str(), dir.c_str()) == TRUE;
}

bool ZipTool::compressDir(std::string fileCompressed, std::string dir, bool deleteDir)
{
	return ZipCompress(dir.c_str(), fileCompressed.c_str(), true) == TRUE;
}

void ZipTool::deleteFolder(const std::string &folderFullPath)
{
	if (folderFullPath.length()==0)
	{
		RemoveDirectory(folderFullPath.c_str());
		return;
	}
	CFileFind   ff;
	std::string tmpPath = folderFullPath + _T("\\*");
	BOOL bFound = ff.FindFile(tmpPath.c_str(), 0);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
		{
			continue;
		}
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			deleteFolder(ff.GetFilePath().GetString());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());
		}

	}
	ff.Close();
	RemoveDirectory(folderFullPath.c_str());
}


std::string ZipTool::FilePathToFolderPath(const std::string &filePath)
{
	return filePath.c_str();
}


ZipTool::ZipTool()
{

}
