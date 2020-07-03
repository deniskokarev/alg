#include "prefix.hpp"
#include "gtest/gtest.h"

TEST(Prefix, PrefixSimple) {
	EXPECT_EQ(4, kmp_strstr("abracadabra", "cad"));
}

TEST(Prefix, StandardStrStr) {
	// finding 'aaaaab' in large 'aaaaaaaaaaaaaaaaaaaaaaab' string
	std::string text(1024*1024, 'a');
	text.back() = 'b';
	std::string key(1024, 'a');
	key.back() = 'b';
	EXPECT_EQ(1024*1023, text.find(key));
}

TEST(Prefix, PrefixPerformace) {
	// finding 'aaaaab' in large 'aaaaaaaaaaaaaaaaaaaaaaab' string
	std::string text(1024*1024, 'a');
	text.back() = 'b';
	std::string key(1024, 'a');
	key.back() = 'b';
	EXPECT_EQ(1024*1023, kmp_strstr(text, key));
}

TEST(Prefix, PrefixRepeating) {
	// prefix function for abcabcabcabcabcabc
	// should be 0 0 0 1 2 3 4 5 ...
	int sz = 1024*1024*3;
	std::string text(sz, ' ');
	for (int i=0; i<sz; i+=3)
		text[i] = 'a';
	for (int i=1; i<sz; i+=3)
		text[i] = 'b';
	for (int i=2; i<sz; i+=3)
		text[i] = 'c';
	std::vector<int> prefix(sz);
	prefix_function(text, prefix);
	for (int i=3; i<sz; i++)
		EXPECT_EQ(i-2, prefix[i]);
}
