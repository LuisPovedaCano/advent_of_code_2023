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

struct Gear
{
    std::size_t line_number { 0 };
    std::size_t position { 0 };
    std::size_t number_1 { 0 };
    std::size_t number_2 { 0 };
};

bool check_line(const std::size_t line_number, Number number, const std::string& line, std::vector<Gear>& gears);
void get_line_numbers(const std::size_t line_number, const std::string& line, std::vector<Number>& numbers);
std::vector<Number> filter_valid_numbers(const std::vector<Number>& numbers, const std::vector<std::string>& lines, std::vector<Gear>& gears);

std::vector<Gear> filter_valid_gears(const std::vector<Gear>& gears);

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

    std::vector<Gear> gears {};
    std::vector<Number> valid_numbers = filter_valid_numbers(numbers, lines, gears);
    std::vector<Gear> valid_gears = filter_valid_gears(gears);

    for (Number element : valid_numbers)
    {
        result_1 += element.number;
    }
    for (Gear element : valid_gears)
    {
        result_2 += element.number_1 * element.number_2;
    }

    //Answer Part 1 - 498559
    //Answer Part 2 - 72246648
    std::cout << "Result 1: " << result_1 << std::endl;
    std::cout << "Result 2: " << result_2 << std::endl;
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

std::vector<Number> filter_valid_numbers(const std::vector<Number>& numbers, const std::vector<std::string>& lines, std::vector<Gear>& gears)
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
                const std::size_t line_number { element.line_number - 1 };
                const std::size_t position { element.starting_position - 1 };
                const char symbol { lines.at(line_number).at(position) };
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    if (symbol == '*')
                    {
                        Gear gear;
                        gear.line_number = line_number;
                        gear.position = position;
                        gear.number_1 = element.number;
                        gears.push_back(gear);
                    }
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            if (element.line_number < lines.size() - 1)
            {
                const std::size_t line_number { element.line_number + 1 };
                const std::size_t position { element.starting_position - 1 };
                const char symbol { lines.at(line_number).at(position) };
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    if (symbol == '*')
                    {
                        Gear gear;
                        gear.line_number = line_number;
                        gear.position = position;
                        gear.number_1 = element.number;
                        gears.push_back(gear);
                    }
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            const std::size_t line_number { element.line_number };
            const std::size_t position { element.starting_position - 1 };
            char symbol = lines.at(line_number).at(position);
            if (!std::isdigit(symbol) && symbol != '.')
            {
                if (symbol == '*')
                {
                    Gear gear;
                    gear.line_number = line_number;
                    gear.position = position;
                    gear.number_1 = element.number;
                    gears.push_back(gear);

                }
                valid_numbers.push_back(element);
                continue;
            }
        }

        //Right side
        if (element.end_position < line_size)
        {
            if (element.line_number > 0)
            {
                const std::size_t line_number { element.line_number - 1 };
                const std::size_t position { element.end_position + 1 };
                const char symbol = lines.at(line_number).at(position);
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    if (symbol == '*')
                    {
                        Gear gear;
                        gear.line_number = line_number;
                        gear.position = position;
                        gear.number_1 = element.number;
                        gears.push_back(gear);
                    }
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            if (element.line_number < lines.size() - 1)
            {
                const std::size_t line_number { element.line_number + 1 };
                const std::size_t position { element.end_position + 1 };
                const char symbol = lines.at(line_number).at(position);
                if (!std::isdigit(symbol) && symbol != '.')
                {
                    if (symbol == '*')
                    {
                        Gear gear;
                        gear.line_number = line_number;
                        gear.position = position;
                        gear.number_1 = element.number;
                        gears.push_back(gear);
                    }
                    valid_numbers.push_back(element);
                    continue;
                }
            }

            const std::size_t line_number { element.line_number };
            const std::size_t position { element.end_position + 1 };
            const char symbol = lines.at(line_number).at(position);
            if (!std::isdigit(symbol) && symbol != '.')
            {
                if (symbol == '*')
                {
                    Gear gear;
                    gear.line_number = line_number;
                    gear.position = position;
                    gear.number_1 = element.number;
                    gears.push_back(gear);
                }
                valid_numbers.push_back(element);
                continue;
            }
        }

        bool is_valid { false };
        if (element.line_number > 0)
        {
            const std::size_t line_number { element.line_number - 1 };
            is_valid = check_line(line_number, element, lines.at(line_number), gears);
        }
        if (is_valid)
        {
            valid_numbers.push_back(element);
        }
        else
        {
            if(element.line_number < lines.size() - 1)
            {
                const std::size_t line_number { element.line_number + 1 };
                is_valid = check_line(line_number, element, lines.at(line_number), gears);
                if (is_valid)
                {
                    valid_numbers.push_back(element);
                }
            }
        }
    }

    return valid_numbers;
}

bool check_line(const std::size_t line_number, Number number, const std::string& line, std::vector<Gear>& gears)
{
    for (std::size_t i { number.starting_position }; i <= number.end_position; ++i)
    {
        const char symbol = line.at(i);
        if (!std::isdigit(symbol) && symbol != '.')
        {
            if (symbol == '*')
            {
                Gear gear;
                gear.line_number = line_number;
                gear.position = i;
                gear.number_1 = number.number;
                gears.push_back(gear);
            }
            return true;
        }
    }

    return false;
}

std::vector<Gear> filter_valid_gears(const std::vector<Gear>& gears)
{
    std::vector<Gear> valid_gears {};
    for (auto i { gears.begin() }; i != gears.end() - 1; ++i)
    {
        for (auto j { i + 1 }; j != gears.end(); ++j)
        {
            if (i->line_number == j->line_number && i->position == j->position && i->number_1 != j->number_1)
            {
                Gear gear = *i;
                gear.number_2 = j->number_1;
                valid_gears.push_back(gear);
            }
        }
    }
    return valid_gears;
}
