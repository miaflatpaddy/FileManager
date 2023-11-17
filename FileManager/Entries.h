#pragma once

#include <string>
#include <filesystem>
#include <list>
#include "Utils.h"

#ifndef FME

#define FME

class filesystem_entry {
protected:
	std::filesystem::perms permissions; // ���������� ����� 
	std::string permissionsString; // ������ ��������� UNIX-Like ("-rw-r--r--")

	uintmax_t size;
	std::string name; // ��� ������
	bool is_directory; // ....
	std::filesystem::path path; // ��������������� �����/�����
	
public:
	filesystem_entry(const std::filesystem::path& p, std::string name); // ��� �������� ����� "..."
	filesystem_entry(const std::filesystem::path& p); // ��� �������� ���������� � �������� ����� �������� ��������
	
	std::string getName() { return this->name;  };
	std::filesystem::path getPath() { return this->path;  };
	std::string getPermissions() { return this->permissionsString; };
	
	bool isFolder();

	virtual bool remove() = 0; // ��� ����� � ����� ������������ ������ 
	void rename(const std::string& p) { std::filesystem::rename(this->getPath(), this->getPath().replace_filename(p)); };
	uintmax_t getSize();
};

class file : public filesystem_entry { // ����
public:
	file(const std::filesystem::path& p) : filesystem_entry(p) {};

	bool remove() override { return std::filesystem::remove(this->path); };
};

class folder : public filesystem_entry { // �����
public:
	folder(const std::filesystem::path& p) : filesystem_entry(p) {};
	folder(const std::filesystem::path& p, const std::string& name) : filesystem_entry(p, name) {};

	bool remove() override { return std::filesystem::remove_all(this->path); }; // ����������� ��������
};

class filesystem_entry_fabric { // ������� ��� ����������
public:
	filesystem_entry_fabric() {};

	filesystem_entry* entryFolder(const std::filesystem::path& p,std::string name);
	filesystem_entry* entryFolder(const std::filesystem::path& p);
	filesystem_entry* entryFile(const std::filesystem::path& p);

	filesystem_entry* entry(const std::filesystem::path& p);
};

#endif