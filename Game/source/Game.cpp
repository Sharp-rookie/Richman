#include "Game.hpp"
#include <iostream>
#include <cstring>
#include <windows.h>
#include <algorithm>
#include "main.hpp"

using namespace std;

const string s_Roll = string("roll");
const string s_Query = string("query");
const string s_Quit = string("quit");
const string s_Sell = string("sell");
const string s_Block = string("block");
const string s_Robot = string("robot");
const string s_Help = string("help");
const string s_Step = string("step");
const string s_Preset = string("preset");
const string s_Dump = string("dump");

Game::Game(uint8_t _player_total_num, uint8_t _order[], int init_fund)
{
    player_total_num = _player_total_num;
    player_current_num = player_total_num;

    for (int i = 0; i < _player_total_num; i++)
    {
        player_index[i] = _order[i];
        player_index_raw[i] = _order[i];
    }

    current_player_i = 0;
    current_player = &(player[player_index[current_player_i]]);

    //初始化地图
    Map_Init();
    //初始化人物
    Player_Init(init_fund);

    system("cls");
    this->Map_Display(this->current_player->name);
}

void Game::Map_Init()
{
    for (int i = 1; i < 29; i++)
    {
        land[i] = Land(Land_Empty, 200);
    }
    for (int i = 29; i < 35; i++)
    {
        land[i] = Land(Land_Empty, 500);
    }
    for (int i = 35; i < 64; i++)
    {
        land[i] = Land(Land_Empty, 300);
    }
    for (int i = 64; i < 70; i++)
    {
        land[i] = Land(Land_Mine, 0);
    }

    land[0] = Land(Land_Origin, 0);
    // land[14] = Land(Land_Hospitol, 0);
    land[14] = Land(Land_Park, 0);
    land[28] = Land(Land_ToolHouse, 0);
    land[35] = Land(Land_GiftHouse, 0);
    // land[49] = Land(Land_Prison, 0);
    land[49] = Land(Land_Park, 0);
    land[63] = Land(Land_MagicHouse, 0);
    land[64].credit = 60;
    land[65].credit = 80;
    land[66].credit = 40;
    land[67].credit = 100;
    land[68].credit = 80;
    land[69].credit = 20;
}

void Game::Player_Init(int init_fund)
{
    for (int i = 0; i < this->player_current_num; i++)
    {
        player[player_index[i]] = Player(player_index[i], init_fund);
    }
}

void Game::Turn_to_next_Player()
{
    current_player_i = (current_player_i + 1) % player_current_num;
    current_player = &(player[player_index[current_player_i]]);
}

uint8_t Game::Get_cur_player_index(char _name)
{
    for (int i = 0; i < this->player_current_num; i++)
    {
        if (this->player[this->player_index[i]].name == _name)
        {
            return this->player_index[i];
        }
    }
    printf("Get_cur_player_index wrong!\n");
    exit(0);
}

void Game::Player_Bankrupt(Player *p)
{
    if (p->fund >= 0)
        return;

    //标记破产
    p->bankrupt = 1;

    //位置为 -1
    p->location = -1;

    //玩家顺序表更新
    int i;
    for (i = 0; i < player_current_num; i++)
    {
        if (player[player_index[i]].name == p->name)
            break;
    }
    for (; i < player_current_num - 1; i++)
    {
        player_index[i] = player_index[i + 1];
    }

    //玩家数量减一
    player_current_num--;

    //地产归公
    for (i = 0; i < LAND_NUM; i++)
    {
        if (land[i].owner == p->name)
        {
            land[i].owner = 0;
            land[i].level = 0;
        }
    }

    //因为破产玩家是当前玩家，所以其退出后后面玩家排序向前移 1，防止在Turn_to_next_Player()里溢出
    current_player_i = (current_player_i - 1 + player_current_num) % player_current_num;
}

void Game::Lay_Block(int loc)
{
    if (this->current_player->tool->barrier <= 0)
    {
        printf("你没有路障！\n");
        return;
    }

    if (loc > 10 || loc < -10)
    {
        printf("距离指定错误！\n");
        return;
    }
    int8_t p_location = this->current_player->location + loc;
    if (p_location >= 0)
    {
        p_location %= 70;
    }
    else
    {
        p_location += 70;
    }

    if (this->land[p_location].isblock == 1)
    {
        printf("指定地点已经有路障了，无法再放！\n");
        return;
    }

    for (uint8_t i = 0; i < this->player_current_num; i++)
    {
        if (p_location == this->player[this->player_index[i]].location)
        {
            printf("指定地点有人，无法放！\n");
            return;
        }
    }

    this->land[p_location].isblock = 1;
    this->current_player->tool->barrier--;

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
    printf("\n");
    this->display_cur_play(this->current_player->name);
    printf("你用了一个路障，还剩%d个\n", this->current_player->tool->barrier);
}

void Game::Lay_Robot()
{
    if (this->current_player->tool->robot <= 0)
    {
        printf("你没有机器人！\n");
        return;
    }

    uint8_t p_location = this->current_player->location;
    uint8_t count = 0;
    for (int i = 0; i < 11; i++)
    {
        if (land[(p_location + i) % 70].isblock == 1)
        {
            count++;
        }
        land[(p_location + i) % 70].isblock = 0;
    }
    this->current_player->tool->robot--;
    printf("你用了一个机器人，还剩%d个，清扫了前方%d个路障\n", this->current_player->tool->robot, count);

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
    printf("\n");
    this->display_cur_play(this->current_player->name);
}

void Game::Roll()
{
    uint8_t step = rand() % 6 + 1; //步数为1~6的随机数
    uint8_t i;
    for (i = 0; i < step; ++i)
    {
        this->current_player->location = (this->current_player->location + 1) % 70;
        if (this->land[this->current_player->location].isblock)
        {
            this->land[this->current_player->location].isblock = 0;

            //刷新屏幕
            system("cls");
            this->Map_Display(this->current_player->name);
            printf("\n");
            this->display_cur_play(this->current_player->name);
            printf("你撞到了路障，走了%d步就停了\n", i + 1);

            return;
        }
    }

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
    printf("\n");
    this->display_cur_play(this->current_player->name);
    printf("前进%d步\n", i);
}

