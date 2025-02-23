


// Project headers
#include "../../../Toolkit/Arrays/DynArray/DynArray.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

// C++ headers
#include <string>
#include <cstring>		// memcmp
#include <algorithm> 	// std::sort
#include <ctime>		// time stuff

// C headers, directories, stat stuff
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

struct DirEntry
{
    std::string 	name;
    bool 			isDir;
    off_t 			size;
    std::time_t 	lastModified;

	bool operator<(const DirEntry& other) const
	{
		if (isDir != other.isDir)
			return (isDir);
		return (name < other.name);
	}
};

bool compareDirEntries(const DirEntry& a, const DirEntry& b)
{
    return (a < b); // Uses the operator< defined in DirEntry
}


bool listDirectory(const std::string& path, DynArray<DirEntry>& fillEntries)
{
    DIR *dir = opendir(path.c_str());

    if (!dir)
        return (false);

    struct dirent *entry;
    while ((entry = readdir(dir)))
	{
        if (!std::memcmp(entry->d_name, ".", 2) || !std::memcmp(entry->d_name, "..", 3))
            continue;

        std::string fullPath = path + "/" + entry->d_name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0)
		{
            fillEntries.emplace_back();
			DirEntry& e = fillEntries.back();
            e.name = entry->d_name;
            e.isDir = S_ISDIR(st.st_mode);
            e.size = st.st_size;
            e.lastModified = st.st_mtime;
        }
    }
    closedir(dir);

	std::sort(fillEntries.begin(), fillEntries.end(), compareDirEntries);

    return (true);
}

std::string generateDirectoryListing(const std::string &path, DynArray<DirEntry> &entries)
{
    // HTML Header
	std::string output;

	output = "<!DOCTYPE html>\n";

    output 	+= "<html>\n<head>\n";
	output 	+= "<title>Index of ";
	output 	+= path;
	output 	+= "</title>\n";
	output 	+= "<style>body{font-family:Arial;}table{width:100%;border-collapse:collapse;}th,td{padding:8px;border:1px solid #ccc;}</style>\n";
	output 	+= "</head>\n<body>\n";
	output 	+= "<h1>Index of ";
	output 	+= path;
	output 	+= "</h1>\n";
	output 	+= "<table>\n";
	output 	+= "<tr><th>Name</th><th>Size</th><th>Last Modified</th></tr>\n";

    output 	+= "<tr><td><a href=\"../\">../ (Parent Directory)</a></td><td>-</td><td>-</td></tr>\n";

    for (size_t i = 0; i < entries.size(); ++i)
	{
		output 	+= "<tr><td><a href=\"";
		output 	+= entries[i].name;
		output 	+= (entries[i].isDir ? "/" : "");
		output 	+= "\">";
		output 	+= entries[i].name;
		output 	+= (entries[i].isDir ? "/" : "");
		output 	+= "</a></td>";
		output 	+= "<td>";
		output 	+= (entries[i].isDir ? "-" : StringUtils::to_string(entries[i].size));
		output 	+= "</td>";
		output 	+= "<td>";
		output 	+= std::asctime(std::localtime(&entries[i].lastModified));
		output 	+= "</td></tr>\n";
    }

    // HTML Footer
    output 	+= "</table>\n</body>\n</html>\n";
	return (output);
}

/*
	Generates the message body in html for the directory listing on a target folder
	@param path: the path to the target folder
	@param placeOutput: the string to place the html output
*/
std::string	DirectoryListing(const std::string& path)
{
	std::string output;

	try
	{
		DynArray<DirEntry> entries;

		if (listDirectory(path, entries))
			output = generateDirectoryListing(path, entries);
		else
			output = "";
	}
	catch(const std::exception& e)
	{
		output = "";
	}

	return (output);
}
