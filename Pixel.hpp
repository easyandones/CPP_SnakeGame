#ifndef PIXEL_H
#define PIXEL_H

#include <string>

#include "Colors.hpp"


class Pixel {
    private:
        std::string text;
        Color color;
    public:
        Pixel(std::string text, Color color): text{text}, color{color} {
        }
        const std::string getText() {
            return this->text;
        }
        const Color getColor() {
            return this->color;
        }
        bool operator==(Pixel& pixel) {
            return this->text == pixel.text && this->color == pixel.color;
        }
        bool operator!=(Pixel& pixel) {
            return !(*this == pixel);
        }
};

#endif