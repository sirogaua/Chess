#include <iostream>
#include "field.h"

using namespace std;

// прототипи функцій (опис їх роботи розташований нижче, де йде їхня реалізація)
void search_moves(field (&mas)[8][8], int row, int column);
void draw_color(field &sec, Color col);
void pishka_move (field (&mas)[8][8], int row, int column);
bool check_sign(int first, int second);
void redraw (field (&mas)[8][8], Color col);
void redraw1 (field (&mas)[8][8], Color col, Color need);
bool for_king (field (&mas)[8][8], field &king);
bool search_overlap(field (&mas)[8][8], int row, int column);
void move_king(field (&mas)[8][8], int row, int column);
bool search_moves_king(field (&mas)[8][8], int row, int column);
void search_moves(field (&mas)[8][8], int row, int column);


int main()
{
    int lose = 0; // зміна контролю гри (виграш і програш)
    bool allow = true; // дозвіл на хід
    int vs_buff = 1; // зміна для контролю ходу (білі або чорні)
    const int H = 8;
    const int V = 8;

    string help_tex = "Help-bar:\n\nWHITES WALK!\n\n"; // текст хелпбара

    field *buff, *white_king, *black_king;
    field mas[V][H]; // поле 8х8
    int i, j;

    for (i = 0; i < V; i++)
    {
        for (j = 0; j < H; j++)
        {
            mas[i][j] = field(j, i);
            if ((j + i) % 2 == 0) // білі клітинки
            {
                mas[i][j].set_color(Color::White);
                mas[i][j].set_original_col(Color::White);
            }
            else // чорні клітинки (в мене вони реалізовані як голубі)
            {
                mas[i][j].set_color(Color::Cyan);
                mas[i][j].set_original_col(Color::Cyan);
            }


            if (i == 1) // для фігур (циклічно розставляємо пішки, так бистріше)
            {
                mas[i][j].set_figure(1);
            }

            if (i == 6)
            {
                mas[i][j].set_figure(-1);
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


    Texture figures_tex, lol_tex;

    figures_tex.loadFromFile("Image/chess_2.png");
    lol_tex.loadFromFile("Image/lose.jpg");


    Sprite figures_spr, lol_spr;

    figures_spr.setTexture(figures_tex);
    figures_spr.setTextureRect(IntRect(0, 0, 128, 128));
    figures_spr.scale(0.5f, 0.5f);

    lol_spr.setTexture(lol_tex);
    lol_spr.setPosition(300, 0);


    Text text, text_help;
	Font font;
	text.setFillColor(Color::Yellow);
	text.setFont(font);
	font.loadFromFile("Fonts/arial.ttf");
	text.setCharacterSize(20);
	text.setStyle(Text::Bold | Text::Underlined);
    text.setPosition(450, 50);




    sf::RenderWindow window(sf::VideoMode(1200, 680), "Chess");
    RectangleShape square, help_column;

    help_column.setSize(Vector2f(250, 200)); // хелпбар
    help_column.setPosition(910, 280);
    help_column.setFillColor(Color::Yellow);

    text_help.setFillColor(Color::Red);
    text_help.setFont(font);
    text_help.setCharacterSize(20);
    text_help.setStyle(Text::Bold);
    text_help.setPosition(920, 290);


    while (window.isOpen())
    {
        sf::Event event;

        window.clear();

        if (lose == 0)
        {

            for (i = 0; i < H; i++)
            {
                for (j = 0; j < V; j++)
                {
                    if (((i == 0) || (i == 7)) && (abs(mas[i][j].get_figure()) == 1)) // коли пішка дійшла кінця і має стати королевою
                    {
                        mas[i][j].set_figure(5 * mas[i][j].get_figure());
                    }

                    if (mas[i][j].get_figure() == 6) // постійно контролюємо, щоб у нас був доступ на клітинки на якій білий король
                    {
                        white_king = &mas[i][j];
                    }

                    if (mas[i][j].get_figure() == -6) // постійно контролюємо, щоб у нас був доступ на клітинки на якій чорний король
                    {
                        black_king = &mas[i][j];
                    }
                    
                    
                    

                    square.setSize(Vector2f(mas[i][j].get_size(), mas[i][j].get_size()));
                    square.setPosition(mas[i][j].get_pixel_start_X(), mas[i][j].get_pixel_start_Y());

                    if ((allow == true) && (mas[i][j].get_color() == Color::Green))
                    {
                        mas[i][j].set_color(mas[i][j].get_select_color());
                    }

                    if (IntRect(mas[i][j].get_pixel_start_X(), mas[i][j].get_pixel_start_Y(), mas[i][j].get_size(), mas[i][j].get_size()).contains(Mouse::getPosition(window)))
                    { // якшо ми на водимо на область поля, то відбувається фіксування руху мишки і клікання
                        while(window.pollEvent(event))
                        {
                            if (event.type == Event::MouseButtonReleased)
                            {
                                
                                if (event.mouseButton.button == Mouse::Left)
                                {
                                    if ((mas[i][j].get_color() != Color::Yellow) && (allow == true) && (vs_buff * mas[i][j].get_figure() > 0)) // виділення клітки для ходу
                                    {
                                        
                                        draw_color(mas[i][j], Color::Yellow);
                                        allow = false;
                                        buff = &mas[i][j];

                                        if (abs(mas[i][j].get_figure()) == 6) // окремо дивимось рухи короля
                                        {
                                            move_king(mas, i, j); 
                                        }
                                        else
                                        { // рухи інших фігур
                                            search_moves(mas, i, j);
                                        }


                                        if (abs(mas[i][j].get_figure()) == 1) //існувал проблема в розділення ходу та атаки пішки
                                        {
                                            pishka_move(mas, i, j);
                                        }
                                        
                                        break;
                                    }

                                    if ((mas[i][j].get_color() == Color::Yellow) && (allow == false)) // відміна ходу (якщо клікнути по тій самій клітці)
                                    {
                                        draw_color(mas[i][j], mas[i][j].get_original_col());
                                        allow = true;
                                        break;
                                    }
                                    

                                    if ((allow == false) && (mas[i][j].get_color() == Color::Green) && (buff->get_figure() != 0)) // перестановка фігури
                                    {
                                        mas[i][j].set_figure((buff)->get_figure());
                                        (buff)->set_figure(0);
                                        buff->set_color(buff->get_original_col());
                                        buff->set_select_color(Color::Yellow);
                                        redraw(mas, Color::Green);
                                        vs_buff *= -1;
                                        allow = true;
                                        buff = nullptr;
                                        

                                        help_tex = "Help-bar:\n\n";

                                        if (vs_buff * white_king->get_figure() == 6)
                                        {
                                            help_tex += "WHITES WALK!\n\n";
                                            if (for_king(mas, (*white_king))) // перевірка шаху та мату для білого короля після ходу чорних
                                            {
                                                help_tex += "CHESS WHITE KING!";
                                                if (search_moves_king(mas, (*white_king).get_pos_Y(), (*white_king).get_pos_X()))
                                                {

                                                }
                                                else
                                                {
                                                    
                                                    lose = 1;
                                                }
                                            }
                                            
                                        }

                                        if (vs_buff * black_king->get_figure() == 6)
                                        {
                                            help_tex += "BLACKS WALK!\n\n";
                                            if (for_king(mas, (*black_king))) // перевірка шаху та мату для чорного короля після ходу білих
                                            {
                                                help_tex += "CHESS BLACK KING!";
                                                if (search_moves_king(mas, (*black_king).get_pos_Y(), (*black_king).get_pos_X()))
                                                {
                                                    
                                                }
                                                else
                                                {
                                                    lose = -1;
                                                }
                                            }
                                            
                                        }

                                    }
                                    
                                }


                            }

                            if ((event.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Escape))) // закриття вікна при натискані Esc
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



            text_help.setString(help_tex);
            window.draw(help_column);
            window.draw(text_help);
        }
        else
        {
            if ((event.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Escape))) // закриття вікна при натискані Esc
            {
                window.close();
            }
            if (lose == -1) // білі виграли
            {
                text.setString("WHITE WIN, Press Esc to exit!");
                window.draw(lol_spr);
                window.draw(text);
            }
            else
            {
                if (lose == 1) // чорні виграли
                {
                    text.setString("BLACK WIN, Press Esc to exit!");
                    window.draw(lol_spr);
                    window.draw(text);
                }
                
            }
        }

        window.display();
    }

    return 0;
}



void draw_color(field &sec, Color col) // перемальовує клітинку sec масиву в колір col і зберігає у буфер select_color попередній колір
{
    if (sec.get_color() != col)
    {
        sec.set_select_color(sec.get_color());
        sec.set_color(col);
    }
    
}



void pishka_move (field (&mas)[8][8], int row, int column) // ходи пішок (без атаки)
{
    
    if ((mas[row - 1][column].get_figure() == 0) && (mas[row][column].get_figure() == -1)) // простий хід у перед на одну клітинку, якщо нема на ній фігури
    {
        draw_color(mas[row - 1][column], Color::Green);

        if ((row == 6) && (mas[row - 2][column].get_figure() == 0)) // хід у перед на дві клітинку, якщо нема на ній фігури
        {                                                           // та пішка ще не ходила
            draw_color(mas[row - 2][column], Color::Green);
        }
        
    }


    if ((mas[row + 1][column].get_figure() == 0) && (mas[row][column].get_figure() == 1)) // простий хід у перед на одну клітинку, якщо нема на ній фігури
    {
        draw_color(mas[row + 1][column], Color::Green);

        if ((row == 1) && (mas[row + 2][column].get_figure() == 0)) // хід у перед на дві клітинку, якщо нема на ній фігури
        {                                                           // та пішка ще не ходила
            draw_color(mas[row + 2][column], Color::Green);
        }
        
    }
}


bool check_sign(int first, int second) // функція, яка перевіряє чи є дві фігури одного кольору
{                                      // вона допоміжна для пошуку можливих ходів фігури
    if ((first > 0) && (second <= 0) && (second >= -6))
    {
        return true;
    }
    else
    {
        if ((first < 0) && (second >= 0) && (second <= 6))
        {
            return true;
        }
        else
        {
            return false;
        }
        
        
    }
    
    
}

void redraw (field (&mas)[8][8], Color col) // перемальовує клітинки які дорівнюють сol в select_color (буферний колір)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mas[i][j].get_color() == col)
            {
                draw_color(mas[i][j], mas[i][j].get_select_color());
            }
            
        }
        
    }
    
}

void redraw1 (field (&mas)[8][8], Color col, Color need) // перемальовує клітинки які дорівнюють сol в need
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mas[i][j].get_color() == col)
            {
                mas[i][j].set_color(need);
            }
            
        }
        
    }
    
}

