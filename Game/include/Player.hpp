#pragma once

#include <cstdio>
#include <cstdint>
#include <string>
#include "Land.hpp"

using namespace std;

struct Tool
{
    // uint8_t bomb;
    uint8_t barrier;
    uint8_t robot;
};

enum class Color
{
    Red,
    Green,
    Yellow,
    Blue,
};

class Player
{
public:
    char name;          //人物名
    int fund;           //金币数
    int credit;         //点数
    int8_t location;    //位置
    struct Tool *tool;  //道具
    uint8_t god_days;   //财神剩余天数
    uint8_t magic_days; //魔法剩余天数
    bool bankrupt;      //是否破产

    Color color; // 颜色

    Player(uint8_t _name = 0, int _fund = 0); //初始化

    bool isBankrupt();  //查询是否破产
    void CheckStatus(); //查询状态

    void UpdateBuliding(Land &_land);          //升级建筑
    void ToolWork();                           //进入道具位置，道具生效
    void BuyLand(Land &_land);                 //进入空地，买空地
    void PayRent(Land &_land, Player &_owner); //进入他人房产，付租金
    void ToolHouse();                          //进入工具屋
    void GiftHouse();                          //进入礼物屋
    void Mine(Land _land);                     //到达矿地
    void Buy_Tool(int tool_type);              //买道具

    void colorfulPrintf(char c);   // 带颜色输出
    void colorfulPrintf(string s); //带颜色输出
};
