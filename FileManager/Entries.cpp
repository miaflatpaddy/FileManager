#include "Entries.h"
#include "Utils.h"
#include <exception>
#include <iostream>

filesystem_entry::filesystem_entry(const std::filesystem::path& p) : path(p) {
	auto tmp = std::filesystem::directory_entry(this->path);
	//if (!tmp.exists()) {
	//	std::cout << tmp.path().string();
	//	throw std::runtime_error("Invalid path or path to deleted object");
	//}
	this->is_directory = false;



	this->name = path.filename().string();
	try {
		this->permissions = tmp.status().permissions();
		this->is_directory = std::filesystem::directory_entry(this->path).is_directory();
		this->permissionsString = permsToString(this->permissions);
	}
	catch (std::filesystem::filesystem_error& e) {
		this->is_directory = false;
		this->permissions = std::filesystem::perms();
		this->permissionsString = "Error";
	}

	if (this->isFolder())
		this->size = getFolderSize(this->getPath());
	else
		try {
			this->size = std::filesystem::file_size(this->getPath());
		}
		catch (std::filesystem::filesystem_error&) {
			this->size = 0;
		}
}

filesystem_entry::filesystem_entry(const std::filesystem::path& p, std::string name) : path(p), name(name) {
	auto tmp = std::filesystem::directory_entry(this->path);
	if (!tmp.exists()) throw std::runtime_error("Invalid path or path to deleted object");


	this->is_directory = false;
	this->permissions = tmp.status().permissions();
	this->is_directory = std::filesystem::directory_entry(this->path).is_directory();
	this->permissionsString = permsToString(this->permissions);
	
	this->size = 0;
}

bool filesystem_entry::isFolder()
{
	try
	{
		if (std::filesystem::directory_entry(this->getPath()).is_directory())
			return true;
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		return false;
	}
	
	return false;

}

filesystem_entry* filesystem_entry_fabric::entryFolder(const std::filesystem::path& p)
{
	auto* f = new folder(p);
	return f;
}

filesystem_entry* filesystem_entry_fabric::entryFolder(const std::filesystem::path& p, std::string name)
{
	auto* f = new folder(p, name);
	return f;
}

filesystem_entry* filesystem_entry_fabric::entryFile(const std::filesystem::path& p)
{
	auto* f = new file(p);
	return f;
}

filesystem_entry* filesystem_entry_fabric::entry(const std::filesystem::path& p)
{
	if (std::filesystem::directory_entry(p).is_directory())
		return entryFolder(p);
	else
		return entryFile(p);
}

uintmax_t filesystem_entry::getSize() { return this->size;  }