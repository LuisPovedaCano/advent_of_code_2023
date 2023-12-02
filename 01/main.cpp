#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

struct ValueIndex
{
    std::size_t value { 0 };
    std::size_t index { 0 };
};

std::size_t get_line_result(ValueIndex df, ValueIndex dl, ValueIndex wf, ValueIndex wl);
std::pair<ValueIndex, ValueIndex> get_first_and_last_digits(const std::string& line);
std::pair<ValueIndex, ValueIndex> get_first_and_last_written_digits(const std::string& line);

int main(int argc, char *argv[])
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

    std::size_t result { 0 };
    std::string line {};
    while (std::getline(file, line))
    {
        ValueIndex digit_first {};
        ValueIndex digit_last {};
        std::tie(digit_first, digit_last) = get_first_and_last_digits(line);

        ValueIndex written_first {};
        ValueIndex written_last {};
        std::tie(written_first, written_last) = get_first_and_last_written_digits(line);

        result += get_line_result(digit_first, digit_last, written_first, written_last);;
    }

    //Answer Part 1 - 54697
    //Answer Part 2 - 54885
    std::cout << "Result: " << result << std::endl;
}

std::size_t get_line_result(ValueIndex df, ValueIndex dl, ValueIndex wf, ValueIndex wl)
{
    ValueIndex min {};
    if (df.value == 0)
    {
        min = wf;
    }
    else if (wf.value == 0)
    {
        min = df;
    }
    else
    {
        min = df.index < wf.index ? df : wf;
    }

    ValueIndex max {};
    if (dl.value == 0)
    {
        max = wl;
    }
    else if (wl.value == 0)
    {
        max = dl;
    }
    else
    {
        max = dl.index > wl.index ? dl : wl;
    }

    return  min.value * 10 + max.value;
}

std::pair<ValueIndex, ValueIndex> get_first_and_last_digits(const std::string& line)
{
    ValueIndex first {};
    ValueIndex last {};

    for (auto it = line.cbegin(); it != line.cend(); ++it)
    {
        if (std::isdigit(*it))
        {
            if (first.value == 0)
            {
                first.value = *it - '0';
                first.index = std::distance(line.cbegin(), it);
            }
            else
            {
                last.value = *it - '0';
                last.index = std::distance(line.cbegin(), it);
            }
        }
    }

    if (last.value == 0)
    {
        last.value = first.value;
        last.index = first.index;
    }

    return std::make_pair(first, last);
}

std::pair<ValueIndex, ValueIndex> get_first_and_last_written_digits(const std::string& line)
{
    const std::array<std::string, 9> digits
    {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    ValueIndex first {};
    ValueIndex last {};

    for (size_t i = 0; i < digits.size(); ++i)
    {
        std::size_t index{ line.find(digits.at(i)) };
        while(index != std::string::npos)
        {
            if (first.value == 0)
            {
                first.value = i + 1;
                first.index = index;
            }
            else if (last.value == 0)
            {
                last.value = i + 1;
                last.index = index;

                if (first.index > last.index)
                {
                    std::swap(first, last);
                }
            }
            else
            {
                if (index < first.index)
                {
                    first.value = i + 1;
                    first.index = index;
                }
                if (index > last.index)
                {
                    last.value = i + 1;
                    last.index = index;
                }
            }
            index = line.find(digits.at(i), index + 1);
        };
    }

    if (last.value == 0)
    {
        last.value = first.value;
        last.index = first.index;
    }

    return std::make_pair(first, last);
}
