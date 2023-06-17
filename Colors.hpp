#ifndef COLORS_H
#define COLORS_H

#include <vector>


class Palette {
    private:
        static const int BRIGHT = 8;
    public:
        static const int BLACK = 0;
        static const int RED = 1;
        static const int GREEN = 2;
        static const int YELLOW = 3;
        static const int BLUE = 4;
        static const int MAGENTA = 5;
        static const int CYAN = 6;
        static const int WHITE = 7;
        static const int BRIGHT_BLACK = BLACK + BRIGHT;
        static const int BRIGHT_RED = RED + BRIGHT;
        static const int BRIGHT_GREEN = GREEN + BRIGHT;
        static const int BRIGHT_YELLOW = YELLOW + BRIGHT;
        static const int BRIGHT_BLUE = BLUE + BRIGHT;
        static const int BRIGHT_MAGENTA = MAGENTA + BRIGHT;
        static const int BRIGHT_CYAN = CYAN + BRIGHT;
        static const int BRIGHT_WHITE = WHITE + BRIGHT;
};

class Color {
    static int idCount;
    public:
        int id;
        int color;
        int backgroundColor;
        Color(int color, int backgroundColor): id{idCount++}, color{color}, backgroundColor{backgroundColor} {
        }
        bool operator==(Color& color) {
            return this->id == color.id;
        }
        bool operator!=(Color& color) {
            return !(*this == color);
        }
};
int Color::idCount = 0;

class Colors {
    public:
        static const Color Default;
        static const Color Text;
        static const Color Text_Green;
        static const Color Text_Red;
        static const Color Air;
        static const Color Wall;
        static const Color ImmuneWall;
        static const Color Growth;
        static const Color Poison;
        static const Color Gate;
        static const Color SnakeHead;
        static const Color Snake;
        static const std::vector<Color> COLOR_LIST;
};

const Color Colors::Default{Palette::YELLOW, Palette::GREEN};
const Color Colors::Text{Palette::BRIGHT_WHITE, Palette::BLACK};
const Color Colors::Text_Green{Palette::BRIGHT_GREEN, Palette::BLACK};
const Color Colors::Text_Red{Palette::BRIGHT_RED, Palette::BLACK};
const Color Colors::Air{Palette::WHITE, Palette::BRIGHT_WHITE};
const Color Colors::Wall{Palette::WHITE, Palette::BRIGHT_BLACK};
const Color Colors::ImmuneWall{Palette::WHITE, Palette::CYAN};
const Color Colors::Growth{Palette::WHITE, Palette::BRIGHT_BLUE};
const Color Colors::Poison{Palette::WHITE, Palette::RED};
const Color Colors::Gate{Palette::WHITE, Palette::MAGENTA};
const Color Colors::SnakeHead{Palette::BRIGHT_WHITE, Palette::BRIGHT_RED};
const Color Colors::Snake{Palette::BRIGHT_WHITE, Palette::BRIGHT_GREEN};
const std::vector<Color> Colors::COLOR_LIST{Default, Text, Text_Green, Text_Red, Air, Wall, ImmuneWall, Growth, Poison, Gate, SnakeHead, Snake};

#endif