# TexTest
This is a naive implementation of unit test of c++

# Usage
Just need include `TexTest.hpp`

# Example
```cpp
#include "TexTest.hpp"
#include <vector>
#include <string>
TEST_CASE("vector test"){
    std::vector<int> a{1,2,3,4,5};
    EXPECT_EQ(a.size(),6);
    EXPECT_EQ(a.size(),4);
}
```
