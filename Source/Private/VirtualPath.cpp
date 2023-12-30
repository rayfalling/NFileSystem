/**
 *
 * NFileSystem
 * Author: Rayfalling 
 * DateTime: 2023-12-10 00:49
 *
 */

#include "VirtualPath.h"

#include <algorithm>
#include <sstream>

using namespace NFileSystem;

static const std::string EmptyPath = "";

bool NFileSystem::IsRelativePath(const std::string& path)
{
	return !IsAbsolutePath(path);
}

bool NFileSystem::IsAbsolutePath(const std::string& path)
{
	if (path.empty())
		return false;

	// Check Windows drive letter
	if (path.size() > 1 && path[1] == ':')
		return true;

	// Check Unix-like path
	if (path[0] == '/')
		return true;

	return false;
}

std::string NFileSystem::StandardizePath(const std::string& path)
{
	std::string result = path;

	// Replace backslash with slash
	std::replace(result.begin(), result.end(), '\\', '/');

	// Remove duplicate slashes
	const auto newEnd = std::unique(
		result.begin(), result.end(),
		[](const char a, const char b)
		{
			return a == '/' && b == '/';
		}
	);
	result.erase(newEnd, result.end());

	// Remove trailing slash
	if (result.size() > 1 && result.back() == '/')
		result.pop_back();

	return result;
}

VirtualPath::VirtualPath() : Dirty(true), Relative(true), OriginPath("/")
{
	UpdateDirty();
}

VirtualPath::VirtualPath(const char* path) : Dirty(true), Relative(true), OriginPath(path)
{
	UpdateDirty();
}

VirtualPath::VirtualPath(const std::string& path) : Dirty(true), Relative(true), OriginPath(path)
{
	UpdateDirty();
}

VirtualPath::VirtualPath(const VirtualPath& path)
{
	*this = path;
}

VirtualPath::VirtualPath(const VirtualPath&& path) noexcept
{
	*this = std::move(path);
}

VirtualPath& VirtualPath::operator=(const VirtualPath& path)
{
	if (this != &path)
	{
		Root = path.Root;
		Dirty = path.Dirty;
		Relative = path.Relative;
		PathList = path.PathList;
		OriginPath = path.OriginPath;
		NormalizedPath = path.NormalizedPath;
	}

	return *this;
}

VirtualPath& VirtualPath::operator=(const VirtualPath&& path)
{
	if (this != &path)
	{
		Root = path.Root;
		Dirty = path.Dirty;
		Relative = path.Relative;
		PathList = std::move(path.PathList);
		OriginPath = std::move(path.OriginPath);
		NormalizedPath = std::move(path.NormalizedPath);
	}

	return *this;
}

VirtualPath& VirtualPath::operator=(const char* path)
{
	Dirty = true;
	OriginPath = path;
	UpdateDirty();

	return *this;
}

VirtualPath& VirtualPath::operator=(const std::string& path)
{
	Dirty = true;
	OriginPath = path;
	UpdateDirty();

	return *this;
}

VirtualPath VirtualPath::operator+(const char* path) const
{
	VirtualPath result(OriginPath + "/" + path);
	return result;
}

VirtualPath VirtualPath::operator+(const std::string& path) const
{
	VirtualPath result(OriginPath + "/" + path);
	return result;
}

VirtualPath VirtualPath::operator+(const VirtualPath& path) const
{
	VirtualPath result(OriginPath + "/" + path.OriginPath);
	return result;
}

VirtualPath VirtualPath::operator/(const char* path) const
{
	VirtualPath result(OriginPath + "/" + path);
	return result;
}

VirtualPath VirtualPath::operator/(const std::string& path) const
{
	VirtualPath result(OriginPath + "/" + path);
	return result;
}

VirtualPath VirtualPath::operator/(const VirtualPath& path) const
{
	VirtualPath result(OriginPath + "/" + path.OriginPath);
	return result;
}

VirtualPath& VirtualPath::operator+=(const char* path)
{
	Dirty = true;
	OriginPath += "/";
	OriginPath += path;
	UpdateDirty();

	return *this;
}

