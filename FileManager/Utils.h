#pragma once

#ifndef FMUTILS

#define FMUTILS

namespace fs = std::filesystem;

std::string permsToString(const std::filesystem::perms& p); // �� ����� � ������

long double getFolderSize(const fs::path& p); // �������� ������ ����� (����������� ��������)

void SetConsoleColor(int k); // ���������� ���� ������ � �������
void openFile(const std::filesystem::path& p); // ������� ����
#endif