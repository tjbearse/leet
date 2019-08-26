#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "../../catch.hpp"
#include <sstream>
#include <utility>
// end test requires

#include <string>
using namespace std;

struct range {
	int start;
	int len;
};

range checkEven(const string & s, int i) {
	int c = 0;
	while(i-c-1 >= 0 &&
		i+c < s.size() &&
		s[i-c-1] == s[i+c]
	) {
		c++;
	}
	return range{i-c,2*c};
}

range checkOdd(const string & s, int i) {
	int c = 0;
	while(i-c-1 >= 0 &&
		i+c+1 < s.size() &&
		s[i-c-1] == s[i+c+1]
	) {
		c++;
	}
	return range{i-c,1+2*c};
}

string longestPalindrome(string s) {
	if (s == "") {
		return s;
	}
	range best{0,1};
	int len = s.size();
	int c = len/2;
	int o = 0;
	int i = c + o;
	while (i > best.len/2 && i < len - best.len/2) {
		range even = checkEven(s, i);
		if (even.len > best.len) {
			best = even;
		}
		range odd = checkOdd(s, i);
		if (odd.len > best.len) {
			best = odd;
		}
		if (o >= 0) {
			o = -(o + 1);
		} else {
			o = -o;
		}
		i = c + o;
	}
	string ans(s, best.start, best.len);
	return ans;
}

TEST_CASE("even palindrome") {
	REQUIRE( longestPalindrome("AAy") == "AA");
	REQUIRE( longestPalindrome("AA") == "AA");
	REQUIRE( longestPalindrome("xAAy") == "AA");
	REQUIRE( longestPalindrome("xzAAy") == "AA");
	REQUIRE( longestPalindrome("xzBAABy") == "BAAB");
}

TEST_CASE("odd palindrome") {
	REQUIRE( longestPalindrome("A") == "A");
	REQUIRE( longestPalindrome("BAB") == "BAB");
	REQUIRE( longestPalindrome("BABy") == "BAB");
	REQUIRE( longestPalindrome("xBABy") == "BAB");
	REQUIRE( longestPalindrome("xzBABy") == "BAB");
	REQUIRE( longestPalindrome("xzCBABCy") == "CBABC");
}

TEST_CASE("multiple palindromes") {
	REQUIRE( longestPalindrome("AABA") == "ABA");
	REQUIRE( longestPalindrome("BABBA") == "ABBA");
	REQUIRE( longestPalindrome("ABBAB") == "ABBA");

	REQUIRE( longestPalindrome("ABBABB") == "BBABB");
	REQUIRE( longestPalindrome("ABBAABBA") == "ABBAABBA");
}

TEST_CASE("undefined string") {
	REQUIRE( longestPalindrome("AB").size() == 1); // equal
	REQUIRE( longestPalindrome("AAxyBB").size() == 2); // equal
	REQUIRE( longestPalindrome("ACAxyBCB").size() == 3); // equal

	REQUIRE( longestPalindrome("ABCDEFEDCBAabcdefedcba").size() == string("ABCDEFEDCBA").size()); // equal
	REQUIRE( longestPalindrome("abcdefedcbaxABCDEFEDCBAyabcdefedcba").size() == string("abcdefedcba").size()); // equal
}

TEST_CASE("early exit") {
	// forward
	REQUIRE( longestPalindrome("ABCDEFEDCBAabcdeedcba") == "ABCDEFEDCBA");
	REQUIRE( longestPalindrome("ABCDEFEDCBAabcdeffedcba") == "abcdeffedcba");
	// outside
	REQUIRE( longestPalindrome("abcdeedcbaxABCDEFEDCBAyabcdeedcba") == "ABCDEFEDCBA");
}

TEST_CASE("benchmark") {
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	string ralphabet = "zyxwvutsrqponmlkjihgfedcba";
	string thousandA(1000, 'a');

	stringstream ss;
	// 19 times forward, 19 times backward
	int n = 19;
	for (int i=0; i < n; i++) {
		ss << alphabet;
	}
	for (int i=0; i < n; i++) {
		ss << ralphabet;
	}
	string thousandAlphabet = ss.str();

	ss.str("");
	for (int i=0; i < n; i++) {
		ss << alphabet << ralphabet;
	}
	string thousandAlphabet2 = ss.str();

	BENCHMARK("palindrome alphabet") {
		return longestPalindrome(alphabet + ralphabet);
	};

	BENCHMARK("palindrome repeatChar") {
		return longestPalindrome("aaaaaaaaaaaaaaaaaaaaaaaaaa");
	};

	BENCHMARK("palindrome alphabet len ~1000") {
		return longestPalindrome(thousandAlphabet);
	};

	BENCHMARK("palindrome repeatCharx1000") {
		return longestPalindrome(thousandA);
	};

	BENCHMARK("many small palindromes repeatCharx1000") {
		return longestPalindrome(thousandAlphabet2);
	};
}
