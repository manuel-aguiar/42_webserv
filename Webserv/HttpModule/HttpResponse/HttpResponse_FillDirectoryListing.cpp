


// Project headers
#include "../../../Toolkit/Arrays/DynArray/DynArray.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/BufferView/BufferView.hpp"

// C++ headers
#include <string>
#include <cstring>		// memcmp
#include <algorithm> 	// std::sort
#include <ctime>		// time stuff

// C headers, directories, stat stuff
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


// Header
static const BufferView DirList_Header1("<!DOCTYPE html>\n"
										"<html>\n<head>\n"
										"<title>Index of ");
										//path
static const BufferView DirList_Header2("</title>\n"
										"<style>body{font-family:Arial;}table{width:100%;border-collapse:collapse;}th,td{padding:8px;border:1px solid #ccc;}</style>\n"
										"</head>\n<body>\n"
										"<h1>Index of ");
										//path
static const BufferView DirList_Header3("</h1>\n"
										"<table>\n"
										"<tr><th>Name</th><th>Size</th><th>Last Modified</th></tr>\n"
										"<tr><td><a href=\"../\">../ (Parent Directory)</a></td><td>-</td><td>-</td></tr>\n");


static const size_t DirList_Header_FixedLen = 
					DirList_Header1.size() +
					DirList_Header2.size() +
					DirList_Header3.size();
// Folders

static const BufferView DirList_Folder1("<tr><td><a href=\"");
										// dir name
										// space or slash
static const BufferView DirList_Folder2("\">");
										// dir name
										//space or slash
static const BufferView DirList_Folder3("</a></td>"
										"<td>");
										// file size
static const BufferView DirList_Folder4("</td>"
										"<td>");
										// last modified
static const BufferView DirList_Folder5("</td></tr>\n");

static const size_t DirList_Folder_FixedLen =
									DirList_Folder1.size() +
									DirList_Folder2.size() +
									DirList_Folder3.size() +
									DirList_Folder4.size() +
									DirList_Folder5.size();

// Footer
static const BufferView	DirList_Footer = ("</table>\n</body>\n</html>\n");

    static void fillHexHeader(char* hexHeader, int hexheaderSize, int chunkSize)
    {
        const char* hex = "0123456789abcdef";
    
        int i = hexheaderSize - 1;
        hexHeader[i--] = '\n';
        hexHeader[i--] = '\r';
        
        while (i >= 0)
        {
            if (chunkSize)
            {
                hexHeader[i--] = hex[chunkSize % 16];
                chunkSize /= 16;
            }
            else
                hexHeader[i--] = '0';
        }
    }

# include "HttpResponse.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/BufferView/BufferView.hpp"

namespace Http
{
	Http::IOStatus::Type		
	Response::mf_fillDirectoryListing_Head(BaseBuffer& writeBuffer)
	{
		const size_t hexHeaderSize = 10; // 10 bytes for the hex header (8 bytes for the size, 2 for \r\n)
		const std::string& targetPath = m_responseData.targetPath;
		size_t dirHead_len = DirList_Header_FixedLen + 2 * targetPath.size();
		size_t totalLength = hexHeaderSize + dirHead_len;

		if (writeBuffer.capacity() < totalLength + 2)
			return (Http::IOStatus::MARK_TO_CLOSE);
		if (writeBuffer.available() < totalLength + 2) // + \r\n at the end 
            return (Http::IOStatus::WAITING); // no room
        
        size_t currentPosition = writeBuffer.size();
        writeBuffer.push("0", hexHeaderSize); // make room for the hex header

        fillHexHeader((char*)&writeBuffer[currentPosition], hexHeaderSize, dirHead_len);
        
        writeBuffer.push(DirList_Header1);
		writeBuffer.push(targetPath);
		writeBuffer.push(DirList_Header2);
		writeBuffer.push(targetPath);
		writeBuffer.push(DirList_Header3);
        writeBuffer.push("\r\n", 2);

		m_dirListing_target = ::opendir(targetPath.c_str());
		if (!m_dirListing_target)
			goto emptyFolder;

		m_dirListing_curEntry = ::readdir(m_dirListing_target);
		if (!m_dirListing_curEntry)
			goto emptyFolder;

		m_fillFunction = &Response::mf_fillDirectoryListing_Folders;
        return (mf_fillDirectoryListing_Folders(writeBuffer));

		/////////////////////////////////////

	emptyFolder:
		m_fillFunction = &Response::mf_fillDirectoryListing_Tail;
		return (mf_fillDirectoryListing_Tail(writeBuffer));
	}

