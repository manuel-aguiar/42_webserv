


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


// Header
static const char* DirList_Header1 = 	"<!DOCTYPE html>\n"
										"<html>\n<head>\n"
										"<title>Index of ";
										//path
static const char* DirList_Header2 = 	"</title>\n"
										"<style>body{font-family:Arial;}table{width:100%;border-collapse:collapse;}th,td{padding:8px;border:1px solid #ccc;}</style>\n"
										"</head>\n<body>\n"
										"<h1>Index of ";
										//path
static const char* DirList_Header3 = 	"</h1>\n"
										"<table>\n"
										"<tr><th>Name</th><th>Size</th><th>Last Modified</th></tr>\n"
										"<tr><td><a href=\"../\">../ (Parent Directory)</a></td><td>-</td><td>-</td></tr>\n";

static const size_t DirList_Header1_len = std::strlen(DirList_Header1);
static const size_t DirList_Header2_len = std::strlen(DirList_Header2);
static const size_t DirList_Header3_len = std::strlen(DirList_Header3);

static const size_t DirList_Header_FixedLen = 
					DirList_Header1_len +
					DirList_Header2_len +
					DirList_Header3_len;
// Folders

static const char *DirList_Folder1 = 	"<tr><td><a href=\"";
										// dir name
										// space or slash
static const char *DirList_Folder2 = 	"\">";
										// dir name
										//space or slash
static const char *DirList_Folder3 = 	"</a></td>"
										"<td>";
										// file size
static const char *DirList_Folder4 = 	"</td>"
										"<td>";
										// last modified
static const char *DirList_Folder5 =	"</td></tr>\n";

static const size_t DirList_Folder1_len = std::strlen(DirList_Folder1);
static const size_t DirList_Folder2_len = std::strlen(DirList_Folder2);
static const size_t DirList_Folder3_len = std::strlen(DirList_Folder3);
static const size_t DirList_Folder4_len = std::strlen(DirList_Folder4);
static const size_t DirList_Folder5_len = std::strlen(DirList_Folder5);

static const size_t DirList_Folder_FixedLen =
									DirList_Folder1_len +
									DirList_Folder2_len +
									DirList_Folder3_len +
									DirList_Folder4_len +
									DirList_Folder5_len;