bool for_king (field (&mas)[8][8], field &king) // функція, яка повертає true, якшо королю в даній позиції шах і false, якшо ні
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mas[i][j].get_figure() * king.get_figure() < 0)
            {
                search_moves(mas, i, j);
            }
            
        }
        
    }

    if (king.get_color() == Color::Green)
    {
        return true;
    }
    return false;
}


bool search_overlap(field (&mas)[8][8], int row, int column) // незакінчена функція перевірки, якшо королю шах і на усіх його можливих 
{                                                            // ходах йому теж буде шах, чи миже його перекрити якась з його фігур
    field buff[8][8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            buff[i][j] = mas[i][j];   
        }
        
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((mas[i][j].get_figure() * mas[row][column].get_figure() > 0) && (mas[i][j].get_figure() * mas[row][column].get_figure() < 36))
            {
                search_moves(buff, i, j);
            }
            
        }
        
    }
    
    return false;
}

void move_king(field (&mas)[8][8], int row, int column) // пошук ходів короля
{
    field buff[8][8]; // створення копії дошки
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            buff[i][j] = mas[i][j];   
        }
        
    }

    for_king(mas, mas[row][column]);
    redraw1(mas, Color::Green, Color::Red);

    if ((row > 0) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column - 1].get_figure()))) // хід на одну клітку вперед і одну вліво
    {
        if (mas[row - 1][column - 1].get_color() != Color::Red)
        {
            buff[row - 1][column - 1].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row - 1][column - 1]))
            {
                draw_color(mas[row - 1][column - 1], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row - 1][column - 1].set_figure(mas[row - 1][column - 1].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((row > 0) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column].get_figure()))) // хід на одну клітку вперед
    {
        if (mas[row - 1][column].get_color() != Color::Red)
        {
            buff[row - 1][column].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row - 1][column]))
            {
                draw_color(mas[row - 1][column], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row - 1][column].set_figure(mas[row - 1][column].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((row > 0) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column + 1].get_figure()))) // хід на одну клітку вперед і одну вправо
    {
        if (mas[row - 1][column + 1].get_color() != Color::Red)
        {
            buff[row - 1][column + 1].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row - 1][column + 1]))
            {
                draw_color(mas[row - 1][column + 1], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row - 1][column + 1].set_figure(mas[row - 1][column + 1].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((row < 7) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column - 1].get_figure()))) // хід на одну клітку назад і одну вліво
    {
        if (mas[row + 1][column - 1].get_color() != Color::Red)
        {
            buff[row + 1][column - 1].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row + 1][column - 1]))
            {
                draw_color(mas[row + 1][column - 1], Color::Green);
            }

            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row + 1][column - 1].set_figure(mas[row + 1][column - 1].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((row < 7) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column].get_figure()))) // хід на одну клітку назад
    {
        if (mas[row + 1][column].get_color() != Color::Red)
        {
            buff[row + 1][column].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row + 1][column]))
            {
                draw_color(mas[row + 1][column], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row + 1][column].set_figure(mas[row + 1][column].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((row < 7) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column + 1].get_figure()))) // хід на одну клітку назад і одну вправо
    {
        if (mas[row + 1][column + 1].get_color() != Color::Red)
        {
            buff[row + 1][column + 1].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row + 1][column + 1]))
            {
                draw_color(mas[row + 1][column + 1], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row + 1][column + 1].set_figure(mas[row + 1][column + 1].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((column > 0) && (check_sign(mas[row][column].get_figure(), mas[row][column + 1].get_figure()))) // хід на одну клітку вправо
    {
        if (mas[row][column + 1].get_color() != Color::Red)
        {
            buff[row][column + 1].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row][column + 1]))
            {
                draw_color(mas[row][column + 1], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row][column + 1].set_figure(mas[row][column + 1].get_figure());
            redraw(buff, Color::Green);
        }
    }

    if ((column < 7) && (check_sign(mas[row][column].get_figure(), mas[row][column - 1].get_figure()))) // хід на одну клітку вліво
    {
        if (mas[row][column - 1].get_color() != Color::Red)
        {
            buff[row][column - 1].set_figure(buff[row][column].get_figure());
            buff[row][column].set_figure(0);
            if (!for_king(buff, buff[row][column - 1]))
            {
                draw_color(mas[row][column - 1], Color::Green);
            }
            buff[row][column].set_figure(mas[row][column].get_figure());
            buff[row][column - 1].set_figure(mas[row][column - 1].get_figure());
            redraw(buff, Color::Green);
        }
    }

    redraw(mas, Color::Red);
}

