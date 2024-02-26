
**Implementation:**

iterate over the input char* (stored in a string_view) and append to the result as required (padded digits or non digits)

while (iter != end(input)  
{  
  &nbsp;if (!digit(c))  
    &nbsp; &nbsp; result.append(c);  
     &nbsp; &nbsp; update(iter)  
   &nbsp; else  
      &nbsp; &nbsp; find_count_of_subsequent_digits()  
      &nbsp; &nbsp; padding = calculate_count_of_zeros_to_pad // could be zero  
      &nbsp; &nbsp; result.append(padding)  
      &nbsp; &nbsp; result.append(digit)  
      &nbsp; &nbsp; update(iter)  
}  

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
