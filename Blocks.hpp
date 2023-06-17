#ifndef BLOCKS_H
#define BLOCKS_H

#include "Colors.hpp"
#include "Pixel.hpp"
#include "Items.hpp"


class Block {
    protected:
        const bool passable;
        const std::string text;
        const Color color;
    public:
        Block(bool passable=false, std::string text="", Color color=Colors::Default): passable{passable}, text{text}, color{color} {
        }
        virtual ~Block() {
        }
        virtual bool isPassable() {
            return this->passable;
        }
        virtual Pixel getPixel() {
            return Pixel{this->text, this->color};
        }
};

class Air: public Block {
    private:
        Item* item;
    public:
        Air(Color color=Colors::Air): Block{true, "", color}, item{nullptr} {
        }
        virtual ~Air() {
            delete this->item;
        }
        void setItem(Item* item) {
            this->item = item;
        }
        bool hasItem() {
            return this->item != nullptr;
        }
        Item* getItem() {
            return this->item;
        }
        void removeItem() {
            delete this->item;
            this->item = nullptr;
        }
        Pixel getPixel() {
            if (this->hasItem())
                return this->getItem()->getPixel();
            return Pixel{this->text, this->color};
        }
};

class Wall: public Block {
    public:
        Wall(): Block{false, "", Colors::Wall} {
        }
};

class ImmuneWall: public Block {
    public:
        ImmuneWall(): Block{false, "", Colors::ImmuneWall} {
        }
};

#endif