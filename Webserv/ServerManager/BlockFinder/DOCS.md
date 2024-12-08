## API Methods

### Adding Blocks
```c++
void	BlockFinder::addServerBlock(const ServerBlocks& block, const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
```
  - Adds a new server block with specified parameters
  - Empty values are automatically converted to wildcards
  - Duplicate blocks are ignored
  - **Note**: Port is mandatory and cannot be a wildcard

### Finding Blocks
```c++
const ServerBlocks*	BlockFinder::findServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
```
  - Returns pointer to matching server block or NULL if not found
  - Follows precedence rules for matching (see below)

### Checking Block Existence
```c++
bool	BlockFinder::hasServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
```
  - Returns true if a matching block exists
  - Uses normalized directives (empty values converted to wildcards)

### Removing Blocks
```c++
void	BlockFinder::removeServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
```
  - Removes a server block matching the specified parameters
  - Uses normalized directives

## Matching Precedence Rules

When finding a server block, the following precedence order is used:

1. Exact match (IP + port + server name)
2. IP:port match with different server name
3. Wildcard IP (`0.0.0.0`) with exact port and server name
4. IP:port match with wildcard server name (`*`)
5. Wildcard IP with wildcard port and exact server name
6. All wildcards

## Testing

### Running Tests
In the `BlockFinder` directory, run:
```bash
c++ --std=c++98 -Wall -Wextra -Werror test.cpp BlockFinder.cpp -o tester && ./tester
```

or, to get assertions:
```bash
c++ --std=c++98 -Wall -Wextra -Werror test.cpp BlockFinder.cpp -o tester && ./tester -wp
```

### Test Cases Available

The test file (`test.cpp`) includes:

1. **Basic Functionality**
   - Adding blocks with exact values
   - Adding blocks with wildcard IP
   - Adding blocks with empty values (auto-conversion to wildcards)

2. **Edge Cases**
   - Duplicate block handling
   - Empty directive handling
   - Wildcard port validation

3. **Precedence Testing**
   - Multiple blocks with varying specificity
   - Different combinations of exact and wildcard values
