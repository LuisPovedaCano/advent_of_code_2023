#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct MapQuery
{
    std::size_t destination { 0 };
    std::size_t source { 0 };
    std::size_t range { 0 };
};

struct SeedInfo
{
    std::size_t seed { 0 };
    std::size_t soil { 0 };
    std::size_t fertilizer { 0 };
    std::size_t water { 0 };
    std::size_t light { 0 };
    std::size_t temperature { 0 };
    std::size_t humidity { 0 };
    std::size_t location { 0 };
};

SeedInfo query_seed(const std::size_t seed, const std::array<std::vector<MapQuery>, 7>& map);
std::vector<std::size_t> get_seeds(const std::string& line);
std::array<std::vector<MapQuery>, 7> parse_data(std::ifstream& file);

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

    std::size_t result_1 { 0 };
    std::size_t result_2 { 0 };

    std::vector<std::size_t> seeds {};
    std::array<std::vector<MapQuery>, 7> map {};
    {
        std::string line {};
        std::getline(file, line);
        seeds = get_seeds(line);
        map = parse_data(file);
    }
    file.close();

    result_1 = std::numeric_limits<std::size_t>::max();
    for (const std::size_t seed : seeds)
    {
        SeedInfo info { query_seed(seed, map) };
        if (static_cast<std::size_t>(info.location) < result_1)
        {
            result_1 = info.location;
        }
    }

    result_2 = std::numeric_limits<std::size_t>::max();
    for (std::size_t i { 0 }; i < seeds.size(); i+=2)
    {
        for (std::size_t j { seeds.at(i) }; j < seeds.at(i) + seeds.at(i + 1); ++j)
        {
            SeedInfo info { query_seed(j, map) };
            if (static_cast<std::size_t>(info.location) < result_2)
            {
                result_2 = info.location;
            }
        }
    }

    //Answer Part 1 - 196167384
    //Answer Part 2 - 125742456
    std::cout << "Result 1: " << result_1 << std::endl;
    std::cout << "Result 2: " << result_2 << std::endl;
}

std::vector<std::size_t> get_seeds(const std::string& line)
{
    std::vector<std::size_t> seeds {};
    std::stringstream ss { line };
    std::string token {};
    //skip the first token
    std::getline(ss, token, ' ');
    while (std::getline(ss, token, ' ')) {
        seeds.push_back(static_cast<std::size_t>(std::stoull(token)));
    }

    return seeds;
}

std::array<std::vector<MapQuery>, 7> parse_data(std::ifstream& file)
{
    std::array<std::vector<MapQuery>, 7> maps {};
    std::string line {};
    std::getline(file, line, ':');
    std::size_t index { 0 };
    while (std::getline(file, line, ':'))
    {
        line.erase(0, 1);
        std::stringstream ss { line };
        std::string token {};
        while (std::getline(ss, token))
        {
            if (!token.empty() && std::isdigit(token.at(0)))
            {
                std::stringstream ss_token { token };
                MapQuery map;
                std::string value {};
                std::getline(ss_token, value, ' ');
                map.destination = static_cast<std::size_t>(std::stoull(value));
                std::getline(ss_token, value, ' ');
                map.source = static_cast<std::size_t>(std::stoull(value));
                std::getline(ss_token, value, ' ');
                map.range = static_cast<std::size_t>(std::stoull(value));
                maps[index].push_back(map);
                map = {};
            }
        }

        index += 1;
    }

    return maps;
}

SeedInfo query_seed(const std::size_t seed, const std::array<std::vector<MapQuery>, 7>& map)
{
    SeedInfo result {};
    result.seed = seed;
    std::size_t current_id { seed };
    std::size_t map_index { 0 };
    for (const std::vector<MapQuery>& type : map)
    {
        switch (map_index)
        {
            case 0: current_id = result.seed; break;
            case 1: current_id = result.soil; break;
            case 2: current_id = result.fertilizer; break;
            case 3: current_id = result.water; break;
            case 4: current_id = result.light; break;
            case 5: current_id = result.temperature; break;
            case 6: current_id = result.humidity; break;
        }
        for (const MapQuery& query : type)
        {
            if (current_id >= query.source && current_id < query.source + query.range)
            {
                const std::size_t destination { query.destination + (current_id - query.source)};
                switch (map_index)
                {
                    case 0: result.soil = destination; break;
                    case 1: result.fertilizer = destination; break;
                    case 2: result.water = destination; break;
                    case 3: result.light = destination; break;
                    case 4: result.temperature = destination; break;
                    case 5: result.humidity = destination; break;
                    case 6: result.location = destination; break;
                }
                break;
            }
            switch (map_index)
            {
                case 0: result.soil = current_id; break;
                case 1: result.fertilizer = current_id; break;
                case 2: result.water = current_id; break;
                case 3: result.light = current_id; break;
                case 4: result.temperature = current_id; break;
                case 5: result.humidity = current_id; break;
                case 6: result.location = current_id; break;
            }
        }
        ++map_index;
    }

    return result;
}
