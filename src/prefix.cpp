#include <prefix.hpp>

/**
 * KMP prefix function
 * prefix vector should be given in the same size as str (or greater)
 */
void prefix_function(const std::string &str, std::vector<int> &prefix) {
	size_t sz = str.size();
	size_t k = 0;
	prefix[0] = 0;
	for (size_t i=1; i<sz; i++) {
		for (; k>0 && str[i] != str[k]; k=prefix[k-1]);
		if (str[i] == str[k]) {
			k++;
			prefix[i] = k;
		} else {
			prefix[i] = 0;
		}
	}
}

/**
 * find first substring is a string based on KMP prefix algorithm
 * will use extra memory to construct intermediate string and prefix buffer
 */
size_t kmp_strstr(const std::string &str, const std::string &key) {
	size_t ksz  = key.size();
	std::string kmp = key + char(0) + str;
	size_t sz = kmp.size();
	std::vector<int> prefix(kmp.size());
	prefix_function(kmp, prefix);
	size_t pos;
	for (pos=ksz+1; pos<sz; pos++)
		if (prefix[pos] == ksz)
			break;
	return pos-ksz-ksz;
}
