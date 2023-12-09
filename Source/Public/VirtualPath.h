/**
 *
 * NFileSystem
 * Author: Rayfalling 
 * DateTime: 2023-12-10 00:49
 *
 */

#pragma once

#ifndef NFILESYSTEM_VIRTUAL_PATH_H
#define NFILESYSTEM_VIRTUAL_PATH_H

#include <string>
#include <vector>

#include "NFileSystemExport.generate.h"

namespace NFileSystem
{
	/**
	 * \brief Path Utils for file system
	 */
	class NFILESYSTEM_EXPORTS VirtualPath
	{
	public:
		VirtualPath();

		VirtualPath(std::string);



	private:
		// Is Root path
		bool IsRoot = false;

		// Is relative path
		bool IsRelative = false;

		// The origin path input
		std::string OriginPath;

		// Divided for real path 
		std::vector<std::string> PathList;
	};
} // NFileSystem

#endif //NFILESYSTEM_VIRTUAL_PATH_H
