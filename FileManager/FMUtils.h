#pragma once

#include<vector>
#include<iostream>
#include<regex>
#include<algorithm>
#include"Entries.h"

#ifndef FM_UTILS_ODD

#define FM_UTILS_ODD

std::vector<filesystem_entry*> _search(const std::filesystem::path& p, std::string q);

std::vector<filesystem_entry*> _sortByName(std::vector<filesystem_entry*> s, bool rec);
std::vector<filesystem_entry*> _sortByDate(std::vector<filesystem_entry*> s, bool rec);
std::vector<filesystem_entry*> _sortByFiletype(std::vector<filesystem_entry*> s, bool rec);

template <typename TP>
std::time_t to_time_t(TP tp) // Конвертер std::filessystem::file_time_type в std::time_t
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}

#endif // !FM_UTILS_ODD
