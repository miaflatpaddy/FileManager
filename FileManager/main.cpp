#include <string>
#include <iostream>
#include <filesystem>
#include <regex>
#include <thread>

#include"Utils.h"
#include"FileManager.h"
#include"FMUtils.h"
#include"UI.h"

int main()
{
    UI fm;

	while (true)
	{
		char c = tolower(getchar());
		if (c == 'd')
			fm.navigateDown();
		else if (c == 'u')
			fm.navigateUp();
		else if (c == 'c')
			fm.Copy();
		else if (c == 'x')
			fm.Cut();
		else if (c == 'p')
			fm.Paste();
		else if (c == 'o')
			fm.Open();
		else if (c == 'm')
			fm.rename();
		else if (c == 'r')
			fm.remove();
		else if (c == 's')
			fm.Search();
		else if (c == '1')
			fm.SortByName();
		else if (c == '2')
			fm.SortByDate();
		else if (c == '3')
			fm.SortByFT();
		else if (c == 'g')
			fm.CreateFolder();
		else if (c == 'f')
			fm.CreateFile();
		else if (c == 't')
			fm.goDirect();
		else if (c == 'y')
			fm.PrintPath();
		else if (c == 'k')
			exit(0);
		else
			fm.draw();
	}

}


