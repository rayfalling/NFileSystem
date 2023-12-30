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
	/// \brief Check if the path is relative
	/// \param path path
	/// \return bool
	[[nodiscard]] bool IsRelativePath(const std::string& path);

	/// \brief Check if the path is absolute
	/// \param path path
	/// \return bool
	[[nodiscard]] bool IsAbsolutePath(const std::string& path);

	/// \brief Standardize the given path
	/// \param path path
	/// \return standardized path
	[[nodiscard]] std::string StandardizePath(const std::string& path);

	/**
	 * \brief Path Utils for file system
	 */
	class NFILESYSTEM_EXPORTS VirtualPath
	{
	public:
		VirtualPath();

		VirtualPath(const char* path);
		VirtualPath(const std::string& path);

		// Copy and Move Constructor
		VirtualPath(const VirtualPath& path);
		VirtualPath(const VirtualPath&& path) noexcept;

		// Copy and Move Assignment
		VirtualPath& operator=(const VirtualPath& path);
		VirtualPath& operator=(const VirtualPath&& path);

		// String Assignment
		VirtualPath& operator=(const char* path);
		VirtualPath& operator=(const std::string& path);

		// String Concatenation
		VirtualPath operator+(const char* path) const;
		VirtualPath operator+(const std::string& path) const;
		VirtualPath operator+(const VirtualPath& path) const;

		VirtualPath operator/(const char* path) const;
		VirtualPath operator/(const std::string& path) const;
		VirtualPath operator/(const VirtualPath& path) const;

		VirtualPath& operator+=(const char* path);
		VirtualPath& operator+=(const std::string& path);
		VirtualPath& operator+=(const VirtualPath& path);

		VirtualPath& operator/=(const char* path);
		VirtualPath& operator/=(const std::string& path);
		VirtualPath& operator/=(const VirtualPath& path);

		friend VirtualPath operator+(const char* left, const VirtualPath& right);
		friend VirtualPath operator+(const std::string& left, const VirtualPath& right);

		friend VirtualPath operator/(const char* left, const VirtualPath& right);
		friend VirtualPath operator/(const std::string& left, const VirtualPath& right);

	protected:
		/// \brief Is the path root
		/// \return bool
		bool IsRootPath() const;

		/// \brief Update the path property
		void UpdateDirty();

		/// \brief Merge normalized path from list
		void MergePathFromList();

		/// \brief Split standardized path to list
		/// \param standardizedPath standardized path
		void SplitPathToList(const std::string& standardizedPath);

	public:
		/// \brief is the path relative
		/// \return bool
		[[nodiscard]] bool IsRelative() const;

		/// \brief get origin path
		/// \return origin path
		[[nodiscard]] const std::string& GetOriginPath() const;

		/// \brief get normalized path
		/// \return normalized path
		[[nodiscard]] const std::string& GetNormalizedPath() const;

		/// \brief Get the path at specific depth
		/// \param depth Depth of directory
		/// \return string "" if out of depth range
		[[nodiscard]] const std::string& GetPathDepth(int depth) const;

		/// \brief get common path
		/// \return common path
		[[nodiscard]] std::string GetCommonPath(const VirtualPath& path) const;

	private:
		// ----- Flags -----
		bool Dirty = false;

		// ----- Property -----

		// Is Root path
		bool Root = false;

		// Is relative path
		bool Relative = false;

		// The origin path input
		std::string OriginPath;

		// The normalized path store
		std::string NormalizedPath;

		// Divided for real path 
		std::vector<std::string> PathList;
	};
} // NFileSystem

#endif //NFILESYSTEM_VIRTUAL_PATH_H
