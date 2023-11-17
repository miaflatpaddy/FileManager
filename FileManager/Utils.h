#pragma once

#ifndef FMUTILS

#define FMUTILS

namespace fs = std::filesystem;

std::string permsToString(const std::filesystem::perms& p); // Из чисел в букавы

long double getFolderSize(const fs::path& p); // Получает размер папки (рекурсивный алгоритм)

void SetConsoleColor(int k); // Установить цвет текста в консоли
void openFile(const std::filesystem::path& p); // Открыть файл
#endif