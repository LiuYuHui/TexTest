#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "TexTest.hpp"
#include <vector>
#include <string>
TEST_CASE("vector test"){
    std::vector<int> a{1,2,3,4,5};
    EXPECT_EQ(a.size(),6);
     EXPECT_EQ(a.size(),4);
}

TEST_CASE("string test"){
    std::string s {"test"};
    EXPECT_EQ(s.size(),4);
    EXPECT_EQ(s.size(),5);
}

TEST_CASE("expressions should be evaluated only once") {
    int a = 5;
    EXPECT_EQ(++a, 6);
    EXPECT_EQ(++a, 8);
}