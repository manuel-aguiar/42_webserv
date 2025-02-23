

# include "CgiNamespace.h"

namespace Cgi
{
    Header::Header(const char* key, const char* value) : key(key), value(value) {}
    Header::Header(const BufferView& key, const BufferView& value) : key(key), value(value) {}
}
