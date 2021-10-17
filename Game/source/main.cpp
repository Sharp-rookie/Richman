#include "main.hpp"

int main()
{
    // system("chcp 65001"); //设置命令行窗口编码方式为 utf-8，防止乱码

    srand((unsigned)time(NULL));

    //帮助信息
    printf("\n帮助文档：\n");
    printf("命令                       功能说明                        参数说明\n");
    printf("\n");
    printf("Roll            掷骰子命令，行走1`6步，步数随机产生             无\n");
    printf("\n");
    printf("Sell n          轮到玩家时，可出售自己的任意房产，出售     n为房产的绝对位置\n");
    printf("                价格为投资总成本的2倍\n");
    printf("\n");
    printf("Block n         玩家可将路障放置到当前位置前后10部的任     n为前后相对距离\n");
    printf("                何位置，任一玩家经过将被拦截，一次有效     负数表示后方\n");
    printf("\n");
    printf("Robot           使用该道具将清除前方10步内任何其他道具\n");
    printf("\n");
    printf("Query           显示自家资产\n");
    printf("\n");
    printf("Help            查看命令帮助\n");
    printf("\n");
    printf("Quit            强制退出\n\n");

    //人物选择
    char read = 0;
    int8_t st[100];
    uint8_t count, user_num;
    printf("请选择2~4个角色：1.钱夫人 2.阿土伯 3.孙小美 4.金贝贝，可自由排序\n");
    while (true)
    {
        count = 0;
        read = getchar();
        while (read != '\n')
        {
            st[count++] = read - '0';
            read = getchar();
        }

        if (count > 4 || count < 2)
        {
            printf("请输入正确格式\n");
            continue;
        }
        uint8_t flag = 0;
        for (int i = 0; i < count; i++)
        {
            if (st[i] < 1 || st[i] > 9)
                flag = 1;
        }
        for (int i = 0; i < count - 1; i++)
        {
            if (st[i] == st[i + 1])
                flag = 1;
        }
        for (int i = 0; i < count - 2; i++)
        {
            if (st[i] == st[i + 2])
                flag = 1;
        }
        for (int i = 0; i < count - 3; i++)
        {
            if (st[i] == st[i + 3])
                flag = 1;
        }
        if (flag)
        {
            printf("请输入正确格式\n");
            continue;
        }
        else
            break;
    }
    uint8_t *charactor = new uint8_t[count];
    for (int i = 0; i < count; i++)
    {
        charactor[i] = st[i] - 1;
    }
    user_num = count;

    //开局资金选择
    int i_fund;
    printf("请输入启动资金，范围：1000~50000\n");
    while (true)
    {
        count = 0;
        i_fund = 0;
        read = getchar();
        while (read != '\n')
        {
            st[count++] = read - '0';
            read = getchar();
        }

        uint8_t flag = 0;
        for (int i = 0; i < count; i++)
        {
            if (st[i] < 0 || st[i] > 9)
                flag = 1;
        }
        for (uint8_t i = 0; i < count; i++)
        {
            i_fund += st[i] * n_10(count - i - 1);
            if (i_fund < 1000 || i_fund > 50000)
                flag = 1;
        }
        if (flag)
        {
            printf("请输入正确格式\n");
            continue;
        }
        else
            break;
    }

    //开启游戏
    Game *g = new Game(user_num, charactor, i_fund);
    g->Game_Start();

    return 0;
}