bool search_moves_king(field (&mas)[8][8], int row, int column) // перевірка чи король може піти на клітинки своїх ходів
{
    if ((row > 0) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column - 1].get_figure()))) // хід на одну клітку вперед і одну вліво
    {
        if (mas[row - 1][column - 1].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
        
    }

    if ((row > 0) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column].get_figure()))) // хід на одну клітку вперед
    {
        if (mas[row - 1][column].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }

    if ((row > 0) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column + 1].get_figure()))) // хід на одну клітку вперед і одну вправо
    {
        if (mas[row - 1][column + 1].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }

    if ((row < 7) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column - 1].get_figure()))) // хід на одну клітку назад і одну вліво
    {
        if (mas[row + 1][column - 1].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }

    if ((row < 7) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column].get_figure()))) // хід на одну клітку назад
    {
        if (mas[row + 1][column].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }

    if ((row < 7) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column + 1].get_figure()))) // хід на одну клітку назад і одну вправо
    {
        if (mas[row + 1][column + 1].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }

    if ((column > 0) && (check_sign(mas[row][column].get_figure(), mas[row][column + 1].get_figure()))) // хід на одну клітку вправо
    {
        if (mas[row][column + 1].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }

    if ((column < 7) && (check_sign(mas[row][column].get_figure(), mas[row][column - 1].get_figure()))) // хід на одну клітку вліво
    {
        if (mas[row][column - 1].get_color() != Color::Green)
        {
            redraw(mas, Color::Green);
            return true;
        }
    }
        
    redraw(mas, Color::Green);
    return false;
}


void search_moves(field (&mas)[8][8], int row, int column) // пошук ходів усіх фігур
{
    if (mas[row][column].get_figure() == -1) // можливі атаки чоргої пішки
    {

        if ((column != 0) && (mas[row - 1][column - 1].get_figure() > 0) && (mas[row - 1][column - 1].get_figure() < 6)) // якщо є фігура по лівій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            draw_color(mas[row - 1][column - 1], Color::Green);
        }

        if ((column != 7) && (mas[row - 1][column + 1].get_figure() > 0) && (mas[row - 1][column + 1].get_figure() < 6)) // якщо є фігура по правій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            draw_color(mas[row - 1][column + 1], Color::Green);
        }
    }

    if (mas[row][column].get_figure() == 1) // можливі атаки білої пішки
    {

        if ((column != 0) && (mas[row + 1][column - 1].get_figure() < 0) && (mas[row + 1][column - 1].get_figure() > -6)) // якщо є фігура по лівій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            draw_color(mas[row + 1][column - 1], Color::Green);
        }

        if ((column != 7) && (mas[row + 1][column + 1].get_figure() < 0) && (mas[row + 1][column + 1].get_figure() > -6)) // якщо є фігура по правій діагоналі
        {                                                                                                               // на відстані однієї клітинки спереду
            draw_color(mas[row + 1][column + 1], Color::Green);
        }

    }

    if (abs(mas[row][column].get_figure()) == 3) // ходи для усіх конів
    {
        if ((row > 1) && (column != 0)) // хід на дві клітки вперед і одну вліво
        {
            if (check_sign(mas[row][column].get_figure(), mas[row - 2][column - 1].get_figure()))
            {
                draw_color(mas[row - 2][column - 1], Color::Green);
            }
               
        }

        if ((row > 1) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row - 2][column + 1].get_figure()))) // хід на дві клітки вперед і одну вправо
        {
            draw_color(mas[row - 2][column + 1], Color::Green);
        }

        if ((row != 0) && (column < 6) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column + 2].get_figure()))) // хід на одну клітки вперед і дві вправо
        {
            draw_color(mas[row - 1][column + 2], Color::Green);   
        }

        if ((row != 0) && (column > 1) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column - 2].get_figure()))) // хід на одну клітки вперед і дві вліво
        {
            draw_color(mas[row - 1][column - 2], Color::Green);   
        }

        if ((row < 6) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row + 2][column - 1].get_figure()))) // хід на дві клітки назад і одну вліво
        {
            draw_color(mas[row + 2][column - 1], Color::Green);
        }

        if ((row < 6) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row + 2][column + 1].get_figure()))) // хід на дві клітки назад і одну вправо
        {
            draw_color(mas[row + 2][column + 1], Color::Green);   
        }

        if ((row != 7) && (column < 6) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column + 2].get_figure()))) // хід на одну клітки назад і дві вправо
        {
            draw_color(mas[row + 1][column + 2], Color::Green);   
        }

        if ((row != 7) && (column > 1) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column - 2].get_figure()))) // хід на одну клітки назад і дві вліво
        {
            draw_color(mas[row + 1][column - 2], Color::Green);
        }
        
    }

    if ((abs(mas[row][column].get_figure()) == 2) || (abs(mas[row][column].get_figure()) == 5)) // ходи для усіх тур і королев
    {
        int i = row - 1; // Усі клітинки з переду до кінця поля або до зустрічі із фігурою
        while ((i >= 0) && (check_sign(mas[row][column].get_figure(), mas[i][column].get_figure())))
        {
            if (mas[i][column].get_figure() != 0)
            {
               draw_color(mas[i][column], Color::Green);
                break;
            }
            else
            {
                draw_color(mas[i][column], Color::Green);
            }
            i--;
        }

        i = row + 1; // Усі клітинки з заду до кінця поля або до зустрічі із фігурою
        while ((i <= 7) && (check_sign(mas[row][column].get_figure(), mas[i][column].get_figure())))
        {
            if (mas[i][column].get_figure() != 0)
            {
                draw_color(mas[i][column], Color::Green);
                break;
            }
            else
            {
                draw_color(mas[i][column], Color::Green);
            }
            i++;
        }

        i = column - 1; // Усі клітинки зправа до кінця поля або до зустрічі із фігурою
        while ((i >= 0) && (check_sign(mas[row][column].get_figure(), mas[row][i].get_figure())))
        {
            if (mas[row][i].get_figure() != 0)
            {
                draw_color(mas[row][i], Color::Green);
                break;
            }
            else
            {
                draw_color(mas[row][i], Color::Green);
            }
            i--;
        }

        i = column + 1; // Усі клітинки зправа до кінця поля або до зустрічі із фігурою
        while ((i <= 7) && (check_sign(mas[row][column].get_figure(), mas[row][i].get_figure())))
        {
            if (mas[row][i].get_figure() != 0)
            {
                draw_color(mas[row][i], Color::Green);
                break;
            }
            else
            {
               draw_color(mas[row][i], Color::Green);
            }
            i++;
        }
        
    }

    if ((abs(mas[row][column].get_figure()) == 4) || (abs(mas[row][column].get_figure()) == 5)) // ходи для усіх офіцерів і королев
    {
        int i = row - 1;
        int j = column - 1; // Усі клітинки по лівій діагоналі на верх до кінця поля або до зустрічі із фігурою
        while ((i >= 0) && (j >= 0) && (check_sign(mas[row][column].get_figure(), mas[i][j].get_figure())))
        {
            if (mas[i][j].get_figure() != 0)
            {
                draw_color(mas[i][j], Color::Green);
                break;
            }
            else
            {
                draw_color(mas[i][j], Color::Green);
            }
            i--;
            j--;
        }

        i = row - 1;
        j = column + 1; // Усі клітинки по правій діагоналі на верх до кінця поля або до зустрічі із фігурою
        while ((i >= 0) && (j <= 7) && (check_sign(mas[row][column].get_figure(), mas[i][j].get_figure())))
        {
            if (mas[i][j].get_figure() != 0)
            {
                draw_color(mas[i][j], Color::Green);
                break;
            }
            else
            {
               draw_color(mas[i][j], Color::Green);
            }
            i--;
            j++;
        }

        i = row + 1;
        j = column + 1; // Усі клітинки по правій діагоналі на низ до кінця поля або до зустрічі із фігурою
        while ((i <= 7) && (j <= 7) && (check_sign(mas[row][column].get_figure(), mas[i][j].get_figure())))
        {
            if (mas[i][j].get_figure() != 0)
            {
                draw_color(mas[i][j], Color::Green);
                break;
            }
            else
            {
                draw_color(mas[i][j], Color::Green);
            }
            i++;
            j++;
        }

        i = row + 1;
        j = column - 1; // Усі клітинки по лівій діагоналі на низ до кінця поля або до зустрічі із фігурою
        while ((i <= 7) && (j >= 0) && (check_sign(mas[row][column].get_figure(), mas[i][j].get_figure())))
        {
            if (mas[i][j].get_figure() != 0)
            {
                draw_color(mas[i][j], Color::Green);
                break;
            }
            else
            {
                draw_color(mas[i][j], Color::Green);
            }
            i++;
            j--;
        }

    }

    if (abs(mas[row][column].get_figure()) == 6) // можливі ходи короля (це використовується для того, 
    {                                            // щоб встановити радіус ворожого короля для ненаближення до нього) 
        if ((row > 0) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column - 1].get_figure()))) // хід на одну клітку вперед і одну вліво
        {
            if (mas[row - 1][column - 1].get_color() != Color::Red)
            {
                draw_color(mas[row - 1][column - 1], Color::Green);
            }
        }

        if ((row > 0) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column].get_figure()))) // хід на одну клітку вперед
        {
            if (mas[row - 1][column].get_color() != Color::Red)
            {
                draw_color(mas[row - 1][column], Color::Green);
            }
        }

        if ((row > 0) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row - 1][column + 1].get_figure()))) // хід на одну клітку вперед і одну вправо
        {
            if (mas[row - 1][column + 1].get_color() != Color::Red)
            {
                draw_color(mas[row - 1][column + 1], Color::Green);
            }
        }

        if ((row < 7) && (column != 0) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column - 1].get_figure()))) // хід на одну клітку назад і одну вліво
        {
            if (mas[row + 1][column - 1].get_color() != Color::Red)
            {
                draw_color(mas[row + 1][column - 1], Color::Green);
            }
        }

        if ((row < 7) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column].get_figure()))) // хід на одну клітку назад
        {
            if (mas[row + 1][column].get_color() != Color::Red)
            {
                draw_color(mas[row + 1][column], Color::Green);
            }
        }

        if ((row < 7) && (column != 7) && (check_sign(mas[row][column].get_figure(), mas[row + 1][column + 1].get_figure()))) // хід на одну клітку назад і одну вправо
        {
            if (mas[row + 1][column + 1].get_color() != Color::Red)
            {
                draw_color(mas[row + 1][column + 1], Color::Green);
            }
        }

        if ((column > 0) && (check_sign(mas[row][column].get_figure(), mas[row][column + 1].get_figure()))) // хід на одну клітку вправо
        {
            if (mas[row][column + 1].get_color() != Color::Red)
            {
                draw_color(mas[row][column + 1], Color::Green);
            }
        }

        if ((column < 7) && (check_sign(mas[row][column].get_figure(), mas[row][column - 1].get_figure()))) // хід на одну клітку вліво
        {
            if (mas[row][column - 1].get_color() != Color::Red)
            {
                draw_color(mas[row][column - 1], Color::Green);
            }
        }
    }
    
    
    
}
