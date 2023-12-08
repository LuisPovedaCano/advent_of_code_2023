#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Step
{
    std::string id {};
    std::string left {};
    std::string right {};
};

Step get_step_by_id(const std::string& id, const std::vector<Step>& steps);
std::size_t count_steps(const std::string& path, const std::vector<Step>& steps);

int main (int argc, char* argv[])
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

    std::string path {};
    std::getline(file, path);
    std::string line {};
    std::vector<Step> steps {};
    while(std::getline(file, line))
    {
        if (line.empty()) continue;
        std::stringstream ss { line };
        Step step {};
        std::getline(ss, step.id, ' ');
        std::getline(ss, step.left, ',');
        std::getline(ss, step.right);
        step.left.erase(0, 3);
        step.right.erase(0, 1);
        step.right.erase(step.right.size() - 1, 1);
        steps.push_back(step);
    }

    std::size_t result_1 { count_steps(path, steps) };
    std::size_t result_2 { 0 };

    file.close();

    //Answer Part 1 -
    //Answer Part 2 -
    std::cout << "Result 1: " << result_1 << std::endl;
    std::cout << "Result 2: " << result_2 << std::endl;
}

std::size_t count_steps(const std::string& path, const std::vector<Step>& steps)
{
    std::size_t result { 0 };
    Step step { get_step_by_id("AAA", steps) };
    for (;;)
    {
        for (char next : path)
        {
            if (step.id == "ZZZ")
            {
                return result;
            }
            if (next == 'L')
            {
                step = get_step_by_id(step.left, steps);
            }
            else
            {
                step = get_step_by_id(step.right, steps);
            }
            if (step.id.empty())
            {
                std::cerr << "Invalid Step" << std::endl;
                std::terminate();
            }
            ++result;
        }
    }

    return result;
}

Step get_step_by_id(const std::string& id, const std::vector<Step>& steps)
{
    for (Step step : steps)
    {
        if (step.id == id)
        {
            return step;
        }
    }
    return {};
}
