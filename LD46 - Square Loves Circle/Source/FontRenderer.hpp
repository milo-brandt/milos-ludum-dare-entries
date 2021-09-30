//
//  FontRenderer.hpp
//  Dummy
//
//  Created by Milo Brandt on 4/19/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef FontRenderer_hpp
#define FontRenderer_hpp

#include "Common/Window.h"
#include <string>

namespace FontRenderer{
    struct CharacterFormat{
        short width = 0;
        short offsetX = 0;
        short offsetY = 0;
        unsigned short rectX = 0;
        unsigned short rectY = 0;
        short rectWidth = 0;
        short rectHeight = 0;
    };
    struct FontMap{
        struct Specification{
            char character;
            CharacterFormat form;
        };
        const char* filename;
        unsigned int pixelHeight;
        unsigned int totalWidth;
        unsigned int totalHeight;
        short minYOffset = 0;
        short maxYBottom = 0;
        short maxHeight = 0;
        CharacterFormat format[256];
        constexpr FontMap(const char* filename, unsigned int pixelHeight, unsigned int totalWidth, unsigned int totalHeight, std::initializer_list<Specification> specs):filename(filename),pixelHeight(pixelHeight), totalWidth(totalWidth), totalHeight(totalHeight){
            for(Specification const& s : specs){
                format[(unsigned char)s.character] = s.form;
                if(s.form.offsetY < minYOffset) minYOffset = s.form.offsetY;
                if(s.form.offsetY + s.form.rectHeight > maxYBottom) maxYBottom = s.form.offsetY + s.form.rectHeight;
            }
            maxHeight = maxYBottom - minYOffset;
        }
        CharacterFormat const& operator[](char c) const{
            return format[(unsigned char)c];
        }
    };
    constexpr FontMap openSans = {"", 40, 512, 512, {{' ', {18, -2, 74, 1, 55, 4, 4}},
        {'!', {19, 3, 23, 6, 4, 13, 56}},
        {'"', {28, 2, 23, 20, 4, 24, 22}},
        {'#', {46, -1, 23, 45, 4, 48, 55}},
        {'$', {41, 2, 20, 94, 1, 37, 62}},
        {'%', {58, 1, 23, 132, 4, 56, 56}},
        {'&', {52, 1, 23, 189, 4, 53, 56}},
        {'\'', {16, 2, 23, 243, 4, 12, 22}},
        {'(', {21, 0, 23, 256, 4, 21, 66}},
        {')', {21, 0, 23, 278, 4, 21, 66}},
        {'*', {39, 0, 20, 300, 1, 38, 36}},
        {'+', {41, 1, 32, 339, 13, 39, 39}},
        {',', {17, 0, 66, 379, 47, 15, 21}},
        {'-', {23, 0, 52, 395, 33, 23, 10}},
        {'.', {19, 3, 65, 419, 46, 13, 14}},
        {'/', {26, -2, 23, 433, 4, 30, 55}},
        {'0', {41, 1, 23, 464, 4, 39, 56}},
        {'1', {41, 4, 23, 1, 71, 23, 55}},
        {'2', {41, 1, 23, 25, 71, 39, 55}},
        {'3', {41, 1, 23, 65, 71, 38, 56}},
        {'4', {41, -1, 23, 104, 71, 43, 55}},
        {'5', {41, 2, 23, 148, 71, 37, 56}},
        {'6', {41, 2, 23, 186, 71, 38, 56}},
        {'7', {41, 1, 23, 225, 71, 39, 55}},
        {'8', {41, 1, 23, 265, 71, 39, 56}},
        {'9', {41, 1, 23, 305, 71, 39, 56}},
        {':', {19, 3, 35, 345, 83, 13, 44}},
        {';', {19, 0, 35, 359, 83, 16, 52}},
        {'<', {41, 1, 31, 376, 79, 39, 39}},
        {'=', {41, 2, 39, 416, 87, 37, 23}},
        {'>', {41, 1, 31, 454, 79, 39, 39}},
        {'?', {30, -2, 23, 1, 136, 33, 56}},
        {'@', {64, 2, 23, 35, 136, 60, 61}},
        {'A', {45, -2, 23, 96, 136, 49, 55}},
        {'B', {46, 4, 23, 146, 136, 41, 55}},
        {'C', {45, 2, 23, 188, 136, 43, 56}},
        {'D', {52, 5, 23, 232, 136, 45, 55}},
        {'E', {39, 4, 23, 278, 136, 33, 55}},
        {'F', {37, 5, 23, 312, 136, 33, 55}},
        {'G', {52, 2, 23, 346, 136, 47, 56}},
        {'H', {52, 4, 23, 394, 136, 44, 55}},
        {'I', {20, 5, 23, 439, 136, 10, 55}},
        {'J', {19, -8, 23, 450, 136, 23, 68}},
        {'K', {44, 5, 23, 1, 205, 41, 55}},
        {'L', {37, 5, 23, 43, 205, 33, 55}},
        {'M', {64, 4, 23, 77, 205, 56, 55}},
        {'N', {54, 5, 23, 134, 205, 44, 55}},
        {'O', {55, 2, 23, 179, 205, 51, 56}},
        {'P', {43, 5, 23, 231, 205, 37, 55}},
        {'Q', {55, 2, 23, 269, 205, 51, 67}},
        {'R', {44, 4, 23, 321, 205, 41, 55}},
        {'S', {39, 1, 23, 363, 205, 37, 56}},
        {'T', {39, -2, 23, 401, 205, 43, 55}},
        {'U', {52, 4, 23, 445, 205, 44, 56}},
        {'V', {42, -2, 23, 1, 276, 46, 55}},
        {'W', {66, -2, 23, 48, 276, 70, 55}},
        {'X', {41, -2, 23, 119, 276, 45, 55}},
        {'Y', {40, -2, 23, 165, 276, 44, 55}},
        {'Z', {41, 0, 23, 210, 276, 41, 55}},
        {'[', {23, 3, 23, 252, 276, 21, 66}},
        {'\\', {26, -2, 23, 274, 276, 30, 55}},
        {']', {23, -1, 23, 305, 276, 21, 66}},
        {'^', {38, -1, 23, 327, 276, 40, 36}},
        {'_', {32, -3, 80, 368, 333, 38, 9}},
        {'`', {41, 11, 20, 407, 273, 19, 15}},
        {'a', {39, 1, 35, 427, 288, 35, 44}},
        {'b', {44, 4, 20, 463, 273, 38, 59}},
        {'c', {34, 2, 35, 1, 358, 32, 44}},
        {'d', {44, 2, 20, 34, 343, 38, 59}},
        {'e', {40, 2, 35, 73, 358, 37, 44}},
        {'f', {24, -1, 20, 111, 343, 31, 58}},
        {'g', {39, -1, 35, 143, 358, 41, 60}},
        {'h', {44, 4, 20, 185, 343, 37, 58}},
        {'i', {18, 3, 22, 223, 345, 12, 56}},
        {'j', {18, -6, 22, 236, 345, 21, 73}},
        {'k', {37, 4, 20, 258, 343, 35, 58}},
        {'l', {18, 4, 20, 294, 343, 10, 58}},
        {'m', {66, 4, 35, 305, 358, 59, 43}},
        {'n', {44, 4, 35, 365, 358, 37, 43}},
        {'o', {43, 2, 35, 403, 358, 39, 44}},
        {'p', {44, 4, 35, 443, 358, 38, 60}},
        {'q', {44, 2, 35, 1, 434, 38, 60}},
        {'r', {29, 4, 35, 40, 434, 26, 43}},
        {'s', {34, 1, 35, 67, 434, 32, 44}},
        {'t', {25, -1, 27, 100, 426, 27, 52}},
        {'u', {44, 3, 36, 128, 435, 37, 43}},
        {'v', {36, -2, 36, 166, 435, 40, 42}},
        {'w', {55, -2, 36, 207, 435, 59, 42}},
        {'x', {37, -1, 36, 267, 435, 39, 42}},
        {'y', {36, -2, 36, 307, 435, 40, 59}},
        {'z', {33, 0, 36, 348, 435, 33, 42}},
        {'{', {27, 0, 23, 382, 422, 27, 66}},
        {'|', {39, 15, 20, 410, 419, 9, 75}},
        {'}', {27, 0, 23, 420, 422, 27, 66}},
        {'~', {41, 1, 44, 448, 443, 39, 13}}}};
    void generateDataToCenter(RenderingSegment& segment, std::string const& text, Vector2<float> center, float height, Vector<float, 4> const& color = {1.f, 1.f, 1.f, 1.f});
}

#endif /* FontRenderer_hpp */
