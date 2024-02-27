#include <iostream>
#include <string>
#include <algorithm>
#include <exception>
#include <regex>
#include <cstring>


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


[[nodiscard]] std::string left_padded(const char* in, const size_t n)
{
	// up front checks. throws if invalid
	validate_input(in, n);

   // regex match numbers and non numbers
   std::regex pattern("\\d+|\\D+");

   // Use std::sregex_iterator to iterate over matches
	const std::string str(in);
   auto begin = std::sregex_iterator(str.begin(), str.end(), pattern);

   // Iterate over matches and print them
   std::string result;
   for (auto it = begin; it != std::sregex_iterator(); ++it)
   {
      const std::string_view & sv = it->str();

		// pad with zeros depending on length of numeric match
      if (std::isdigit(sv[0]))
      {
         result.append((sv.size() > n) ? 0 : n - sv.size() , '0');
      }
      result.append(sv.begin(), sv.end());
   }

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
		std::cout <<  left_padded(input, n) << std::endl;;
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

