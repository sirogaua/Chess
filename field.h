#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class field
{
private:
    int pos_X;
    int pos_Y;
    int pixel_start_X;
    int pixel_start_Y;
    int size;
    Color color;
    Color select_color;
    Color original_col;
    int figure;
public:
    field();
    field(int X, int Y);
    field(const field &cur);
    void set_color(Color col);
    void set_select_color(Color col);
    void set_original_col(Color col);
    void set_figure(int fig);
    Color get_color();
    Color get_select_color();
    Color get_original_col();
    int get_size();
    int get_pos_X();
    int get_pos_Y();
    int get_pixel_start_X();
    int get_pixel_start_Y();
    int get_figure();
    ~field();
};