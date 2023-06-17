#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <string>

#include "Position.hpp"
#include "Colors.hpp"
#include "Pixel.hpp"


class DisplayConfig {
    public:
        static const int X_SCALE = 3;
        static const int Y_SCALE = 1;
};


class Window {
    private:
        WINDOW* window;
    public:
        const int xSize;
        const int ySize;
        Window(int xPos, int yPos, int xSize, int ySize): xSize{xSize}, ySize{ySize}, window{newwin(ySize * DisplayConfig::Y_SCALE, xSize * DisplayConfig::X_SCALE, yPos * DisplayConfig::Y_SCALE, xPos * DisplayConfig::X_SCALE)} {
        }
        Window(Window* parent, int xPos, int yPos, int xSize, int ySize): xSize{xSize}, ySize{ySize}, window{derwin(parent->window, ySize * DisplayConfig::Y_SCALE, xSize * DisplayConfig::X_SCALE, yPos * DisplayConfig::Y_SCALE, xPos * DisplayConfig::X_SCALE)} {
        }
        virtual ~Window() {
            delwin(this->window);
        }
        void showBox() {
            box(this->window, 0, 0);
        }
        void print(Position position, std::string text, Color color) {
            wattron(this->window, COLOR_PAIR(color.id));
            mvwprintw(this->window, position.y * DisplayConfig::Y_SCALE, position.x * DisplayConfig::X_SCALE, "%s", text.c_str());
        }
        void printPixel(Position position, Pixel pixel) {
            wattron(this->window, COLOR_PAIR(pixel.getColor().id));
            std::string text = pixel.getText();
            if (text.length() >= DisplayConfig::X_SCALE) {
                text = text.substr(0, DisplayConfig::X_SCALE);
            }
            else {
                int padding = DisplayConfig::X_SCALE - text.length();
                text = std::string(padding - (padding / 2), ' ') + text + std::string(padding / 2, ' ');
            }
            mvwprintw(this->window, position.y * DisplayConfig::Y_SCALE, position.x * DisplayConfig::X_SCALE, "%s", text.c_str());
        }
        void printCenter(int yPos, std::string text, Color color) {
            this->print(Position{(this->xSize - ((int)text.length() / DisplayConfig::X_SCALE)) / 2, yPos}, text, color);
        }
        void clear() {
            werase(this->window);
        }
        void refresh() {
            wrefresh(this->window);
        }
};


class Display {
    public:
        Window* frame;
        Window* world;
        Window* stats;
        Window* mission;
        Display(int worldX, int worldY) {
            initscr();
            curs_set(0);
            cbreak();
            keypad(stdscr, TRUE);
            nodelay(stdscr, TRUE);
            noecho();
            start_color();
            for (auto it = Colors::COLOR_LIST.begin(); it != Colors::COLOR_LIST.end(); it++) {
                init_pair(it->id, it->color, it->backgroundColor);
            }
            this->frame = new Window{0, 0, worldX + 12, worldY + 4};
            this->frame->printCenter(1, "Snake Game", Colors::Text);
            this->frame->printCenter(this->frame->ySize - 2, "20203123 JiWon Lee", Colors::Text);
            this->world = new Window{this->frame, 1, 2, worldX, worldY};
            this->stats = new Window{this->frame, worldX + 1, 2, this->frame->xSize - worldX - 2, worldY / 2 + 2};
            this->mission = new Window{this->frame, worldX + 1, this->stats->ySize + 2, this->frame->xSize - worldX - 2, worldY - this->stats->ySize};
            this->frame->showBox();
            refresh();
        }
        virtual ~Display() {
            endwin();
        }
};

#endif