	Http::IOStatus::Type		
	Response::mf_fillDirectoryListing_Folders(BaseBuffer& writeBuffer)
	{
		const size_t hexHeaderSize = 10; // 10 bytes for the hex header (8 bytes for the size, 2 for \r\n)
		const std::string& targetPath = m_responseData.targetPath;
		const size_t fixedEntrySize = 
				DirList_Folder_FixedLen + 
				25 + // room for time
				2 + // directory indicators;
				20; // size_t max number of characters, sizeof file
		size_t totalWritten = 0;
		int curPosition = -1;

		if (writeBuffer.available() < hexHeaderSize + 2)
			return (Http::IOStatus::WAITING);

		while (m_dirListing_curEntry)
		{
			if (!std::memcmp(m_dirListing_curEntry->d_name, ".", 2) || !std::memcmp(m_dirListing_curEntry->d_name, "..", 3))
			{
				m_dirListing_curEntry = ::readdir(m_dirListing_target);
				continue ;
			}
			size_t entryNameLen = std::strlen(m_dirListing_curEntry->d_name);
			size_t totalSize = fixedEntrySize + 2 * entryNameLen;
			size_t requirement = totalSize + (curPosition == -1 ? hexHeaderSize + 2 : 0);

			if (writeBuffer.capacity() < requirement)
				return (Http::IOStatus::MARK_TO_CLOSE);

			if (writeBuffer.available() < requirement)
				goto writeAvailable;
			
			if (curPosition == -1)
			{
				curPosition = writeBuffer.size();
				writeBuffer.push("0", hexHeaderSize);
			}

			std::string fullPath = targetPath + "/" + m_dirListing_curEntry->d_name;
			
			struct stat st;
			if (stat(fullPath.c_str(), &st) != 0)
				return (Http::IOStatus::MARK_TO_CLOSE);
			
			bool isDirectory = S_ISDIR(st.st_mode);

			totalSize = 0;
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder1);
				totalSize += DirList_Folder1.size();
			///////////////////////////////////////////////            
			writeBuffer.push(m_dirListing_curEntry->d_name, entryNameLen);
				totalSize += entryNameLen;
			///////////////////////////////////////////////       
			if (isDirectory)
			{
				writeBuffer.push("/", 1);
				totalSize++;
			}
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder2);
				totalSize += DirList_Folder2.size();
			///////////////////////////////////////////////
			writeBuffer.push(m_dirListing_curEntry->d_name, entryNameLen);
				totalSize += entryNameLen;
			///////////////////////////////////////////////
			if (isDirectory)
			{
				writeBuffer.push("/", 1);
				totalSize++;
			}
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder3);
				totalSize += DirList_Folder3.size();
			///////////////////////////////////////////////
			if (isDirectory)
			{
				writeBuffer.push("-", 1);
				totalSize++;
			}
			else
			{
				std::string fileSize = StringUtils::to_string(st.st_size);
				writeBuffer.push(fileSize);
				totalSize += fileSize.size();
			}
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder4);
				totalSize += DirList_Folder4.size();
			///////////////////////////////////////////////
			const char* lastModified = std::asctime(std::localtime(&st.st_mtime));
			size_t lastMod_len = std::strlen(lastModified);

			writeBuffer.push(lastModified, lastMod_len);
				totalSize += lastMod_len;
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder5);
				totalSize += DirList_Folder5.size();
			///////////////////////////////////////////////

			totalWritten += totalSize;
			m_dirListing_curEntry = ::readdir(m_dirListing_target);	
		}

		goto writeAvailable;	

		/////////////////////////////////////////////////

	writeFinished:

		m_fillFunction = &Response::mf_fillDirectoryListing_Tail;
		return (mf_fillDirectoryListing_Tail(writeBuffer));
		
		/////////////////////////////////////////////////

	writeAvailable:

		if (curPosition == -1)
		{
			if (m_dirListing_curEntry == NULL)
				goto writeFinished;
			return (Http::IOStatus::WAITING);
		}

		fillHexHeader((char*)&writeBuffer[curPosition], hexHeaderSize, totalWritten);
		writeBuffer.push("\r\n", 2);

		if (m_dirListing_curEntry == NULL)
			goto writeFinished;
		
		return (Http::IOStatus::WRITING);
	}

	Http::IOStatus::Type		
	Response::mf_fillDirectoryListing_Tail(BaseBuffer& writeBuffer)
	{
		const size_t hexHeaderSize = 10; // 10 bytes for the hex header (8 bytes for the size, 2 for \r\n)
		size_t totalLength = hexHeaderSize + DirList_Footer.size() + 2 + 5;

		if (m_dirListing_target != NULL)
		{
			::closedir(m_dirListing_target);
			m_dirListing_target = NULL;
		}

		if (writeBuffer.capacity() < totalLength)
			return (Http::IOStatus::MARK_TO_CLOSE);
		if (writeBuffer.available() < totalLength) // + \r\n at the end 
            return (Http::IOStatus::WAITING); // no room
        
        size_t currentPosition = writeBuffer.size();
        writeBuffer.push("0", hexHeaderSize); // make room for the hex header

        fillHexHeader((char*)&writeBuffer[currentPosition], hexHeaderSize, DirList_Footer.size());
        
        writeBuffer.push(DirList_Footer);
        writeBuffer.push("\r\n", 2);
		writeBuffer.push("0\r\n\r\n", 5);

		return (mf_fillFinish(writeBuffer));		
	}
}
