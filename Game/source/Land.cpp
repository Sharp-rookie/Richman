#include "Land.hpp"
#include "main.hpp"

Land::Land(LandType _type, int _origin_price)
{
    type = _type;
    owner = 0;
    level = 0;
    isblock = 0;
    origin_price = _origin_price;
    credit = 0;
}

int Land::Sell_price()
{
    if (type == Land_Empty && owner != 0)
        return origin_price * (level + 1) * 2;
    else
        return 0;
}