VirtualPath& VirtualPath::operator+=(const std::string& path)
{
	Dirty = true;
	OriginPath += "/";
	OriginPath += path;
	UpdateDirty();

	return *this;
}

VirtualPath& VirtualPath::operator+=(const VirtualPath& path)
{
	Dirty = true;
	OriginPath += path.OriginPath;
	UpdateDirty();

	return *this;
}

VirtualPath& VirtualPath::operator/=(const char* path)
{
	Dirty = true;
	OriginPath += "/";
	OriginPath += path;
	UpdateDirty();

	return *this;
}

VirtualPath& VirtualPath::operator/=(const std::string& path)
{
	Dirty = true;
	OriginPath += "/";
	OriginPath += path;
	UpdateDirty();

	return *this;
}

VirtualPath& VirtualPath::operator/=(const VirtualPath& path)
{
	Dirty = true;
	OriginPath += path.OriginPath;
	UpdateDirty();

	return *this;
}

bool VirtualPath::IsRelative() const
{
	return Relative;
}

const std::string& VirtualPath::GetOriginPath() const
{
	return OriginPath;
}

const std::string& VirtualPath::GetNormalizedPath() const
{
	return NormalizedPath;
}

const std::string& VirtualPath::GetPathDepth(const int depth) const
{
	if (depth < 0 || depth >= PathList.size())
		return EmptyPath;

	return PathList[depth];
}

std::string VirtualPath::GetCommonPath(const VirtualPath& path) const
{
	// If the path is relative, return empty path
	if (path.Relative || Relative)
		return EmptyPath;

	// you can only find the common path in absolute mode
	std::stringstream result("/");
	const size_t minDepth = std::min(PathList.size(), path.PathList.size());
	for (int i = 0; i < minDepth; ++i)
	{
		if (PathList[i] != path.PathList[i])
		{
			return result.str();
		}

		result << PathList[i];
	}

	return EmptyPath;
}

bool VirtualPath::IsRootPath() const
{
	// Unix and Unix-like file system
	if (NormalizedPath == "/")
		return true;

	// Windows file system
	if (NormalizedPath.size() == 2 && NormalizedPath[1] == ':')
		return true;

	return false;
}

void VirtualPath::UpdateDirty()
{
	if (!Dirty)
		return;

	const auto standardizePath = StandardizePath(OriginPath);

	Relative = IsRelativePath(standardizePath);
	SplitPathToList(standardizePath);
	MergePathFromList();
	NormalizedPath = standardizePath;
	Root = IsRootPath();

	Dirty = false;
}

void VirtualPath::MergePathFromList()
{
	for (const auto& path : PathList)
	{
		NormalizedPath += NormalizedPath.empty() && !Relative ? "/" : "";
		NormalizedPath += path;
	}
}

void VirtualPath::SplitPathToList(const std::string& standardizedPath)
{
	// Split path
	std::string::size_type pos;
	std::string::size_type prev = 0;

	while ((pos = standardizedPath.find('/', prev)) != std::string::npos)
	{
		// ReSharper disable once CppTooWideScopeInitStatement
		const auto chunk = standardizedPath.substr(prev, pos - prev);

		// back to parent directory
		if (chunk == "..")
		{
			// If the path is relative and parent is not empty
			// just push back
			if (!PathList.empty())
			{
				PathList.pop_back();
			}
			// else push back for make absolute path later
			else if (Relative)
			{
				PathList.push_back(chunk);
			}
		}
		// Ignore current directory
		else if (chunk != "." && !chunk.empty())
		{
			PathList.push_back(chunk);
		}

		prev = pos + 1;
	}

	PathList.push_back(standardizedPath.substr(prev));
}

VirtualPath NFileSystem::operator+(const char* left, const VirtualPath& right)
{
	return std::move(VirtualPath(left) + right);
}

VirtualPath NFileSystem::operator+(const std::string& left, const VirtualPath& right)
{
	return std::move(VirtualPath(left) + right);
}

VirtualPath NFileSystem::operator/(const char* left, const VirtualPath& right)
{
	return std::move(VirtualPath(left) / right);
}

VirtualPath NFileSystem::operator/(const std::string& left, const VirtualPath& right)
{
	return std::move(VirtualPath(left) / right);
}
