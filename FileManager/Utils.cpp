#include<string>
#include<sstream>
#include<filesystem>
#include<regex>
#include<iostream>
#include <windows.h>
#include"Utils.h"

#include "Windows.h"
#include <shellapi.h>

namespace fs = std::filesystem;

std::string permsToString(const std::filesystem::perms& p)
{
    std::stringstream tmp;

    tmp << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
        << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
        << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
        << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
        << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
        << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
        << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
        << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
        << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");

    return tmp.str();
}

long double getFolderSize(const fs::path& p) 
{
    long double size = 0;
    try {
        for (fs::recursive_directory_iterator it(p);
            it != fs::recursive_directory_iterator();
            ++it)
        {
            if (it.depth() > 10) {
                it.disable_recursion_pending();
                it.pop();
            }
            if (size > 1073741824)
                return 1073750000;
            if (!fs::is_directory(*it))
                size += fs::file_size(*it);

            if (it.depth() > 10)
                return size;
        }
    }
    catch (std::filesystem::filesystem_error&) {
        return 0;
    }
    

    return size;
}

void SetConsoleColor(int k) {
    HANDLE  hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, k);
}

void openFile(const std::filesystem::path& p) {
    LPCWSTR path = p.c_str();
    ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOW);
}