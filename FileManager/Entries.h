#pragma once

#include <string>
#include <filesystem>
#include <list>
#include "Utils.h"

#ifndef FME

#define FME

class filesystem_entry {
protected:
	std::filesystem::perms permissions; // Привилегии файла 
	std::string permissionsString; // Строка привилеги UNIX-Like ("-rw-r--r--")

	uintmax_t size;
	std::string name; // Имя записи
	bool is_directory; // ....
	std::filesystem::path path; // Местонахождение файла/папки
	
public:
	filesystem_entry(const std::filesystem::path& p, std::string name); // Для создания папки "..."
	filesystem_entry(const std::filesystem::path& p); // Для создания примитивов с которыми будет работать менеджер
	
	std::string getName() { return this->name;  };
	std::filesystem::path getPath() { return this->path;  };
	std::string getPermissions() { return this->permissionsString; };
	
	bool isFolder();

	virtual bool remove() = 0; // Для папки и файла используются разные 
	void rename(const std::string& p) { std::filesystem::rename(this->getPath(), this->getPath().replace_filename(p)); };
	uintmax_t getSize();
};

class file : public filesystem_entry { // Файл
public:
	file(const std::filesystem::path& p) : filesystem_entry(p) {};

	bool remove() override { return std::filesystem::remove(this->path); };
};

class folder : public filesystem_entry { // Папка
public:
	folder(const std::filesystem::path& p) : filesystem_entry(p) {};
	folder(const std::filesystem::path& p, const std::string& name) : filesystem_entry(p, name) {};

	bool remove() override { return std::filesystem::remove_all(this->path); }; // Рекурсивное удаление
};

class filesystem_entry_fabric { // Фабрика для примитивов
public:
	filesystem_entry_fabric() {};

	filesystem_entry* entryFolder(const std::filesystem::path& p,std::string name);
	filesystem_entry* entryFolder(const std::filesystem::path& p);
	filesystem_entry* entryFile(const std::filesystem::path& p);

	filesystem_entry* entry(const std::filesystem::path& p);
};

#endif