#pragma once

//���嶥��ṹ��
typedef struct tagVertex
{
	int row;	//��
	int col;	//��
	int info;	//��Ϣ��
}Vertex;

#define BLANK		-1	//��ʾ��ͼΪ��
#define MAX_ROW		10	//��Ϸ��ͼ����
#define MAX_COL		16	//��Ϸ��ͼ����
#define MAX_PIC_NUM 20	//ͼƬ��ɫ
#define MAP_TOP		50	//��Ϸ��ͼ���Ͻ�������
#define MAP_LEFT	50	//��Ϸ��ͼ���ϽǺ�����
#define MAP_WIDTH	40	//��ϷͼƬ���
#define MAP_HEIGHT	40	//��ϷͼƬ�߶�
#define PLAY_TIMER_ID 1	//��ʱ��ID
#define GAME_LOSE	-1	//ʧ��
#define GAME_WIN	0	//�ɹ�
#define GAME_PLAY	1	//���ڽ���
