#include "UI.h"
#include"Utils.h"
#include"FMUtils.h"

#include <chrono>
#include <ctime>
#include <time.h>
#include <thread>

#pragma comment(lib, "shell32.lib")

#define DEBUG

bool UI::_confirm(const std::string& m)
{
	system("CLS");

	std::cout << m << std::endl;
	char ans;

	std::cin >> ans;
	if (tolower(ans) == 'y')
		return true;
	else if (tolower(ans) == 'n')
		return false;
	else
		return this->_confirm(m);

}

std::string UI::_userInput(const std::string& m)
{
	system("CLS");

	std::cout << m << std::endl;
	std::string ans;

	std::cin >> ans;

	return ans;
}

UI::UI() : page(0), fm(File_Manager()), cursorPos(0)
{
	size_t dirSize = fm.getCurrentFolderContent().size();
	int odd = dirSize % this->pageLimit;
	this->pageAmount = ((dirSize - odd) / pageLimit) + int(odd > 0);
	this->draw();
}

void UI::draw()
{	
	SetConsoleColor(7);
	system("CLS");


	auto tmp = fm.getCurrentFolderContent();

	std::cout << "Last edit  Permissions  Size          File name" << std::endl;

	auto tmplimit = (this->page * this->pageLimit) + this->pageLimit;
	auto limit = (tmplimit <= tmp.size()) ? tmplimit : tmp.size();


	for (uint16_t i = (this->page * this->pageLimit); i < limit; i++) {

		if (i == this->cursorPos && tmp[i]->isFolder())
			SetConsoleColor(19);
		else if (i == this->cursorPos)
			SetConsoleColor(23);
		else if (tmp[i]->isFolder())
			SetConsoleColor(11);
		else if (tmp[i]->isFolder() && tmp[i]->getPath() == this->fm.BufferedPath())
			SetConsoleColor(13);
		else if (tmp[i]->getPath() == this->fm.BufferedPath())
			SetConsoleColor(12);
		else
			SetConsoleColor(7);

		try {
			std::string sc = "B";

			auto s = tmp[i]->getSize();

			if (s > 1024) {
				s /= 1024;
				sc = "KB";
			}

			if (s > 1024) {
				s /= 1024;
				sc = "MB";
			}

			if (s > 1024) {
				s /= 1024;
				sc = ">GB";
			}


			auto tt = to_time_t(std::filesystem::last_write_time(tmp[i]->getPath()));
			tm ss;
			localtime_s(&ss, &tt);
			std::cout << ss.tm_year + 1900 << ":" << ss.tm_mon << ":" << ss.tm_mday << "  " << tmp[i]->getPermissions() << "   " << s << sc << "    " << tmp[i]->getName() << std::endl;
			SetConsoleColor(7);
		}
		catch (std::filesystem::filesystem_error& e) {
			std::cout << "----------  " << tmp[i]->getPermissions() << "    " << tmp[i]->getName() << std::endl;
		}
		
		
		
	}
	SetConsoleColor(7);

	std::cout << 
"\n\nD - Navigate down \t U - Navigate up \t O - Open File \n\
X - Cut \t C - Copy \t P - Paste \n\
M - Raname \t R - Romove \t S - Search \n\
Sorting options (press again to togle order):\n\
1 - Sort by name \t 2 - Sort by Date \t \n\
3 - Sort by file type\n Y - Show current path\n\
G - Create folder \t F - Create file \n\
T - Go to... (like cd) \t K - Exit Application";

	if (fm.BufferedPath().empty() && fm.BufferedPath() != "")
		std::cout << "File in buffer: " << fm.BufferedPath() << std::endl;

#ifdef DEBUG

	std::cout << "Page: " << this->page << " Cursor: " << int(this->cursorPos) << std::endl;

#endif // DEBUG
}

void UI::navigateUp()
{
	uint16_t top = (this->page * pageLimit);
	uint16_t bot = (this->page * pageLimit) + pageLimit;

	if (this->cursorPos < 1)
		this->cursorPos = 0;
	else if (this->cursorPos <= top) {
		this->page--;
		this->cursorPos--;
	}
	else if (this->cursorPos > bot) {
		this->page++;
		this->cursorPos--;
	} else
		this->cursorPos--;

	this->draw();

#ifdef DEBUG

	std::cout << "Top: " << top << " Bot: " << bot << std::endl;

#endif // DEBUG
}

void UI::navigateDown()
{
	uint16_t top = (this->page * pageLimit);
	uint16_t bot = (this->page * pageLimit) + pageLimit - 1;

	if (this->cursorPos >= this->fm.getCurrentFolderContent().size() - 1)
		this->cursorPos = this->fm.getCurrentFolderContent().size() - 1;
	else if (this->cursorPos < top) {
		this->page--;
		this->cursorPos++;
	}
	else if (this->cursorPos >= bot) {
		this->page++;
		this->cursorPos++;
	}
	else
		this->cursorPos++;

	this->draw();

#ifdef DEBUG

	std::cout << "Top: " << top << " Bot: " << bot << std::endl;

#endif // DEBUG
}

void UI::Copy()
{
	this->fm.copy(this->fm.getCurrentFolderContent()[this->cursorPos]->getPath());
	this->draw();
}

void UI::Cut()
{
	this->fm.cut(this->fm.getCurrentFolderContent()[this->cursorPos]->getPath());
	this->draw();
}

void UI::Paste()
{
	this->fm.paste();
	this->fm.getCurrentFolderContent();
	this->draw();
}

void UI::Open()
{
	try {
		auto tmp = this->fm.getCurrentFolderContent()[this->cursorPos];
		auto a = tmp->getPath();
		auto b = tmp->getName();

		if (tmp->isFolder()) {
			this->fm.goTo(a);

			if (b == "...")
				this->fm.getCurrentFolderContent_f();
			else
				this->fm.getCurrentFolderContent();


			this->cursorPos = 0;
			this->page = 0;

			this->draw();
		}
		else {
			openFile(a);
			this->draw();
		}

		
	}
	catch (const char* e){
		std::cout << e << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(2000));
	}
	
}

void UI::rename()
{
	auto tmp = this->fm.getCurrentFolderContent()[this->cursorPos];
	
	std::string ans;
	uint8_t aa = 0;
	do {
		if (aa == 0) {
			ans = this->_userInput("Choose new name:");
			aa++;
		} else
			ans = this->_userInput("Name you've choosen already exist. Please choose new name:");
	} while (std::filesystem::directory_entry(this->fm.CurrentPath() / ans).exists());

	std::filesystem::rename(tmp->getPath(), this->fm.CurrentPath() / ans);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	this->fm.getCurrentFolderContent();
	this->draw();
}

void UI::remove()
{
	auto tmp = this->fm.getCurrentFolderContent()[this->cursorPos];
	if (this->_confirm(std::string("Are you sure you want to delete ") + tmp->getName() + "? [y/n]"))
		tmp->remove();

	this->fm.getCurrentFolderContent_f();
	this->draw();
}

void UI::SortByName()
{
	this->ns ^= true;
	this->fm.sortByName(this->ns);
	this->draw();
}

void UI::SortByDate()
{
	this->ns ^= true;
	this->fm.sortByDate(this->ns);
	this->draw();
}
void UI::SortByFT()
{
	this->ns ^= true;
	this->fm.sortByFiletype(this->ns);
	this->draw();
}

void UI::CreateFolder()
{
	auto ans = _userInput("Enter folder name:");
	this->fm.createFolder(ans);
	this->fm.getCurrentFolderContent_f();
	this->draw();
}

void UI::CreateFile() {
	auto ans = _userInput("Enter file name and extension:");
	this->fm.createFile(ans);
	this->fm.getCurrentFolderContent_f();
	this->draw();
}

void UI::Search()
{
	this->cursorPos = 0;
	auto ans = this->_userInput("Enter pattern or word that you are looking for:");
	this->fm.search(ans);
	this->draw();
}

void UI::goDirect()
{
	this->cursorPos = 0;
	auto ans = _userInput("Enter folder that you want :");
	this->fm.goTo(ans);
	this->draw();
}

void UI::PrintPath()
{
	this->cursorPos = 0;
	_userInput(this->fm.CurrentPath().string());
	this->draw();
}