# padding
padding implementation

Build:

g++ --std=c++23 -Wall -pedantic -o main ./main.cxx

or to disable the call to "reserve()" up front and see impact:

g++ --std=c++23 -DDISABLE_RESERVE -Wall -pedantic -o main ./main.cxx


The solution has time complexity and space complexity:

