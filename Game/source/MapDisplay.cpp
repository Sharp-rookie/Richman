#include <cstdio>
#include "Game.hpp"
#include "main.hpp"

void Game::display_loc(char _name, uint8_t loc)
{
    for (int i = 0; i < this->player_current_num; ++i)
    {
        if (this->player[player_index[i]].isBankrupt() == false && this->player[player_index[i]].location == loc && this->player[player_index[i]].name == _name)
        {
            this->player[player_index[i]].colorfulPrintf(this->player[player_index[i]].name);
            return;
        }
    }
    for (int i = 0; i < this->player_current_num; ++i)
    {
        if (this->player[player_index[i]].isBankrupt() == false && this->player[player_index[i]].location == loc)
        {
            this->player[player_index[i]].colorfulPrintf(this->player[player_index[i]].name);
            return;
        }
    }
    if (this->land[loc].isblock > 0)
    {
        putchar('#');
    }
    else if (this->land[loc].type == Land_Origin)
    {
        putchar('S');
    }
    else if (this->land[loc].type == Land_ToolHouse)
    {
        putchar('T');
    }
    else if (this->land[loc].type == Land_GiftHouse)
    {
        putchar('G');
    }
    else if (this->land[loc].type == Land_Park)
    {
        putchar('P');
    }
    else if (this->land[loc].type == Land_MagicHouse)
    {
        putchar('M');
    }
    else if (this->land[loc].type == Land_Mine)
    {
        putchar('$');
    }
    else if (this->land[loc].type == Land_Empty)
    {
        if (this->land[loc].owner == 0)
        {
            putchar(this->land[loc].level + '0');
        }
        else
        {
            uint8_t owner = this->land[loc].owner;
            this->player[this->Get_cur_player_index(owner)].colorfulPrintf(this->land[loc].level + '0');
        }
    }
    else
    {
        printf("!\n");
    }
    return;
}

void Game::Map_Display(char _name)
{
    for (int i = 0; i < 29; ++i)
    {
        this->display_loc(_name, i);
    }
    putchar('\n');
    for (int i = 0; i < 6; ++i)
    {
        this->display_loc(_name, 69 - i);
        for (int j = 0; j < 27; ++j)
            putchar(' ');
        this->display_loc(_name, 29 + i);
        putchar('\n');
    }
    for (int i = 63; i >= 35; --i)
    {
        this->display_loc(_name, i);
    }
    putchar('\n');
}

void Game::display_cur_play(char _name)
{
    string name;
    switch (_name)
    {
    case 'Q':
        name = "钱夫人>";
        break;
    case 'A':
        name = "阿土伯>";
        break;
    case 'S':
        name = "孙小美>";
        break;
    case 'J':
        name = "金贝贝>";
        break;
    }
    this->current_player->colorfulPrintf(name);
}