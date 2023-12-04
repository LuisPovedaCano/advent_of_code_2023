#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

std::size_t calculate_number_of_scratchcards(const std::vector<std::size_t>& results);
std::pair<std::size_t, std::size_t> check_combination_value(const std::vector<int>& winning, const std::vector<int>& played);
std::vector<int> get_numbers_from_string(const std::string& string);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Path to the puzzle file must be provided" << std::endl;
        std::terminate();
    }

    std::ifstream file(argv[1]);
    if (!file.good())
    {
        std::cerr << "Failed to read the file" << std::endl;
        std::terminate();
    }

    std::size_t result_1 { 0 };
    std::size_t result_2 { 0 };
    std::string line {};
    std::vector<std::size_t> combination_values {};
    while (std::getline(file, line, ':'))
    {
        std::string winning_numbers {};
        std::getline(file, winning_numbers, '|');
        std::string played_numbers {};
        std::getline(file, played_numbers);

        std::vector<int> winning_combination { get_numbers_from_string(winning_numbers) };
        std::vector<int> played_combination { get_numbers_from_string(played_numbers) };

        std::size_t result {};
        std::size_t hits {};
        std::tie(result, hits) = check_combination_value(winning_combination, played_combination);
        combination_values.push_back(hits);
        result_1 += result;
    }

    result_2 = calculate_number_of_scratchcards(combination_values);

    //Answer Part 1 - 28750
    //Answer Part 2 - 10212704
    std::cout << "Result 1: " << result_1 << std::endl;
    std::cout << "Result 2: " << result_2 << std::endl;
}

std::vector<int> get_numbers_from_string(const std::string& string)
{
    std::vector<int> result {};
    const std::regex pattern("\\s{2,}");
    std::stringstream ss { std::regex_replace(string, pattern, " ").erase(0, 1) };
    std::string token {};
    while (std::getline(ss, token, ' ')) {
        result.push_back(std::stoi(token));
    }

    return result;
}

std::pair<std::size_t, std::size_t> check_combination_value(const std::vector<int>& winning, const std::vector<int>& played)
{
    std::size_t result { 0 };
    std::size_t hits { 0 };

    for (int number : played)
    {
        auto it { std::find(winning.begin(), winning.end(), number) };
        if (it != winning.cend())
        {
            ++hits;
            if (result == 0)
            {
                result = 1;
            }
            else
            {
                result <<= 1;
            }
        }
    }

    return std::make_pair(result, hits);
}

std::size_t calculate_number_of_scratchcards(const std::vector<std::size_t>& results)
{
    std::size_t result { 0 };
    std::vector<std::size_t> scratchcards {};
    scratchcards.resize(results.size());
    std::fill(scratchcards.begin(), scratchcards.end(), 1);

    std::size_t runs { 1 };
    for (std::size_t i { 0 }; i < results.size() - 1; ++i)
    {
        for (std::size_t j { 0 }; j < runs; ++j)
        {
            for (std::size_t k { 1 }; k <= results.at(i); ++k)
            {
                ++scratchcards.at(i + k);
            }
        }
        runs = scratchcards.at(i + 1);
    }

    for (std::size_t e : scratchcards)
    {
        result += e;
    }

    return result;
}
