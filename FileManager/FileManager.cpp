#include "FileManager.h"
#include "FMUtils.h"

#include<fstream>
#include<filesystem>

File_Manager::File_Manager() {
	this->currentPath = std::filesystem::current_path();
	this->getCurrentFolderContent();
}

std::vector<filesystem_entry*> File_Manager::getCurrentFolderContent() {
	auto& res = this->currentFolderContent;
	
	if (this->cachedPath == this->CurrentPath())
	return res;	

	this->cachedPath = this->CurrentPath();

	if (!res.empty()) {
		for (auto& i : res)
			delete i;

		res.clear();
	}
	
	auto& path = this->currentPath;
	auto fb = filesystem_entry_fabric();

	if (currentPath.string().size() >= 4)
		res.push_back(fb.entryFolder(this->currentPath.parent_path(), "..."));

	for (const auto& entry : fs::directory_iterator(path))
		if (entry.is_directory()) 
			res.push_back(fb.entryFolder(entry.path()));
		else
			res.push_back(fb.entryFile(entry.path()));

	return this->currentFolderContent;
}

std::vector<filesystem_entry*> File_Manager::getCurrentFolderContent_f() {
	auto& res = this->currentFolderContent;

	this->cachedPath = this->CurrentPath();

	for (auto& i : res)
		delete i;

	res.clear();
	

	auto& path = this->currentPath;
	auto fb = filesystem_entry_fabric();

	res.push_back(fb.entryFolder(this->currentPath.parent_path(), std::string("...")));

	for (const auto& entry : fs::directory_iterator(path))
		if (entry.is_directory())
			res.push_back(fb.entryFolder(entry.path()));
		else
			res.push_back(fb.entryFile(entry.path()));

	return this->currentFolderContent;
}

void File_Manager::goTo(const std::filesystem::path& p) {
	std::filesystem::path tmp;
	
	if (p.is_absolute())
		tmp = p;
	else
		tmp = this->currentPath / p;

	if (!std::filesystem::directory_entry(tmp).exists())
		throw std::runtime_error("Path doesn't exist");

	this->currentPath = tmp;
	this->getCurrentFolderContent();
}

bool File_Manager::createFolder(const std::string& n) {
	return std::filesystem::create_directory(this->currentPath / n);
}

void File_Manager::createFile(const std::string& n) {
	std::ofstream f((this->currentPath / n).string());
	f.close();
}

void File_Manager::copy(const std::filesystem::path& n) {
	this->state = 1;
	this->ccBuffer = n;
}

void File_Manager::cut(const std::filesystem::path& n) {
	this->state = 2;
	this->ccBuffer = n;
}

bool File_Manager::paste() {
	if (this->ccBuffer.empty() && this->state != 0)
		return false;
	
	if (this->state == 1) {
		int i = 0;

		auto newname = this->currentPath / this->ccBuffer.filename();

		while (this->ccBuffer == newname
			|| std::filesystem::directory_entry(newname).exists())
		{
			auto ext = newname.extension().string();
			auto fname = newname.string();
			if (ext != "") {
				newname = fname.substr(0, fname.size() - ext.size() - ((i == 0) ? 0 : std::to_string(i).size() + 2)) + "(" + std::to_string(i) + ")" + ext;
			}
			else
				newname = fname + "(" + std::to_string(i) + ")" + ext;

			i++;
		}

		std::filesystem::copy(this->ccBuffer, newname);
	}

	if (this->state == 2) {
		int i = 0;

		auto newname = this->currentPath / this->ccBuffer.filename();

		if (!std::filesystem::directory_entry(newname).exists()) {
			std::filesystem::rename(this->ccBuffer, newname);
			return true;
		}

		system("CLS");
		std::cout << "Do you want replace file(y) or keep both(n)?" << std::endl;
		char ans;
		std::cin >> ans;

		if (ans == 'n')
			while (std::filesystem::directory_entry(newname).exists())
			{
				auto ext = newname.extension().string();
				auto fname = newname.string();
				if (ext != "") {
					newname = fname.substr(0, fname.size() - ext.size() - ((i == 0) ? 0 : std::to_string(i).size() + 2)) + "(" + std::to_string(i) + ")" + ext;
				}
				else
					newname = fname + "(" + std::to_string(i) + ")" + ext;

				i++;
			}
		else if (ans == 'y') {
			std::filesystem::remove(newname);
		}

		std::filesystem::rename(this->ccBuffer, newname);

		this->ccBuffer.clear();
	}

	this->getCurrentFolderContent_f();

	return true;
}


std::vector<filesystem_entry*> File_Manager::sortByDate(bool rec) {
	this->currentFolderContent = _sortByDate(this->currentFolderContent, rec);
	return this->currentFolderContent;
}

std::vector<filesystem_entry*> File_Manager::sortByName(bool rec) {
	this->currentFolderContent = _sortByName(this->currentFolderContent, rec);
	return this->currentFolderContent;
}

std::vector<filesystem_entry*> File_Manager::sortByFiletype(bool rec) {
	this->currentFolderContent = _sortByFiletype(this->currentFolderContent, rec);
	return this->currentFolderContent;
}

void File_Manager::search(const std::string& q)
{
	this->currentFolderContent = _search(this->CurrentPath(), q);
}