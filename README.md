<ins>**Overview**</ins>

The code here presents an illustration of the evolution of a solution to a "padding" problem. The first implemention could
possibly be termed the "reflex" approach. On top of that a second approach is provided using C++ 20 "ranges" which illustrates
the power of ranges and how code is impacted. The range based solution is by far the cleanest, most readable and modern. 

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
Then search for sequence of digits (find_if + another lambda). Using offsets and std::distance between iterators, calculate the padding (if any) required. 
Append (possibly zero) padding to result.
Append digits to result.


The following pseudo code describes the first approach;


while (iter  != end(input)\
{\
  &nbsp; append_non_digits_to_result;\
  &nbsp; digit_count = count_subsequent_digits()  
  &nbsp; padding = calculate_count_of_zeros_to_pad(digit_count) // could be zero  
  &nbsp; result.append(padding)  
  &nbsp; result.append(digits)  
  &nbsp; update(iter)  
}  


I was happy enough with that approach however the code backing it is clunky (first_non_digit_loc and dist). 

![image](https://github.com/grahamers/padding/assets/19392728/882d21f0-fcaf-4bb2-a5fd-4c61c66a0ce4)


**Second Implementation:**

Here, we let ranges do most of the work for us. Again, use offsets (to point to "current" item in input string).

pseudo code:

while (offset < input.size())\
{\
  &nbsp; take_view_of_non_digits_from_input_sequence_and_copy_to_output;\
  &nbsp; determine_padding_by_taking_view_of_digits_in_input 
  &nbsp; result.append(padding)  
  &nbsp; result.append(digits)  
  &nbsp; update(offset)  
}  

![image](https://github.com/grahamers/padding/assets/19392728/9b993261-e2e9-41b0-8547-4a55e689eec7)

Ranges help us out here in 2 places;

Here, in a single line of code (albeit split over 4 lines for readability, we are taking a view of all non
digit characters from input sequence, copying them to the output result and updating the offset via transform.

![image](https://github.com/grahamers/padding/assets/19392728/051e4e16-25ad-448c-bc66-bed51385cefe)



Then we handle digits, again ranges (via offset) give us a "view" of the subsequent sequence of digits. The count of these digits
will be used to determine padding (std::ranges::distance).

![image](https://github.com/grahamers/padding/assets/19392728/02199ad0-6167-46f1-a928-f8890e571da7)


After that, its simply a matter of appending the padding and digits and updating the offset.


![image](https://github.com/grahamers/padding/assets/19392728/5301720a-b5eb-436a-968c-b42b7344f7bf)


A third solution is possible (without ranges) using regex. 

Match digit and non digit sequences using a sregex_iterator. Add padding to all matches that 
are digit sequences of length < N.

![image](https://github.com/grahamers/padding/assets/19392728/fbed0dda-a547-4f6b-8832-f92025ad13dd)


**Build:**

$  g++ --version
*g++ (Ubuntu 13.1.0-8ubuntu1~22.04) 13.1.0*

$ g++ -Wall -Werror -pedantic --std=c++23 -o main ./main.cxx

 
$ ./main "James Bond 7" 3\
James Bond 007

$ ./main "James Bond 07" 3\
James Bond 007

$ ./main "James Bond 007" 3\
James Bond 007

$ ./main "7James7Bond 07" 3\
007James007Bond 007

$ ./main "PI=3.14" 2\
PI=03.14

$ ./main "It's 3:13pm" 2\
It's 03:13pm

$ ./main "It's 12:13pm" 2\
It's 12:13pm

$ ./main "99UR1337" 6\
000099UR001337

$ ./main  a  2\
a

$ ./main  1a  2\
01a

$ ./main  1a1  2\
01a01

$ ./main  11  2\
11

$ ./main  1a11  2\
01a11

$ ./main  11a11  2\
11a11

$ ./main  1111a11  2\
1111a11

$ ./main  1111a11 4\
1111a0011

$ ./main  1 20\
00000000000000000001

$ ./main  "" 4

*GENERAL EXCEPTION: empty input, check parameters* // note this is deliberate.

$ ./main  1 20000000000000

OUT OF RANGE, check parameters


