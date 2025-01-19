

# ifndef TOOLKITDUMMY_HPP

# define TOOLKITDUMMY_HPP

//C++ headers
# include <string>
# include <cstring>

class ToolkitDummy
{
    public:
        ToolkitDummy();
        ToolkitDummy(int value);
        ~ToolkitDummy();
        ToolkitDummy(const ToolkitDummy& other);
        ToolkitDummy& operator=(const ToolkitDummy& other);

        bool operator==(const ToolkitDummy& other);
        bool operator!=(const ToolkitDummy& other);

        const char* print() const;

        int                     getValue() const;
        const std::string&      getName() const;

    private:
        int             m_value;
        int*            m_data;
        std::string     m_name;
};

std::ostream& operator<<(std::ostream& os, const ToolkitDummy& dummy);

#endif