// Footer
static const char*	DirList_Footer = "</table>\n</body>\n</html>\n";
static const size_t DirList_Footer_len = std::strlen(DirList_Footer);

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
		char hexHeader[10];
		const size_t hexHeaderSize = sizeof(hexHeader) / sizeof(hexHeader[0]);
		const std::string& targetPath = m_responseData.targetPath;
		size_t dirHead_len = DirList_Header_FixedLen + 2 * targetPath.size();
		size_t totalLength = hexHeaderSize + dirHead_len;

		if (writeBuffer.capacity() < totalLength + 2)
			return (Http::IOStatus::MARK_TO_CLOSE);
		if (writeBuffer.available() < totalLength + 2) // + \r\n at the end 
            return (Http::IOStatus::WAITING); // no room
        
        size_t currentPosition = writeBuffer.size();
        writeBuffer.push(hexHeader, hexHeaderSize); // make room for the hex header

        fillHexHeader(hexHeader, hexHeaderSize, dirHead_len);
        std::memcpy(&writeBuffer[currentPosition], hexHeader, hexHeaderSize);
        
        writeBuffer.push(DirList_Header1, DirList_Header1_len);
		writeBuffer.push(targetPath);
		writeBuffer.push(DirList_Header2, DirList_Header2_len);
		writeBuffer.push(targetPath);
		writeBuffer.push(DirList_Header3, DirList_Header3_len);
        writeBuffer.push("\r\n", 2);

		m_dirListing_target = ::opendir(targetPath.c_str());
		m_dirListing_curEntry = ::readdir(m_dirListing_target);

		m_fillFunction = &Response::mf_fillDirectoryListing_Folders;
        return (mf_fillDirectoryListing_Folders(writeBuffer));
	}

	Http::IOStatus::Type		
	Response::mf_fillDirectoryListing_Folders(BaseBuffer& writeBuffer)
	{
		char hexHeader[10];
		const size_t hexHeaderSize = sizeof(hexHeader) / sizeof(hexHeader[0]);
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
				writeBuffer.push(hexHeader, hexHeaderSize);
			}

			std::string fullPath = targetPath + "/" + m_dirListing_curEntry->d_name;
			
			struct stat st;
			if (stat(fullPath.c_str(), &st) != 0)
				return (Http::IOStatus::MARK_TO_CLOSE);
			
			totalSize = 0;
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder1, DirList_Folder1_len);
				totalSize += DirList_Folder1_len;
			///////////////////////////////////////////////            
			writeBuffer.push(m_dirListing_curEntry->d_name, entryNameLen);
				totalSize += entryNameLen;
			///////////////////////////////////////////////       
			if (S_ISDIR(st.st_mode))
			{
				writeBuffer.push("/", 1);
				totalSize++;
			}
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder2, DirList_Folder2_len);
				totalSize += DirList_Folder2_len;
			///////////////////////////////////////////////
			writeBuffer.push(m_dirListing_curEntry->d_name, entryNameLen);
				totalSize += entryNameLen;
			///////////////////////////////////////////////
			if (S_ISDIR(st.st_mode))
			{
				writeBuffer.push("/", 1);
				totalSize++;
			}
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder3, DirList_Folder3_len);
				totalSize += DirList_Folder3_len;
			///////////////////////////////////////////////
			if (S_ISDIR(st.st_mode))
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
			writeBuffer.push(DirList_Folder4, DirList_Folder4_len);
				totalSize += DirList_Folder4_len;
			///////////////////////////////////////////////
			const char* lastModified = std::asctime(std::localtime(&st.st_mtime));
			size_t lastMod_len = std::strlen(lastModified);

			writeBuffer.push(lastModified, lastMod_len);
				totalSize += lastMod_len;
			///////////////////////////////////////////////
			writeBuffer.push(DirList_Folder5, DirList_Folder5_len);
				totalSize += DirList_Folder5_len;
			///////////////////////////////////////////////


			totalWritten += totalSize;
			m_dirListing_curEntry = ::readdir(m_dirListing_target);	
		}

		goto writeAvailable;	

	writeFinished:

		m_fillFunction = &Response::mf_fillDirectoryListing_Tail;
		return (mf_fillDirectoryListing_Tail(writeBuffer));
	
	writeAvailable:
		fillHexHeader(hexHeader, hexHeaderSize, totalWritten);
        std::memcpy(&writeBuffer[curPosition], hexHeader, hexHeaderSize);
		writeBuffer.push("\r\n", 2);

		if (m_dirListing_curEntry == NULL)
			goto writeFinished;
		
		return (Http::IOStatus::WRITING);
	}

	Http::IOStatus::Type		
	Response::mf_fillDirectoryListing_Tail(BaseBuffer& writeBuffer)
	{
		char hexHeader[10];
		const size_t hexHeaderSize = sizeof(hexHeader) / sizeof(hexHeader[0]);
		size_t totalLength = hexHeaderSize + DirList_Footer_len + 2 + 5;

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
        writeBuffer.push(hexHeader, hexHeaderSize); // make room for the hex header

        fillHexHeader(hexHeader, hexHeaderSize, DirList_Footer_len);
        std::memcpy(&writeBuffer[currentPosition], hexHeader, hexHeaderSize);
        
        writeBuffer.push(DirList_Footer, DirList_Footer_len);
        writeBuffer.push("\r\n", 2);
		writeBuffer.push("0\r\n\r\n", 5);

		return (mf_fillFinish(writeBuffer));		
	}
}