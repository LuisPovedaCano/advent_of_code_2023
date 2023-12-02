#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct GameInfo
{
    int red { 0 };
    int green { 0 };
    int blue { 0 };
};

struct Game
{
    std::size_t id { 0 };
    std::vector<GameInfo> records {};
    GameInfo min_cubes {};
};

int calculate_min_cubes(Game& game);
Game get_game_from_line(const std::string& line);
bool validate_game(const Game& bags);

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
    int power { 0 };
    std::string line {};
    while (std::getline(file, line))
    {
        Game game = get_game_from_line(line);
        power += calculate_min_cubes(game);
        if (validate_game(game))
        {
            result += game.id;
        }
    }

    //Answer Part 1 - 2632
    std::cout << "Result 1: " << result << std::endl;
    //Answer Part 2 - 69629
    std::cout << "Result 2: " << power << std::endl;
}

int calculate_min_cubes(Game& game)
{
    for (const GameInfo record : game.records)
    {
        if (game.min_cubes.red < record.red) game.min_cubes.red = record.red;
        if (game.min_cubes.green < record.green) game.min_cubes.green = record.green;
        if (game.min_cubes.blue < record.blue) game.min_cubes.blue = record.blue;
    }

    return game.min_cubes.red * game.min_cubes.green * game.min_cubes.blue;
}

Game get_game_from_line(const std::string& line)
{
    static std::size_t game_id { 0 };
    Game game;
    game.id = ++game_id;

    std::stringstream ss { line };
    std::vector<GameInfo> tokens {};
    std::string token {};
    //skip the first two tokens
    std::getline(ss, token, ' ');
    std::getline(ss, token, ' ');
    GameInfo record {};
    while (std::getline(ss, token, ' ')) {
        const std::string number{token};
        std::getline(ss, token, ' ');
        const std::string color{token};
        switch (color.at(0))
        {
            case 'r': record.red += std::stoi(number); break;
            case 'g': record.green += std::stoi(number); break;
            case 'b': record.blue += std::stoi(number); break;
        }
        if (*(color.crbegin()) == ',') continue;

        game.records.push_back(record);
        record = {};
    }
    return game;
}

bool validate_game(const Game& game)
{
    const int max_red_cubes { 12 };
    const int max_green_cubes { 13 };
    const int max_blue_cubes { 14 };

    for (const GameInfo record : game.records)
    {
        if (record.red > max_red_cubes) return false;
        if (record.green > max_green_cubes) return false;
        if (record.blue > max_blue_cubes) return false;
    }

    return true;
}
