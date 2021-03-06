# Richman
19级种子班软件工程与项目管理课设——大富翁游戏开发



#### 简介

&emsp;&emsp;大富翁系列是由大宇资讯制作以休闲轻松为主的pc游戏，于1989年11月28日发行，截至目前已更新到第9版。大富翁游戏发行至今已成为一款家喻户晓的知名游戏，甚至已成为一代代人们心中的童年回忆。为实践敏捷开发模式，种子班的软件工程与项目管理课程历来要求学生分组合作简易版大富翁游戏的开发。

&nbsp;

#### 游戏规则

&emsp;&emsp;地图大小为70格，其中分布有空地、道具店、礼品店、魔法屋、公园、矿地共6种地形。玩家们从起点出发，每回合投掷色子以前进相应步数。本游戏包含：房产机制、道具机制、礼品机制、魔法机制。当玩家资金小于0时破产，全场仅剩下一位玩家时，该玩家获胜，游戏结束。

1. **房产机制**

   购买：

   每位玩家有初始资金，当来到无他人资产的空地时可选择购买该地为自己的资产，游戏中分为3种地段，不同地段的空地价格有所不同。

   升级：

   玩家再一次来到自己资产的空格时，可选择升级该资产，升级价格为该空地原始价格。初始等级为0，最高可升至3级，分别对应：0—空地、1—茅房、2—洋房、3—摩天大楼。一回合只能升级一次。

   收租：

   当有玩家来到你名下（或你来到其他玩家）的资产时，需上交租金。租金为该资产总投资价值的一半。

   出售：

   玩家在自己回合可选择出售自己的资产，售价为总投资价格的2倍。

2. **道具机制**

   玩家可以使用点数在道具屋购买道具。道具分两种：

   ​	路障：阻拦经过其的任何玩家使停在该处，生效一次后消失

   ​	机器人：清除使用者前方10步的所有路障，生效一次后消失

3. **礼品机制**

   玩家来到礼品屋时可选择礼品：

   ​	支票：资金 +2000

   ​	点卡：点数 +200

   ​	财神：接下来5回合内不需要上交租金

4. **魔法机制**

   玩家来到礼品屋时可指定在场任一玩家发动魔法，使其接下来2回合无法进行任何行动，可叠加。

&nbsp;

#### 游戏功能

###### 开机选项

选择角色：1.钱夫人 2.阿土伯 3.孙小美 4.金贝贝，可自由排序，至少2人

选择开局资金：范围：1000~50000

###### 回合行动

| 指令    | 功能                                             |
| ------- | ------------------------------------------------ |
| Roll    | 掷骰子，随机行走1~6步                            |
| Sell n  | 出售自己的房产，n为房产位置，售价为投资总值的2倍 |
| Block n | 放置路障到相对自身n步位置，n范围-10~10           |
| Robot   | 使用机器人                                       |
| Query   | 显示自身当前资产（资金、点数、道具）             |
| Help    | 查看命令帮助文档                                 |
| Quit    | 强制退出                                         |

&nbsp;

#### 目录结构

Game为游戏目录，TDD为自动化测试目录

&nbsp;

#### 使用说明

Game下有可执行文件`Rich.exe`，可直接运行或自行编译

TDD下main.py为自动化测试脚本，gen.py为自动化生成测试样例脚本

&nbsp;

#### 开发人员

lrk、yy、djk、pp、wcy、lzs、zzw
