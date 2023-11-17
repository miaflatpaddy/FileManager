#include"FMUtils.h"

#include <chrono>
#include <ctime>
#include <time.h>

std::vector<filesystem_entry*> _search(const std::filesystem::path& p, std::string q)
{
    std::cout << q << std::endl;

    std::string tmp = "(.*";
    for (auto i : q) {
        if (i == '*')
            tmp += ".*";
        else if (i == '.')
            tmp += "[.]";
        else if (isalnum(i))
            tmp.push_back(i);
        else {
            tmp += std::string("[" + i) + "]";
        }
    }
    tmp.append(".*)");

    std::vector<filesystem_entry*> res;

    //std::cout << tmp << std::endl;

    try {
        std::regex querry(tmp);

        filesystem_entry_fabric fb;

        res.push_back(fb.entryFolder(p, "..."));

        for (auto i : std::filesystem::recursive_directory_iterator(p)) {
            auto namae = i.path().filename();
            if (std::regex_search(namae.string(), querry))
                res.push_back(fb.entry(i.path()));
        }

    }
    catch (std::regex_error& e) {
        std::cout << e.what();
    }

    return res;
}

void PrintList(const std::vector<filesystem_entry*>& res) 
{
    for (auto& i : res) {
        auto tt = to_time_t(std::filesystem::last_write_time(i->getPath()));
        tm ss;
        localtime_s(&ss, &tt);
        std::cout << ss.tm_year + 1900 << ":" << ss.tm_mon << ":" << ss.tm_mday << "  " << (i->isFolder()? "f " : "") << i->getName() << std::endl;
    }
}

std::string _toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::vector<filesystem_entry*> _sortByName(std::vector<filesystem_entry*> s, bool rec)
{
    if (rec)
        std::sort(s.begin(), s.end(), [](filesystem_entry* a, filesystem_entry* b) {
            auto tmp = _toLower(a->getName()).compare(_toLower((b->getName())));
            return tmp < 0;
        });
    else
        std::sort(s.begin(), s.end(), [](filesystem_entry* a, filesystem_entry* b) {
            auto tmp = _toLower(a->getName()).compare(_toLower((b->getName())));
            return tmp > 0;
        });

    return s;
}

std::vector<filesystem_entry*> _sortByDate(std::vector<filesystem_entry*> s, bool rec) {
    if (rec)
        std::sort(s.begin(), s.end(), [](filesystem_entry* a, filesystem_entry* b) {
            return std::filesystem::last_write_time(a->getPath()) < std::filesystem::last_write_time(b->getPath());
        });
    else
        std::sort(s.begin(), s.end(), [](filesystem_entry* a, filesystem_entry* b) {
            return std::filesystem::last_write_time(a->getPath()) > std::filesystem::last_write_time(b->getPath());
        });

    return s;
}

std::vector<filesystem_entry*> _sortByFiletype(std::vector<filesystem_entry*> s, bool rec) {
    if (rec)
        std::sort(s.begin(), s.end(), [](filesystem_entry* a, filesystem_entry* b) {
        auto tmp = _toLower(a->getPath().extension().string()).compare(_toLower((b->getPath().extension().string())));
        return tmp < 0;
            });
    else
        std::sort(s.begin(), s.end(), [](filesystem_entry* a, filesystem_entry* b) {
        auto tmp = _toLower(a->getPath().extension().string()).compare(_toLower((b->getPath().extension().string())));
        return tmp > 0;
            });
    
    return s;
}