void Game::Step(int n)
{
    int step = n;
    if (step >= 0)
    {
        for (int i = 0; i < step; ++i)
        {
            this->current_player->location = (this->current_player->location + 1) % 70;
            if (this->land[this->current_player->location].isblock)
            {
                this->land[this->current_player->location].isblock = 0;

                //刷新屏幕
                system("cls");
                this->Map_Display(this->current_player->name);
                printf("\n");
                this->display_cur_play(this->current_player->name);
                printf("你撞到了路障，走了%d步就停了\n", i + 1);

                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < -step; ++i)
        {
            this->current_player->location = (this->current_player->location - 1 + 70) % 70;
            if (this->land[this->current_player->location].isblock)
            {
                this->land[this->current_player->location].isblock = 0;

                //刷新屏幕
                system("cls");
                this->Map_Display(this->current_player->name);
                printf("\n");
                this->display_cur_play(this->current_player->name);
                printf("你撞到了路障，退了%d步就停了\n", i + 1);

                return;
            }
        }
    }

    system("cls");
    this->Map_Display(this->current_player->name);
    printf("\n");
}

string Game::Get_charactor_name(char _name)
{
    string s;
    switch (_name)
    {
    case 'Q':
        s = "钱夫人";
        break;
    case 'A':
        s = "阿土伯";
        break;
    case 'S':
        s = "孙小美";
        break;
    case 'J':
        s = "金贝贝";
        break;
    }
    return s;
}

void Game::MagicHouse()
{
    printf("请输入要陷害的玩家：");
    for (int i = 0; i < player_current_num; i++)
    {
        switch (player[player_index[i]].name)
        {
        case 'A':
            printf("2——%s ", Get_charactor_name(player[player_index[i]].name).c_str());
            break;
        case 'Q':
            printf("1——%s ", Get_charactor_name(player[player_index[i]].name).c_str());
            break;
        case 'S':
            printf("3——%s ", Get_charactor_name(player[player_index[i]].name).c_str());
            break;
        case 'J':
            printf("4——%s ", Get_charactor_name(player[player_index[i]].name).c_str());
            break;
        }
    }
    printf("（其他输入表示放弃）\n ");

    char c;
    cin >> c;

    switch (c)
    {
    case '2':
        c = 'A';
        break;
    case '1':
        c = 'Q';
        break;
    case '3':
        c = 'S';
        break;
    case '4':
        c = 'J';
        break;
    default:
    {
        printf("输入无效\n");
        return;
    }
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
            flag++;
    }
    if (flag == 1)
    {
        this->player[Get_cur_player_index(c)].magic_days += 2;

        printf("%s被魔法陷害，轮空轮数加2，共需轮空%d轮\n", Get_charactor_name(this->player[Get_cur_player_index(c)].name).c_str(), this->player[Get_cur_player_index(c)].magic_days);
        return;
    }

    printf("输入无效！\n");
}

void Game::SellBuliding(Land &_land)
{
    if (_land.owner != this->current_player->name || _land.type != Land_Empty) //输入的不是自己的地产
    {
        printf("这不是你的地产!\n");
        return;
    }

    // printf("确认要卖出该地产吗？[y/n]\n");
    // while (true)
    // {
    //     string s;
    //     cin >> s;
    //     if (s == string("y") || s == string("Y"))
    //     {
    //         this->current_player->fund += _land.Sell_price(); //获得卖房收益
    //         uint8_t m = _land.Sell_price();
    //         _land.owner = 0; //土地所有者清除
    //         _land.level = 0; //土地等级归为空地
    //         //刷新屏幕
    //         system("cls");
    //         this->Map_Display(this->current_player->name);
    //         printf("\n");
    //         this->display_cur_play(this->current_player->name);
    //         printf("你卖出了此房产，获得收入：%d，当前资金：%d\n", m, this->current_player->fund);
    //     }
    //     else if (s == string("n") || s == string("N"))
    //     {
    //         break;
    //     }
    //     else
    //     {
    //         printf("注意输入规范，请重新输入[y/n]\n");
    //         continue;
    //     }
    //     break;
    // }

    this->current_player->fund += _land.Sell_price(); //获得卖房收益
    uint8_t m = _land.Sell_price();
    _land.owner = 0; //土地所有者清除
    _land.level = 0; //土地等级归为空地

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
    printf("\n");
    this->display_cur_play(this->current_player->name);
    printf("你卖出了此房产，获得收入：%d，当前资金：%d\n", m, this->current_player->fund);
}

void Game::Help()
{
    printf("\n帮助文档：\n");
    printf("命令                       功能说明                        参数说明\n");
    printf("\n");
    printf("Roll            掷骰子命令，行走1`6步，步数随机产生 。         无\n");
    printf("\n");
    printf("Sell n          轮到玩家时，可出售自己的任意房产，出售    n为房产的绝对位置\n");
    printf("                价格为投资总成本的2倍\n");
    printf("\n");
    printf("Block n         玩家可将路障放置到当前位置前后10部的任     n为前后相对距离\n");
    printf("                何位置，任一玩家经过将被拦截，一次有效       负数表示后方\n");
    printf("\n");
    printf("Robot           使用该道具将清除前方10步内任何其他道具\n");
    printf("\n");
    printf("Query           显示自家资产\n");
    printf("\n");
    printf("Help            查看命令帮助\n");
    printf("\n");
    printf("Quit            强制退出\n\n");
}

void Game::Game_Start()
{
    char last_name = 'Q';
    string name;

    while (true)
    {
        //system("cls");
        // this->Map_Display(last_name);

        while (true)
        {
            printf("\n");
            this->display_cur_play(this->current_player->name);

            if (this->current_player->isBankrupt())
                break;

            if (this->current_player->magic_days > 0)
            {
                this->current_player->magic_days--;
                printf("魔法生效中，%s本轮轮空，还剩%d轮魔法消失！\n", Get_charactor_name(this->current_player->name).c_str(), this->current_player->magic_days);

                if (this->land[this->current_player->location].owner != this->current_player->name && this->land[this->current_player->location].owner != 0)
                {
                    //交租
                    this->current_player->PayRent(this->land[this->current_player->location], this->player[Get_cur_player_index(this->land[this->current_player->location].owner)]);
                    //破产清算
                    Player_Bankrupt(this->current_player);
                }

                break;
            }

            string s;
            cin >> s;
            //读取命令行指令
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            if (s_Roll == s || s_Step == s) //前进：摇色子、指定步数
            {
                if (s_Roll == s)
                {
                    this->Roll();
                }
                else
                {
                    int n;
                    cin >> n;

                    //清除缓存
                    if (cin.rdstate())
                    {
                        printf("请输入正确格式的指令\n");
                        cin.clear();
                        cin.ignore(65535, '\n');
                        break;
                    }

                    this->Step(n);
                }

                switch (this->land[this->current_player->location].type)
                {
                case Land_Empty:
                    if (this->land[this->current_player->location].owner == 0)
                    {
                        this->display_cur_play(this->current_player->name);
                        this->current_player->BuyLand(this->land[this->current_player->location]);
                    }
                    else if (this->land[this->current_player->location].owner == this->current_player->name)
                    {
                        this->display_cur_play(this->current_player->name);
                        this->current_player->UpdateBuliding(this->land[this->current_player->location]);
                    }
                    else
                    {
                        this->display_cur_play(this->current_player->name);
                        this->current_player->PayRent(this->land[this->current_player->location], this->player[Get_cur_player_index(this->land[this->current_player->location].owner)]);

                        //破产清算
                        Player_Bankrupt(this->current_player);
                    }
                    break;
                case Land_ToolHouse:
                {
                    this->display_cur_play(this->current_player->name);
                    this->current_player->ToolHouse();
                    break;
                }
                case Land_GiftHouse:
                {
                    this->display_cur_play(this->current_player->name);
                    this->current_player->GiftHouse();
                    break;
                }
                case Land_MagicHouse:
                {
                    this->display_cur_play(this->current_player->name);
                    this->MagicHouse();
                    break;
                }
                case Land_Mine:
                {
                    this->display_cur_play(this->current_player->name);
                    this->current_player->Mine(this->land[this->current_player->location]);
                    break;
                }
                default:
                    break;
                }

                if (this->current_player->god_days > 0)
                {
                    this->current_player->god_days--;
                }

                break; // break退出此人循环
            }
            else if (s_Quit == s) //终止游戏
            {
                exit(0);
            }
            else if (s_Query == s) //查看玩家状态
            {
                this->current_player->CheckStatus();
            }
            else if (s_Sell == s) //出售地产
            {
                int n;
                cin >> n;

                //清除缓存
                if (cin.rdstate())
                {
                    printf("请输入正确格式的指令\n");
                    cin.clear();
                    cin.ignore(65535, '\n');
                    break;
                }

                this->SellBuliding(this->land[n]);
            }
            else if (s_Block == s) //使用障碍
            {
                char n[20];
                uint8_t i;
                i = 0;
                memset(n, 0, sizeof(n));
                scanf("%s", n);

                while (n[i])
                {
                    i++;
                }
                if (i == 1 && n[0] > '0' && n[0] <= '9')
                {
                    this->Lay_Block(n[0] - '0');
                }
                else if (i == 2 && n[0] >= '0' && n[0] <= '9' && n[1] >= '0' && n[1] <= '9')
                {
                    this->Lay_Block((n[0] - '0') * 10 + (n[1] - '0'));
                }
                else if (i == 2 && n[0] == '-' && n[1] >= '0' && n[1] <= '9')
                {
                    this->Lay_Block(-(n[1] - '0'));
                }
                else if (i == 3 && n[0] == '-' && n[1] >= '0' && n[1] <= '9' && n[2] >= '0' && n[2] <= '9')
                {
                    this->Lay_Block(-((n[1] - '0') * 10 + (n[2] - '0')));
                }
                else
                {
                    printf("请输入正确格式的指令\n");
                }
            }
            else if (s_Robot == s) //使用机器人
            {
                this->Lay_Robot();
            }
            else if (s_Help == s) //显示帮助信息
            {
                this->Help();
            }
            else if (s_Dump == s)
            {
                this->Dump();
            }
            else if (s_Preset == s)
            {
                string ss;
                cin >> ss;
                if (ss == string("user"))
                    this->Preset_user();
                else if (ss == string("map"))
                    this->Preset_map();
                else if (ss == string("fund"))
                {
                    this->Preset_fund();
                    Player_Bankrupt(this->current_player); //破产清算
                }
                else if (ss == string("credit"))
                    this->Preset_credit();
                else if (ss == string("gift"))
                    this->Preset_gift();
                else if (ss == string("barrier"))
                    this->Preset_barrier();
                else if (ss == string("userloc"))
                    this->Preset_userloc();
                else if (ss == string("nextuser"))
                {
                    this->Preset_nextuser();
                    break;
                }
            }
        }

        if (player_current_num == 1)
        {
            break;
        }
        last_name = this->current_player->name;
        this->Turn_to_next_Player();
    }

    this->Turn_to_next_Player();
    printf("%s获得胜利！\n", Get_charactor_name(this->current_player->name).c_str());
    return;
}

void Game::Preset_user()
{
    string n;
    cin >> n;
    int i = 0;
    player_total_num = 0;
    player_current_num = 0;
    for (string::iterator iter = n.begin(); iter != n.end(); iter++)
    {
        player_total_num++;
        player_current_num++;
        if (*iter == 'Q')
        {
            player_index_raw[i] = 0;
            player_index[i++] = 0;
        }
        else if (*iter == 'A')
        {
            player_index_raw[i] = 1;
            player_index[i++] = 1;
        }
        else if (*iter == 'S')
        {
            player_index_raw[i] = 2;
            player_index[i++] = 2;
        }
        else if (*iter == 'J')
        {
            player_index_raw[i] = 3;
            player_index[i++] = 3;
        }
    }

    //重新初始化地图
    Map_Init();
    //重新初始化人物，启动资金默认10000
    Player_Init(10000);

    current_player_i = player_current_num - 1;
}

void Game::Preset_map()
{
    char n[10];
    memset(n, 0, sizeof(n));
    scanf("%s", n);
    char c;
    cin >> c;
    int level;
    cin >> level;

    if (c != 'Q' && c != 'A' && c != 'S' && c != 'J')
    {
        printf("玩家%C不存在\n", c);
        return;
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("玩家%s不存在\n", Get_charactor_name(c).c_str());
        return;
    }

    uint8_t i = 0, n_w = 0;
    while (n[i])
    {
        i++;
    }
    for (int j = 1; j < i - 1; j++)
    {
        n_w += (n[j] - '0') * pow(10, (i - 2 - j));
    }

    if (land[n_w].type != Land_Empty)
    {
        printf("指定位置不是空地，不能买\n");
        return;
    }

    if (level < 0 || level > 3)
    {
        printf("指定房产等级不在范围内\n");
        return;
    }

    land[n_w].owner = c;
    land[n_w].level = level;

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
}

void Game::Preset_fund()
{
    char c;
    cin >> c;
    int fund;
    cin >> fund;

    if (c != 'Q' && c != 'A' && c != 'S' && c != 'J')
    {
        printf("玩家%C不存在\n", c);
        return;
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("玩家%s不存在\n", Get_charactor_name(c).c_str());
        return;
    }

    player[Get_cur_player_index(c)].fund = fund;
}

void Game::Preset_credit()
{
    char c;
    cin >> c;
    int credit;
    cin >> credit;

    if (c != 'Q' && c != 'A' && c != 'S' && c != 'J')
    {
        printf("玩家%C不存在\n", c);
        return;
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("玩家%s不存在\n", Get_charactor_name(c).c_str());
        return;
    }

    player[Get_cur_player_index(c)].credit = credit;
}

void Game::Preset_gift()
{
    char c;
    cin >> c;
    string tool;
    cin >> tool;
    int num;
    cin >> num;

    if (c != 'Q' && c != 'A' && c != 'S' && c != 'J')
    {
        printf("玩家%C不存在\n", c);
        return;
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("玩家%s不存在\n", Get_charactor_name(c).c_str());
        return;
    }

    if (tool == string("barrier"))
    {
        player[Get_cur_player_index(c)].tool->barrier = num;
    }
    else if (tool == string("robot"))
    {
        player[Get_cur_player_index(c)].tool->robot = num;
    }
    else if (tool == string("god"))
    {
        player[Get_cur_player_index(c)].god_days = num;
    }
    else
    {
        printf("不存在此道具\n");
    }
}

void Game::Preset_barrier()
{
    int n;
    cin >> n;

    if (n < 0 || n > 69)
    {
        printf("指定位置不是地，不能放\n");
        return;
    }

    land[n].isblock = 1;

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
}

void Game::Preset_userloc()
{
    char c;
    cin >> c;
    int loc;
    cin >> loc;
    int m_days;
    cin >> m_days;

    if (loc < 0 || loc > 69)
    {
        printf("指定位置不是地，去不了\n");
        return;
    }

    if (c != 'Q' && c != 'A' && c != 'S' && c != 'J')
    {
        printf("玩家%C不存在\n", c);
        return;
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("玩家%s不存在\n", Get_charactor_name(c).c_str());
        return;
    }

    player[Get_cur_player_index(c)].location = loc;
    player[Get_cur_player_index(c)].magic_days = m_days;

    //刷新屏幕
    system("cls");
    this->Map_Display(this->current_player->name);
}

void Game::Preset_nextuser()
{
    char c;
    cin >> c;

    if (c != 'Q' && c != 'A' && c != 'S' && c != 'J')
    {
        printf("玩家%C不存在\n", c);
        return;
    }

    uint8_t flag = 0;
    for (int i = 0; i < player_current_num; i++)
    {
        if (c == player[player_index[i]].name)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("玩家%s不存在\n", Get_charactor_name(c).c_str());
        return;
    }

    current_player = &(player[Get_cur_player_index(c)]); //当前玩家
    for (int i = 0, j = -1; i < player_current_num; i++, j++)
    {
        if (current_player->name == player[player_index[i]].name)
        {
            current_player_i = (j + player_current_num) % player_current_num;
            break;
        }
    }
}

void Game::Dump()
{
    FILE *fp = NULL;
    fp = fopen("dump.txt", "w+");
    fprintf(fp, "user ");
    for (int i = 0; i < player_total_num; i++)
    {
        fprintf(fp, "%c", this->player[this->player_index_raw[i]].name);
    }
    fprintf(fp, "\n");
    for (int i = 1; i <= player_total_num; i++)
    {
        for (int j = 0; j < LAND_NUM; j++)
        {
            if (this->land[j].owner == this->player[this->player_index_raw[i - 1]].name)
            {
                fprintf(fp, "map [%d] %c %d\n", j, this->player[this->player_index_raw[i - 1]].name, this->land[j].level);
            }
        }
    }
    for (int i = 0; i < player_total_num; i++)
    {
        fprintf(fp, "fund %c %d\n", this->player[this->player_index_raw[i]].name, this->player[this->player_index_raw[i]].fund);
    }
    for (int i = 0; i < player_total_num; i++)
    {
        fprintf(fp, "credit %c %d\n", this->player[this->player_index_raw[i]].name, this->player[this->player_index_raw[i]].credit);
    }
    for (int i = 0; i < player_total_num; i++)
    {
        fprintf(fp, "gift %c barrier %d\n", this->player[this->player_index_raw[i]].name, this->player[this->player_index_raw[i]].tool->barrier);
        fprintf(fp, "gift %c robot %d\n", this->player[this->player_index_raw[i]].name, this->player[this->player_index_raw[i]].tool->robot);
        fprintf(fp, "gift %c god %d\n", this->player[this->player_index_raw[i]].name, this->player[this->player_index_raw[i]].god_days);
    }
    for (int j = 0; j < LAND_NUM; j++)
    {
        if (this->land[j].isblock == 1)
        {
            fprintf(fp, "barrier %d\n", j);
        }
    }
    for (int i = 0; i < player_total_num; i++)
    {
        fprintf(fp, "userloc %c %d %d\n", this->player[this->player_index_raw[i]].name, this->player[this->player_index_raw[i]].location, this->player[this->player_index_raw[i]].magic_days);
    }
    fprintf(fp, "nextuser %c", this->current_player->name);
    fclose(fp);

    exit(0);
}