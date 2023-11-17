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
	std::filesystem::path currentPath; // ������� �����������������
	std::filesystem::path cachedPath; // ���������� �����������������

	// ���������� ������� �����
	std::vector<filesystem_entry*> currentFolderContent; 
	
	
	std::filesystem::path ccBuffer; // ����� ��� �����������/�����������
	int state; // ��������� ��� ������

public:
	
	File_Manager();

	std::filesystem::path CurrentPath() { return this->currentPath; };
	
	// ������ ������ �������� ����������� �����
	std::vector<filesystem_entry*> getCurrentFolderContent_f(); 

	// ���������� ���������� ������� �����, ���� ���� �� ��������� ������ ������������
	std::vector<filesystem_entry*> getCurrentFolderContent(); 


	void goTo(const std::filesystem::path& p); // ������� ����
	bool createFolder(const std::string&); // ������ �����
	void createFile(const std::string&); // ������� ���

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


