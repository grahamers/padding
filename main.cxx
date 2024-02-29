#include <iostream>
#include <string>
#include <algorithm>
#include <exception>
#include <ranges>
#include <cstring>

/* 

Obviously in real life we would be separating code out into classes, declared in .h and implemented in .cxx etc. 
A "padding" Exception might be defined but std::invalid_argument suffices here. 

The ranges approach "left_padded_ranges" is more elegant and has lazy evaulation. We can discuss but a cleaner way of having to store;

"auto len = result.size();" 

would be cleaner.

*/	


void validate_input(const char* input, const int n)
{
	// handle error and corner cases
	if (nullptr == input)
	{
		throw(std::invalid_argument("null input"));
	}

	if (!std::strlen(input))
	{
		throw(std::invalid_argument("empty input"));
	}

	if (n == 0 || n == 1)
	{
		throw(std::invalid_argument(std::string("wrong padding, n=").append(std::to_string(n))));
	}

	if (n < 0)
	{
		throw(std::invalid_argument(std::string("negative padding value is invald, n=").append(std::to_string(n))));
	}
}

// Note this is NOT the default implementation (there are 2 implementations)
[[nodiscard]] std::string left_padded_ranges(const char* input, const int n)
{
   // up front checks. throws if invalid
   validate_input(input, n);

	
   std::string result;
   #ifndef DONT_RESERVE
   // wouldn't normally put #ifdefs like this in code. 
   // minimise impact of push_back reallocations for large 'n'
   result.reserve(std::strlen(input)  + n);
   #endif
	
   std::string_view sv(input);
   size_t offset{0};
   auto is_digit = [] (const char& c) -> bool { return std::isdigit(c);};
   auto is_not_digit = [] (const char& c) -> bool { return !std::isdigit(c);};


   while (offset < sv.size())
   {
      // non digits
      std::ranges::copy(sv | std::ranges::views::drop(offset)
            | std::ranges::views::take_while(is_not_digit)
            | std::ranges::views::transform([&offset] (char c) { ++offset; return c;}),
            std::back_inserter(result));

      auto dist = std::ranges::distance(sv | std::ranges::views::drop(offset) | std::views::take_while(is_digit));


      // only append if there are digits to append
      result.append(dist && (n - dist > 0) ? n-dist : 0 , '0');
      result.append(sv, offset, dist);

      // update the offset for next iteration
      offset+=dist;
   }

   return result;
}

// Note this IS the default implementation (there are 2 implementations)
[[nodiscard]] std::string left_padded(const char* input, const int n)
{
	// up front checks. throws if invalid
	validate_input(input, n);

	std::string result;
	// wouldn't normally put #ifdefs like this in code. 
	#ifndef DONT_RESERVE
	// minimise impact of push_back reallocations for large 'n'
	result.reserve(std::strlen(input)  + n);
	#endif

	std::string_view str{input};
	auto curr_start = begin(str);

	// iterate from left to right
	while (curr_start != str.end())
	{
		// simply add any non digits
		if (!std::isdigit(*curr_start))
		{
			result.push_back(*curr_start);
			// move curr_start along
			++curr_start;
		}
		else // (padded) digits
		{
			auto first_non_digit_loc = std::find_if(curr_start, end(str),  [](char c) { return !std::isdigit(c); });
			auto dist = std::distance(curr_start, first_non_digit_loc);
			// add the padding based on the number of digits found
			result.append(n - dist > 0 ? n-dist : 0 , '0');
			// add the actual digits
			result.append(curr_start, curr_start + dist);

			// update curr_start for next loop iteration
			curr_start = first_non_digit_loc;
		}
	}

	result.shrink_to_fit();
	return result;

}

int main(int argc, char** argv) 
{

	if (argc !=3)
	{
		std::cerr << "usage: " << argv[0] << " <input string> <n>" << std::endl;
		return 1;
	}


	char* input {argv[1]};	
	try
	{
		int n = std::stoi(argv[2]);
		auto result = left_padded(input, n);
		// range based approach
		// auto result = left_padded_ranges(input, n);
		
		// wouldn't normally put #ifdefs like this in code. 
		#ifndef DONT_COUT
		std::cout << result << std::endl;
		#endif
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "OUT OF RANGE, check parameters " << std::endl;
	}
	catch (const std::bad_alloc& ba)
	{
		std::cerr << "MEMORY EXCHAUSTED, check parameters " << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "GENERAL EXCEPTION: " << ex.what() << ", check parameters " << std::endl;
	}
	catch ( ... )
	{
		std::cerr << "UNKNOWN EXCEPTION DEBUG! " << std::endl;
	}
	return 0;

}

