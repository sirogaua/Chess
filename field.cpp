#include "field.h"


field::field() // конструктор за замовчуванням
{
    
}

field::field(const field &cur) // конструктор копіювання
{
    pos_X = cur.pos_X;
    pos_Y = cur.pos_Y;
    size = cur.size;
    color = cur.color;
    select_color = cur.select_color;
    original_col = cur.original_col;
    figure = cur.figure;
}

field::field(int X, int Y) // конструктор із параметром
{
    pos_X = X;
    pos_Y = Y;
    size = 75;
    pixel_start_X = 300 + X * size;
    pixel_start_Y = 50 + Y * size;
    figure = 0;
    select_color = Color::Yellow;
}

/////////SETTERS
void field::set_color(Color col)
{
    color = col;
}

void field::set_original_col(Color col)
{
    original_col = col;
}

void field::set_select_color(Color col)
{
    select_color = col;
}   

void field::set_figure(int fig)
{
    figure = fig;
}

///////// GETTERS
Color field::get_color()
{
    return color;
}

Color field::get_original_col()
{
    return original_col;
}

Color field::get_select_color()
{
    return select_color;
}

int field::get_size()
{
    return size;
}

int field::get_pos_X()
{
    return pos_X;
}

int field::get_pos_Y()
{
    return pos_Y;
}

int field::get_pixel_start_X()
{
    return pixel_start_X;
}

int field::get_pixel_start_Y()
{
    return pixel_start_Y;
}

int field::get_figure()
{
    return figure;
}

field::~field() // деструктор
{
}