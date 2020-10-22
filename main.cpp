#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shape.hpp>

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
    int figure;
    bool was_move;
public:
    field();
    field(int X, int Y);
    void set_color(Color col);
    void set_select_color(Color col);
    void set_figure(int fig);
    void set_was_move();
    Color get_color();
    Color get_select_color();
    int get_size();
    int get_pos_X();
    int get_pos_Y();
    int get_pixel_start_X();
    int get_pixel_start_Y();
    int get_figure();
    bool get_was_move();
    ~field();
};

field::field()
{
    
}

field::field(int X, int Y)
{
    pos_X = X;
    pos_Y = Y;
    size = 75;
    pixel_start_X = 300 + X * size;
    pixel_start_Y = 50 + Y * size;
    figure = 0;
    select_color = Color::Yellow;
    was_move = true;
}

/////////SETTERS
void field::set_color(Color col)
{
    color = col;
}

void field::set_was_move()
{
    was_move = !was_move;
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

bool field::get_was_move()
{
    return was_move;
}

field::~field()
{
}



void search_moves(field (&mas)[8][8], int row, int column)
{
    if (mas[row][column].get_figure() == -1) // можливі ходи чоргої пішки
    {

        if ((column != 0) && (mas[row - 1][column - 1].get_figure() > 0) && (mas[row - 1][column - 1].get_figure() < 6)) // якщо є фігура по лівій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            mas[row - 1][column - 1].set_select_color(mas[row - 1][column - 1].get_color());
            mas[row - 1][column - 1].set_color(Color::Green);
        }

        if ((column != 7) && (mas[row - 1][column + 1].get_figure() > 0) && (mas[row - 1][column + 1].get_figure() < 6)) // якщо є фігура по правій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            mas[row - 1][column + 1].set_select_color(mas[row - 1][column + 1].get_color());
            mas[row - 1][column + 1].set_color(Color::Green);
        }

        if (mas[row - 1][column].get_figure() == 0) // простий хід у перед на одну клітинку, якщо нема на ній фігури
        {
            mas[row - 1][column].set_select_color(mas[row - 1][column].get_color());
            mas[row - 1][column].set_color(Color::Green);

            if ((row == 6) && (mas[row - 2][column].get_figure() == 0)) // хід у перед на дві клітинку, якщо нема на ній фігури
            {                                                           // та пішка ще не ходила
                mas[row - 2][column].set_select_color(mas[row - 2][column].get_color());
                mas[row - 2][column].set_color(Color::Green);
            }
            
        }

    }

    if (mas[row][column].get_figure() == 1) // можливі ходи білої пішки
    {

        if ((column != 0) && (mas[row + 1][column - 1].get_figure() < 0) && (mas[row + 1][column - 1].get_figure() > -6)) // якщо є фігура по лівій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            mas[row + 1][column - 1].set_select_color(mas[row + 1][column - 1].get_color());
            mas[row + 1][column - 1].set_color(Color::Green);
        }

        if ((column != 7) && (mas[row + 1][column + 1].get_figure() < 0) && (mas[row + 1][column + 1].get_figure() > -6)) // якщо є фігура по правій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            mas[row + 1][column + 1].set_select_color(mas[row + 1][column + 1].get_color());
            mas[row + 1][column + 1].set_color(Color::Green);
        }

        if (mas[row + 1][column].get_figure() == 0) // простий хід у перед на одну клітинку, якщо нема на ній фігури
        {
            mas[row + 1][column].set_select_color(mas[row + 1][column].get_color());
            mas[row + 1][column].set_color(Color::Green);

            if ((row == 1) && (mas[row + 2][column].get_figure() == 0)) // хід у перед на дві клітинку, якщо нема на ній фігури
            {                                                           // та пішка ще не ходила
                mas[row + 2][column].set_select_color(mas[row + 2][column].get_color());
                mas[row + 2][column].set_color(Color::Green);
            }
            
        }

    }
    
}



