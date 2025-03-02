
# include <string>
# include <sstream>

extern const char* getStatusMessage(int statusCode);

std::string generateDefaultErrorPage(int statusCode, const std::string& serverName, const std::string& errorMessage)
{
    std::stringstream ss;
    const char* statusText = getStatusMessage(statusCode);

    ss << "<!DOCTYPE html>\n"
    << "<html>\n"
    << "<head>\n"
    << "<title>" << statusCode << " " << statusText << "</title>\n"
    << "<style>\n"
    << "body {\n"
    << "    font-family: system-ui, sans-serif;\n"
    << "    margin: 40px auto;\n"
    << "    max-width: 1750px;\n"
    << "    padding: 0 10px;\n"
    << "    color: #444;\n"
    << "    text-align: center;\n"
    << "}\n"
    << ".server {\n"
    << "    font-size: 20px;\n"
    << "    font-weight: bold;\n"
    << "}\n"
    << ".status-code {\n"
    << "    font-size: 48px;\n"
    << "    font-weight: bold;\n"
    << "    color: #e74c3c;\n"
    << "    margin: 30px 0 10px 0;\n"
    << "}\n"
    << "h1 {\n"
    << "    margin: 10px 0;\n"
    << "}\n"
    << "hr {\n"
    << "    width: 100%;\n"
    << "    margin: 10px auto;\n"
    << "    border: none;\n"
    << "    border-top: 1px solid #ddd;\n"
    << "}\n"
    << "</style>\n"
    << "</head>\n"
    << "<body>\n"
    << "    <div class=\"server\">" << serverName << "</div>\n"
    << "    <hr>\n"
    << "    <div class=\"status-code\">" << statusCode << "</div>\n"
    << "    <h1>" << statusText << "</h1>\n"
    << "    <p>" << errorMessage << "</p>\n"
    << "</body>\n"
    << "</html>\n";

    return (ss.str());
}