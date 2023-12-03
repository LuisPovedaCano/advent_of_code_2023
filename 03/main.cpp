#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Number
{
    std::size_t line_number { 0 };
    std::size_t starting_position { 0 };
    std::size_t end_position { 0 };
    std::size_t number { 0 };
};

bool check_line(Number number, const std::string& line);
void get_line_numbers(const std::size_t line_number, const std::string& line, std::vector<Number>& numbers);
std::vector<Number> filter_valid_numbers(const std::vector<Number>& numbers, const std::vector<std::string>& lines);

int main(int argc, char* argv[])
{
    //if (argc != 2)
    //{
    //    std::cerr << "Path to the puzzle file must be provided" << std::endl;
    //    std::terminate();
    //}

    //std::ifstream file(argv[1]);
    //std::ifstream file("../data/aoc_2023_03.txt");
    std::ifstream file("test.txt");
    if (!file.good())
    {
        std::cerr << "Failed to read the file" << std::endl;
        std::terminate();
    }

    std::size_t result { 0 };
    std::string line {};
    std::size_t line_number { 0 };
    std::vector<std::string> lines {};
    std::vector<Number> numbers {};
    while (std::getline(file, line))
    {
        get_line_numbers(line_number, line, numbers);
        ++line_number;
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }

    std::cout << "Numbers found: " << numbers.size() << std::endl;
    std::vector<Number> valid_numbers = filter_valid_numbers(numbers, lines);
    std::cout << "Valid numbers found: " << valid_numbers.size() << std::endl;
    for (Number element : valid_numbers)
    {
        result += element.number;
    }
    //Answer Part 1 - 498559
    //Answer Part 2 -
    std::cout << "Result: " << result << std::endl;
}

void get_line_numbers(const std::size_t line_number, const std::string& line, std::vector<Number>& numbers)
{
    Number number {};
    bool writing_digits { false };
    std::size_t multiplier { 1 };
    for (auto it = line.crbegin(); it != line.crend(); ++it)
    {
        if (std::isdigit(*it))
        {
            if (!writing_digits)
            {
                number.end_position = line.length() - std::distance(line.crbegin(), it) - 1;
                writing_digits = true;
            }
            number.number += (*it - '0') * multiplier;
            multiplier *= 10;
        }
        else
        {
            if (writing_digits)
            {
                number.starting_position = line.length() - std::distance(line.crbegin(), it);
                number.line_number = line_number;
                numbers.push_back(number);
                number = {};
                writing_digits = false;
                multiplier = 1;
            }
        }
    }

    if (writing_digits)
    {
        number.line_number = line_number;
        number.starting_position = 0;
        numbers.push_back(number);
    }
}

std::vector<Number> filter_valid_numbers(const std::vector<Number>& numbers, const std::vector<std::string>& lines)
{
    std::vector<Number> valid_numbers {};
    const std::size_t line_size { lines.at(0).length() - 1 };
    for (Number element : numbers)
    {
        //Left side
        if (element.starting_position > 0)
        {
            if (element.line_number > 0)
            {
                const char symbol = lines.at(element.line_number - 1).at(element.starting_position - 1);
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            if (element.line_number < lines.size() - 1)
            {
                const char symbol = lines.at(element.line_number + 1).at(element.starting_position - 1);
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            char symbol = lines.at(element.line_number).at(element.starting_position - 1);
            if (!std::isdigit(symbol) && symbol != '.')
            {
                valid_numbers.push_back(element);
                continue;
            }
        }

        //Right side
        if (element.end_position < line_size)
        {
            if (element.line_number > 0)
            {
                const char symbol = lines.at(element.line_number - 1).at(element.end_position + 1);
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            if (element.line_number < lines.size() - 1)
            {
                const char symbol = lines.at(element.line_number + 1).at(element.end_position + 1);
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            const char symbol = lines.at(element.line_number).at(element.end_position + 1);
            if (!std::isdigit(symbol) && symbol != '.')
            {
                valid_numbers.push_back(element);
                continue;
            }
        }

        bool is_valid { false };
        if (element.line_number > 0)
        {
            is_valid = check_line(element, lines.at(element.line_number - 1));
        }
        if (is_valid)
        {
            valid_numbers.push_back(element);
        }
        else
        {
            if(element.line_number < lines.size() - 1)
            {
                is_valid = check_line(element, lines.at(element.line_number + 1));
                if (is_valid)
                {
                    valid_numbers.push_back(element);
                }
            }
        }
    }

    return valid_numbers;
}

bool check_line(Number number, const std::string& line)
{
    for (std::size_t i = number.starting_position; i <= number.end_position; ++i)
    {
        const char symbol = line.at(i);
        if (!std::isdigit(symbol) && symbol != '.') return true;
    }

    return false;
}
