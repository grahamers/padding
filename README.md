<ins>**Overview**</ins>

The code here presents an illustration of the evolution of a solution to a "padding" problem. The first implemention could
possibly be termed the "reflex" approach. On top of that a second approach is provided using C++ 20 "ranges". Finally a third solution is coded illustrating
the power of ranges and how code is impacted. The final range based solution is by far the cleanest, most readable and modern. 

<ins>**Task**</ins>

Provide a "padding" function that will pad (prefix) a string to N chars such that any sequence of numeric digits is at least N
digits long, any sequences will be padded with zeros. 

e.g. \
N = 3, input "1" -> "001"\
N = 4, input "1" -> "0001"\
N = 3, input "x" -> "x"\
N = 2, input "1a8" -> "01a08"\
etc.




**First Implementation:**

Iterate over the input using "iter".  

Search for sequence of non digits (STL find_if + lambda), append this directly to output. 
Search for sequence of digits (find_if + lambda). Using offsets and std::distance between iterators, calculate the padding (if any) required. 
Append (possibly zero) padding to result.
Append digits to result.


The following pseudo code describes the first approach;


while (iter  != end(input)\
{\
  &nbsp; append_non_digits_to_result;\
  &nbsp;  update iter;\
  &nbsp; digit_count = count_subsequent_digits()  
  &nbsp; padding = calculate_count_of_zeros_to_pad(digit_count) // could be zero  
  &nbsp; result.append(padding)  
  &nbsp; result.append(digits)  
  &nbsp; update(iter)  
}  


I was happy enough with that approach (time and space complexity are fine). This translates to the following code;

![approach_1](https://github.com/grahamers/padding/assets/19392728/946b2a2b-0093-4ad0-a69e-dfa4673005bf)

**NOTE:**

There are two implementations, the first "left_padded" (above pseudo code) and a second "left_padded_ranges" which uses c++ 20 views/ranges. The default is
"left_padded" as described above. 

**Build:**
$  g++ --version
*g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0*

$ g++ -Wall -Werror -pedantic --std=c++23 -o main ./main.cxx

There are 2 #ifndefs in code, simply to enable "time" for simple benchmark. By default just compile as above.

a) "DONT_RESERVE"  They simply serve to highlight the effect of reserving space in the output result (string). If n is very large, not calling "reserve()" on result up front has a significant performance impact.

b) "DONT_COUT" , as it name indicates, we dont cout the result (if we want to use "time" to measure actual padding algorithm.

This will disable the reserve and wont print the result

$ g++ -DDONT_RESERVE -DDONT_COUT  -Wall -Werror -pedantic --std=c++23 -o main ./main.cxx

This will reserve and wont print the result

$ g++ -DDONT_COUT  -Wall -Werror -pedantic --std=c++23 -o main ./main.cxx

The default: (reserve and print)

**Tests:**
 
$ ./main "James Bond 7" 3

James Bond 007

$ ./main "James Bond 07" 3

James Bond 007

$ ./main "James Bond 007" 3

James Bond 007

$ ./main "7James7Bond 07" 3

007James007Bond 007

$ ./main "PI=3.14" 2

PI=03.14

$ ./main "It's 3:13pm" 2

It's 03:13pm

$ ./main "It's 12:13pm" 2

It's 12:13pm

$ ./main "99UR1337" 6

000099UR001337

$ ./main  a  2

*a*

$ ./main  1a  2

*01a*

$ ./main  1a1  2

*01a01*

$ ./main  11  2

*11*

$ ./main  1a11  2

*01a11*

$ ./main  11a11  2

*11a11*

$ ./main  1111a11  2

*1111a11*
$ ./main  1111a11 4

*1111a0011*

$ ./main  1 20

00000000000000000001

$ ./main  "" 4

*GENERAL EXCEPTION: empty input, check parameters*

$ ./main  1 20000000000000

OUT OF RANGE, check parameters

If we want to see the performance without the call to reserve() on the result string (and also diable printing the result);

The following should highlight;

$ g++ -DDONT_RESERVE -DDONT_COUT  -Wall -Werror -pedantic --std=c++23 -o main ./main.cxx

$ time ./main  1 1000000000

real    0m3.395s

user    0m0.374s

sys     0m3.021s


$ g++  -DDONT_COUT  -Wall -Werror -pedantic --std=c++23 -o main ./main.cxx

$ time ./main  1 1000000000

real    0m0.555s

user    0m0.121s

sys     0m0.433s



**Time complexity & space complexity:**

We can discuss. 