int main()
{
    bool allow = true;
    const int H = 8;
    const int V = 8;

    field *buff;
    field mas[V][H]; // поле 8х8
    int i, j;

    for (i = 0; i < V; i++)
    {
        for (j = 0; j < H; j++)
        {
            mas[i][j] = field(j, i);
            if ((j + i) % 2 == 0) // для поля
            {
                mas[i][j].set_color(Color::White);
            }
            else
            {
                mas[i][j].set_color(Color::Cyan);
            }


            if (i == 1) // для фігур (циклічно розставляємо пішки, так бистріше)
            {
                mas[i][j].set_figure(1);
            }

            if (i == 6)
            {
                    mas[i][j].set_figure(-1);
            }
            

            if ((i == 1) || (i == 6))
            {
                mas[i][j].set_was_move();
            }
            

            
        }
        
    }

     // для білих
    mas[0][0].set_figure(2); // тура 
    mas[0][7].set_figure(2);

    mas[0][1].set_figure(3); // кінь
    mas[0][6].set_figure(3);

    mas[0][2].set_figure(4); // офіцер
    mas[0][5].set_figure(4);

    mas[0][3].set_figure(5); // королева
    mas[0][4].set_figure(6); // король

    // для чорних
    mas[7][0].set_figure(-2); // тура 
    mas[7][7].set_figure(-2);

    mas[7][1].set_figure(-3); // кінь
    mas[7][6].set_figure(-3);

    mas[7][2].set_figure(-4); // офіцер
    mas[7][5].set_figure(-4);

    mas[7][3].set_figure(-5); // королева
    mas[7][4].set_figure(-6); // король


    Texture figures_tex;
    figures_tex.loadFromFile("Image/chess_2.png");

    Sprite figures_spr;
    figures_spr.setTexture(figures_tex);
    figures_spr.setTextureRect(IntRect(0, 0, 128, 128));
    figures_spr.scale(0.5f, 0.5f);

    sf::RenderWindow window(sf::VideoMode(1200, 680), "Chess");
    RectangleShape square;

    while (window.isOpen())
    {
        sf::Event event;

        window.clear();


        for (i = 0; i < H; i++)
        {
            for (j = 0; j < V; j++)
            {
                if (((i == 0) || (i == 7)) && (abs(mas[i][j].get_figure()) == 1)) // коли пішка дійшла кінця і має стати королевою
                {
                    mas[i][j].set_figure(5 * mas[i][j].get_figure());
                }
                


                square.setSize(Vector2f(mas[i][j].get_size(), mas[i][j].get_size()));
                square.setPosition(mas[i][j].get_pixel_start_X(), mas[i][j].get_pixel_start_Y());

                if ((allow == true) && (mas[i][j].get_color() == Color::Green))
                {
                    mas[i][j].set_color(mas[i][j].get_select_color());
                    mas[i][j].set_select_color(Color::Yellow);
                }

                if (IntRect(mas[i][j].get_pixel_start_X(), mas[i][j].get_pixel_start_Y(), mas[i][j].get_size(), mas[i][j].get_size()).contains(Mouse::getPosition(window)))
                {
                    while(window.pollEvent(event))
                    {
                        if (event.type == Event::MouseButtonReleased)
                        {
                            
                            if (event.mouseButton.button == Mouse::Left)
                            {
                                if ((mas[i][j].get_color() != Color::Yellow) && (allow == true)) // виділення клітки для ходу
                                {

                                    mas[i][j].set_select_color(mas[i][j].get_color());
                                    mas[i][j].set_color(Color::Yellow);
                                    allow = false;
                                    buff = &mas[i][j];
                                    search_moves(mas, i, j);
                                    break;
                                }

                                if ((mas[i][j].get_color() == Color::Yellow) && (allow == false)) // відміна ходу (якщо клікнути по тій самій клітці)
                                {
                                    mas[i][j].set_color(mas[i][j].get_select_color());
                                    mas[i][j].set_select_color(Color::Yellow);
                                    allow = true;
                                    break;
                                }
                                

                                if ((allow == false) && (mas[i][j].get_color() != Color::Yellow) && (buff->get_figure() != 0)) // перестановка фігури
                                {
                                    mas[i][j].set_figure((buff)->get_figure());
                                    (buff)->set_figure(0);
                                    buff->set_color(buff->get_select_color());
                                    buff->set_select_color(Color::Yellow);
                                    
                                    allow = true;
                                    buff = nullptr;
                                }
                                
                            }


                        }

                        if ((event.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Escape)))
                        {
                            window.close();
                        }	
                    }
                }

                square.setFillColor(mas[i][j].get_color());
                window.draw(square);


                // малюємо фігури
                if (mas[i][j].get_figure() == 1) // білі пішки
                {
                    figures_spr.setTextureRect(IntRect(0, 128, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }
                
                if (mas[i][j].get_figure() == 2) // білі тури
                {
                    figures_spr.setTextureRect(IntRect(384, 128, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == 3) // білі коні
                {
                    figures_spr.setTextureRect(IntRect(128, 128, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == 4) // білі офіцери
                {
                    figures_spr.setTextureRect(IntRect(256, 128, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }
                
                if (mas[i][j].get_figure() == 5) // біла королева
                {
                    figures_spr.setTextureRect(IntRect(512, 128, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == 6) // білий король
                {
                    figures_spr.setTextureRect(IntRect(640, 128, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == -1) // чорні пішки
                {
                    figures_spr.setTextureRect(IntRect(0, 0, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }
                
                if (mas[i][j].get_figure() == -2) // чорні тури
                {
                    figures_spr.setTextureRect(IntRect(384, 0, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == -3) // чорні коні
                {
                    figures_spr.setTextureRect(IntRect(128, 0, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == -4) // чорні офіцери
                {
                    figures_spr.setTextureRect(IntRect(256, 0, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }
                
                if (mas[i][j].get_figure() == -5) // чорний королева
                {
                    figures_spr.setTextureRect(IntRect(512, 0, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }

                if (mas[i][j].get_figure() == -6) // чорний король
                {
                    figures_spr.setTextureRect(IntRect(640, 0, 128, 128));
                    figures_spr.setPosition(mas[i][j].get_pixel_start_X(),  mas[i][j].get_pixel_start_Y());
                    window.draw(figures_spr);
                }
                
            }
            
        }
        





        window.display();
    }

    return 0;
}