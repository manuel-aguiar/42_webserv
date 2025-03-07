#include "HttpResponse.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../GenericUtils/Files/File.hpp"

#include <iostream>
#include <string>
#include <ctime>

namespace Http {

	unsigned long simple_hash(const std::string& input)
	{
		unsigned long	hash = 1234;
		for (size_t i = 0; i < input.length(); ++i)
			hash = ((hash << 5) + hash) + input[i]; // hash * 33 + char
		return (hash);
	}

	#define ETAG_FILE_READ_AMOUNT 1024

	// Function to generate ETag from content and last modified time
	std::string Response::mf_generateEtag(File& file, time_t last_modified)
	{
		char content[ETAG_FILE_READ_AMOUNT];
		
		file.seek(0, SEEK_SET);
		size_t bytes_read = file.read(content, ETAG_FILE_READ_AMOUNT);
		file.seek(0, SEEK_SET);

		std::string		content_str(content, bytes_read);
		std::string		size_str = StringUtils::to_string(file.size());
		std::string		last_modified_str = StringUtils::to_string(last_modified);
		std::string		content_sample = content_str.substr(0, std::min(bytes_read, size_t(ETAG_FILE_READ_AMOUNT)));
		std::string		combined_input = size_str + "-" + last_modified_str + "-" + content_sample;
		unsigned long	hash_value = simple_hash(combined_input);
		
		return ("\"" + StringUtils::to_string(hash_value) + "\"");
	}
}
