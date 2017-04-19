#pragma once

//定义顶点结构体
typedef struct tagVertex
{
	int row;	//行
	int col;	//列
	int info;	//信息类
}Vertex;

#define BLANK		-1	//表示地图为空
#define MAX_ROW		10	//游戏地图行数
#define MAX_COL		16	//游戏地图列数
#define MAX_PIC_NUM 20	//图片花色
#define MAP_TOP		50	//游戏地图左上角纵坐标
#define MAP_LEFT	50	//游戏地图左上角横坐标
#define MAP_WIDTH	40	//游戏图片宽度
#define MAP_HEIGHT	40	//游戏图片高度
#define PLAY_TIMER_ID 1	//计时器ID
#define GAME_LOSE	-1	//失败
#define GAME_WIN	0	//成功
#define GAME_PLAY	1	//正在进行
