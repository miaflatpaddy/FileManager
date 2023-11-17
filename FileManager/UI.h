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
	bool ns = false; // ������� ���������� �� �����
	bool ds = false; // ������� ���������� �� ����
	bool fs = false; // ������� ���������� �� ���� �����

	bool _confirm(const std::string& m); // ������� ��� ����� ���������� ������������ ����������� �������� 
	std::string _userInput(const std::string& m);// ������� ��� ����� ���������� ������������ ������ �����-�� ������
	uint8_t cursorPos; // ���������� ����


protected:
	uint16_t page; // ������� ��������
	uint16_t pageAmount; // ����� ���������� �������

public:

	UI();
	//~UI();

	// ��������� ����������
	void draw(); 

	// ����������� ����� �� ������
	void navigateUp();

	// ����������� ���� �� ������
	void navigateDown(); 

	// ��
	void Copy();
	void Cut();
	void Paste();
	void Open();

	// ���� ��
	void rename();
	void remove();

	// � ��� ��
	void CreateFolder();
	void CreateFile();

	// ...
	void SortByName();
	void SortByDate();
	void SortByFT(); // �� ���� �����

	void goDirect(); // ������� �� ����
	void PrintPath(); // ���������� ������� ����

	// No comments
	void Search(); 
};

#endif // !FMUI_CLS

