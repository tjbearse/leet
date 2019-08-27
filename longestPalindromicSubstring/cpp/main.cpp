#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "../../catch.hpp"
#include <sstream>
#include <utility>
// end test requires

#include <string>
using namespace std;

string longestPalindrome(string s) {
	if (s == "") {
		return s;
	}
	int ibest = 0;
	int lbest = 1;
	int len = s.size();
	int center = len/2;
	int offset = 0;
	int i = center;
	// go from the center outward to get big palindromes first
	// restrit range to not check if there isn't room to beat us
	while (i >= lbest/2 && i < len - lbest/2) {
		int L = i, R = i;
		if (L - 1 >= 0 && s[L-1] == s[L]) {
			L--;
		}
		while(L-1 >= 0 &&
			R+1 < len &&
			s[L-1] == s[R+1]
		) {
			R++;
			L--;
		}
		int lcurr = (R-L)+1;
		if (lcurr >= lbest && (lcurr > lbest || L < ibest)) {
			ibest = L;
			lbest = lcurr;
		}
		offset = (offset >= 0) ? -(offset + 1) : -offset;
		i = center + offset;
	}
	return s.substr(ibest, lbest);
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
	REQUIRE( longestPalindrome("babad") == "bab");
	REQUIRE( longestPalindrome("AB") == "A"); // equal
	REQUIRE( longestPalindrome("AAxyBB") == "AA"); // equal
	REQUIRE( longestPalindrome("ACAxyBCB") == "ACA"); // equal

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
