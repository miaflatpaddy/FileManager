#pragma once

#include <vector>
#include <filesystem>
#include "Entries.h"
#include "FMUtils.h"

#ifndef FMCLS

#define FMCLS

class File_Manager
{
private:
	std::filesystem::path currentPath; // Текущее месторасположение
	std::filesystem::path cachedPath; // Предыдущее месторасположение

	// Содержание текущей папки
	std::vector<filesystem_entry*> currentFolderContent; 
	
	
	std::filesystem::path ccBuffer; // Буфер для копирования/перемещения
	int state; // Состояние для буфера

public:
	
	File_Manager();

	std::filesystem::path CurrentPath() { return this->currentPath; };
	
	// Всегда заново получает содержимаое папки
	std::vector<filesystem_entry*> getCurrentFolderContent_f(); 

	// Возвращает содержимое текущей папки, если путь не изменился вернет кешированное
	std::vector<filesystem_entry*> getCurrentFolderContent(); 


	void goTo(const std::filesystem::path& p); // Сменить путь
	bool createFolder(const std::string&); // Содать папку
	void createFile(const std::string&); // Создать фал

	std::filesystem::path BufferedPath() { return this->ccBuffer;  } 

	std::vector<filesystem_entry*> sortByName(bool rec);
	std::vector<filesystem_entry*> sortByDate(bool rec);
	std::vector<filesystem_entry*> sortByFiletype(bool rec);

	void search(const std::string& q);

	void copy(const std::filesystem::path&);
	void cut(const std::filesystem::path&);
	bool paste();
};

#endif // !FILE_MANAGER_CLS


