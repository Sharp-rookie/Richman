#pragma once

#include <cstdint>
#include "Land.hpp"
#include "Player.hpp"

const int MAX_PLAYER_NUM = 4; //最大玩家数
const int LAND_NUM = 70;      //地图格数

class Game
{
public:
    uint8_t player_total_num;                 //玩家总量
    uint8_t player_current_num;               //剩余玩家数量
    uint8_t player_index_raw[MAX_PLAYER_NUM]; //最初所有玩家顺序索引
    uint8_t player_index[MAX_PLAYER_NUM];     //在场玩家顺序索引
    Player player[MAX_PLAYER_NUM];            //玩家对象组
    Land land[LAND_NUM];                      //地图对象组

    Player *current_player;                   //当前玩家
    uint8_t current_player_i;                 //当前玩家索引
    uint8_t Get_cur_player_index(char _name); //根据名字返回玩家对应索引
    string Get_charactor_name(char _name);    //根据玩家名字返回对应人名

    void Game_Start(); //开始游戏，解析命令行输入执行对应操作

    Game(){};
    Game(uint8_t total_num, uint8_t _order[], int init_fund); //初始化
    void Map_Init();                                          //初始化地图
    void Player_Init(int init_fund);                          //初始化玩家
    void Turn_to_next_Player();                               //切换到下一个玩家

    void Lay_Block(int loc);                   //使用障碍
    void Lay_Robot();                          //使用机器人
    void Map_Display(char _name);              //打印地图
    void display_loc(char _name, uint8_t loc); // 打印地图具体位置的字符
    void display_cur_play(char _name);         // 打印玩家名
    void Player_Bankrupt(Player *p);           //清算人物资产是否破产
    void Roll(Player p);                       //掷色子，移位
    void Help();                               //帮助信息
    void Roll();                               //掷色子
    void Step(int n);                          //指定步数
    void MagicHouse();                         //魔法屋
    void SellBuliding(Land &_land);            //卖房

    //测试功能
    void Preset_user();
    void Preset_map();
    void Preset_fund();
    void Preset_credit();
    void Preset_gift();
    void Preset_barrier();
    void Preset_userloc();
    void Preset_nextuser();
    void Dump();
};