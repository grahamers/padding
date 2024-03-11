#include <iostream>
#include <string>
#include <algorithm>
#include <exception>
#include <cassert>
#include <ranges>

#include <cstring>

using namespace std;

void validate_input(const char *input, const int n)
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

std::string left_padded(const char *input, const int n)
{
   validate_input(input, n);
   std::string result;
   result.reserve(std::strlen(input) + n);

   string_view str{input};
   auto curr_start = begin(str);

   while (curr_start != str.end())
   {
      // append non digits to output
      auto first_digit_loc = std::find_if(curr_start, end(str), [](char c)
                                 { return std::isdigit(c); });
      result.append(curr_start, first_digit_loc);

      // handle digits
      auto first_non_digit_loc = std::find_if(first_digit_loc, end(str), [](char c)
                                    { return !std::isdigit(c); });

      auto dist = std::distance(first_digit_loc, first_non_digit_loc);

      // use dist, n and iterstors to pad output if required. Hmm....
      if (n - dist > 0 && (first_digit_loc != first_non_digit_loc))
      {
         result.append(n - dist, '0');
      }

      // append digits to output.
      result.append(first_digit_loc, first_non_digit_loc);

      curr_start = first_non_digit_loc;
   }

   result.shrink_to_fit();
   return result;
}

[[nodiscard]] std::string left_padded_ranges(const char *input, const int n)
{

   std::string result;
   result.reserve(std::strlen(input) + n);

   std::string_view sv(input);
   size_t offset{0};

   while (offset < sv.size())
   {
      // non digits
      std::ranges::copy(sv | std::ranges::views::drop(offset) | std::ranges::views::take_while([](const char &c) -> bool
                                                                         { return !std::isdigit(c); }) |
                       std::ranges::views::transform([&offset](char c)
                                             { ++offset; return c; }),
                    std::back_inserter(result));

      auto dist = std::ranges::distance(sv | std::ranges::views::drop(offset) | std::views::take_while([](const char &c) -> bool
                                                                               { return std::isdigit(c); }));

      // only append if there are digits to append
      result.append(dist && (n - dist > 0) ? n - dist : 0, '0');
      result.append(sv, offset, dist);

      // update the offset for next iteration
      offset += dist;
   }

   result.shrink_to_fit();
   return result;
}

int main(int argc, char **argv)
{

   if (argc != 3)
   {
      std::cerr << "usage: " << argv[0] << " <input string> <n>" << endl;
      return 1;
   }

   char *input{argv[1]};
   try
   {
      int n = std::stoi(argv[2]);
      auto result = left_padded(input, n);
      auto result2 = left_padded_ranges(input, n);
      assert(result == result2);
      cout << result << endl;
   }
   catch (const std::out_of_range &oor)
   {
      cerr << "OUT OF RANGE, check parameters " << endl;
   }
   catch (const std::bad_alloc &ba)
   {
      cerr << "MEMORY EXCHAUSTED, check parameters " << endl;
   }
   catch (const std::exception &ex)
   {
      cerr << "GENERAL EXCEPTION: " << ex.what() << ", check parameters " << endl;
   }
   catch (...)
   {
      cerr << "Unknown EXCEPTION DEBUG! " << endl;
   }
   return 0;
}
