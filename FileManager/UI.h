#pragma once

#include<vector>
#include<filesystem>
#include<string>
#include<iostream>
#include"Entries.h"
#include"FileManager.h"

#ifndef FMUI_CLS

#define FMUI_CLS

class UI
{
private:
	const size_t pageLimit = 10;

	File_Manager fm;
	bool ns = false; // Порядок сортировки по имени
	bool ds = false; // Порядок сортировки по дате
	bool fs = false; // Порядок сортировки по типу файла

	bool _confirm(const std::string& m); // Функция что будет спрашивать пользователя подтвердить действие 
	std::string _userInput(const std::string& m);// Функция что будет спрашивать пользователя ввести какую-то строку
	uint8_t cursorPos; // Выделенные файл


protected:
	uint16_t page; // Текущая страница
	uint16_t pageAmount; // Общее количество страниц

public:

	UI();
	//~UI();

	// Отрисовка интерфейса
	void draw(); 

	// Перемещение вверх по списку
	void navigateUp();

	// Перемещение вниз по списку
	void navigateDown(); 

	// ДА
	void Copy();
	void Cut();
	void Paste();
	void Open();

	// Тоже да
	void rename();
	void remove();

	// И это да
	void CreateFolder();
	void CreateFile();

	// ...
	void SortByName();
	void SortByDate();
	void SortByFT(); // По типу файла

	void goDirect(); // Перейти по пути
	void PrintPath(); // Напечатать текущий путь

	// No comments
	void Search(); 
};

#endif // !FMUI_CLS

