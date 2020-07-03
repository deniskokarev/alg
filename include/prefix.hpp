#ifndef __PREFIX_HH__
#define __PREFIX_HH__

#include <vector>
#include <string>

/**
 * KMP prefix function
 * prefix vector should be given in the same size as str (or greater)
 */
void prefix_function(const std::string &str, std::vector<int> &prefix);

/**
 * find first substring is a string based on KMP prefix algorithm
 * will use extra memory to construct intermediate string and prefix buffer
 */
size_t kmp_strstr(const std::string &str, const std::string &key);

#endif // __PREFIX_HH__
