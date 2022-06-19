#include <string>
#include <vector>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
void base64_decode(std::string const&s, std::vector<unsigned char>& bytes);

int _base64_decode( const char * base64, unsigned char * bindata );