#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>
#include<Windows.h>
#include<conio.h>




#define SIZE 8
#define FilePath "maxs.dat"					//文件存储
const char comp_c = 'X';					//电脑玩家
const char player_c = 'O';					//自己


int max = 0;								//确定最高分
char maxs[10];
int temp;


int readMaxs(char *path);					//读写文件
void writeMaxs(int max, char *path);

void Pos(int x, int y);						//设置光标位置
void display(char board[][SIZE]);			//棋盘展示
void reset_board(char board[][SIZE]);		//重置棋盘
int valid_moves(char board[][SIZE], bool moves[][SIZE], char player);     //判断有效移动
int player_counters(char board[][SIZE], char player);					  //电脑如何移动
void make_move(char board[][SIZE], size_t row, size_t col, char player);  //移动
void computer_move(char board[][SIZE], bool moves[][SIZE], char player);  //电脑移动,最优
int best_move(char board[][SIZE], bool moves[][SIZE], char player);		  //最好的移动
int get_score(char board[][SIZE], char player);							  //最优得分

																		  //展示界面
void flash();
void rank();
void ruel();
//文件的读写
int readMaxs(char *path)
{
	FILE *fp;			//文件指针
	int max;
	if ((fp = fopen(path, "r")) == NULL)
		return 0;
	fscanf(fp, "%d", &max);
	fclose(fp);
	return max;
}

void writeMaxs(int max, char *path)
{

	FILE *fp;
	if ((fp = fopen(path, "w")) == NULL)
		return;

	fprintf(fp, "%d", max);

	temp = max;
	fclose(fp);
}




//游戏
int game(void)			//主界面
{
	FILE *fp;
	//两个棋盘,一个原始棋盘,一个布尔棋盘
	char board[SIZE][SIZE] = { 0 };			//二维数组,原始棋盘,检查是否有空的棋盘
	bool moves[SIZE][SIZE] = { false };		//记录有效步数
	int no_moves = 0;						//
	int invalid_moves = 0;
	char again = 0;
	char y = 0;
	size_t x = 0;
	int s = 0;
	int key = 0;

	//主界面

	bool next_player = true;
	//两个循环,让电脑和玩家轮流下
	//游戏主循环界面

	do
	{

		reset_board(board);				//初始的棋盘
		next_player = !next_player;

		no_moves = 4;			//计算棋子数

								//游戏开始
		do

		{

			system("cls");

			display(board);				//绘制棋盘

			Sleep(200);

			if (true == (next_player = !next_player))  //下一个对象,双方轮换下棋
			{
				printf("按ESC退出游戏,按任意键继续游戏");
				int ch;
				ch = getch();
				if (27 == ch)
				{
					s = player_counters(board, player_c); //比较最高分

					if (s > temp)
					{
						max = s;
						writeMaxs(max, (char*)FilePath);
					}
					return 0;
				}
				if (valid_moves(board, moves, player_c)) //判断玩家的有效下子
				{
					for (;;)
					{

						printf("\n请输入行(数字)和列(字母):");
						getchar();
						scanf("%d %c", &x, &y);
						fflush(stdin);   //返回值为正值,读取
										 //将其加减可以转换为索引值
						y = tolower(y) - 'a';
						--x;
						if (y < 0 || y >= SIZE || x >= SIZE || !moves[x][y])
						{
							//如果下棋位置不在数组里面,或者在棋盘范围之外
							fflush(stdin);
							system("cls");
							display(board);
							printf("\n亲,你走错了哦,再试试吧.\n");
							printf("按ESC退出游戏,按任意键继续游戏");
							int ch;
							ch = getch();
							if (27 == ch)
							{
								s = player_counters(board, player_c); //比较最高分
								if (s > temp)
								{
									max = s;
									writeMaxs(max, (char*)FilePath);
								}
								return 0;
							}
							continue;
						}
						//输出记录
						make_move(board, x, y, player_c);
						++no_moves;
						//每移动一次,就+1
						break;
					}
				}
				else
				{
					//有效棋盘小于2 结束游戏  
					if (++invalid_moves < 2)
					{
						printf("\n怎么破,没有地方可以下了耶,让我们跳过一个回合吧");
						scanf("%c", &again, sizeof(again));
						fflush(stdin);
					}
					else
						printf("\n双方皆无棋可下，故游戏结束");
				}
			}

			else
			{
				//电脑下棋
				if (valid_moves(board, moves, comp_c))
				{
					invalid_moves = 0;
					computer_move(board, moves, comp_c); //电脑移动
					++no_moves;
				}
				else
				{
					if (++invalid_moves < 2)
						printf("\n 我没有位置下棋啦");
					else
						printf("\n都下不了棋子了耶,游戏结束了哦");
				}
			}
		} while (no_moves < SIZE*SIZE&&invalid_moves < 2); //游戏结束
		display(board);									//展示结束的棋盘
		printf("最终得分是：\n");

		s = player_counters(board, player_c); //比较最高分
		if (s > temp)
		{
			max = s;
			writeMaxs(max, (char*)FilePath);
		}
		printf("电脑的分数 %d\n 玩家的分数 %d \n\n", player_counters(board, comp_c), player_counters(board, player_c));
		if (player_counters(board, comp_c) > player_counters(board, player_c))
			printf("电脑赢了哦\n");
		else
			printf("完美，你赢拉\n");

		printf("您想要再来一局么(y/n):");
		scanf(" %c", &again, sizeof(again));
		fflush(stdin);
	} while (tolower(again) == 'y');
	printf("\nGoodbye~\n");
	return 0;
}
//定义起始的棋盘.
void reset_board(char board[][SIZE])
{

	//重置棋盘,将所有棋盘变成空格
	for (size_t row = 0; row < SIZE; ++row)
		for (size_t col = 0; col < SIZE; ++col)
		{
			board[row][col] = ' ';
		}
	//根据定义的SIZE将四个棋子放在中间
	size_t mid = SIZE / 2;
	board[mid - 1][mid - 1] = board[mid][mid] = player_c;
	board[mid - 1][mid] = board[mid][mid - 1] = comp_c;
}


//输入绘制的棋盘
void display(char board[][SIZE])
{

	int max, com, play;
	max = readMaxs((char*)FilePath);
	com = player_counters(board, comp_c);
	play = player_counters(board, player_c);
	int row = 0;
	int col = 0;
	char col_label = 'a';

	printf("\n ");
	for (int i = 0; i < SIZE; i++)
	{
		Pos(1, 3);
		for (size_t col = 0; col < SIZE; ++col)
			printf("   %c", (char)(col_label + col));

	}
	printf("\n");

	for (row = 0; row<SIZE; row++)
	{
		printf("  +");
		for (col = 0; col<SIZE; col++)
			printf("---+");
		printf("\n%2d|", row + 1);

		for (col = 0; col<SIZE; col++)
			printf(" %c |", board[row][col]);
		printf("\n");
	}

	printf("  +");
	for (col = 0; col<SIZE; col++)
		printf("---+");
	printf("\n");
	Pos(3, 1);
	system("date /T");
	Pos(3, 2);
	system("TIME /T");

	Pos(46, 1);  printf("╔════════════════════════════════╗");
	Pos(46, 2);  printf("║          黑白棋人机大战      ");
	Pos(46, 3);  printf("╠════════════════════════════════╣");
	Pos(46, 4);  printf("║   请输入对应的行和列开始下棋 ");
	Pos(46, 5);  printf("║                              ");
	Pos(46, 6);  printf("║                              ");
	Pos(46, 7);  printf("║       电脑的分数:  %d        ", com);
	Pos(46, 8);  printf("╠════════════════════════════════╣");
	Pos(46, 9);  printf("║       玩家的分数:  %d        ", play);
	Pos(46, 10); printf("║                              ");
	Pos(46, 11); printf("║                              ");
	Pos(46, 12); printf("║                              ");
	Pos(46, 13); printf("╠════════════════════════════════╣");
	Pos(46, 14); printf("║      玩家最高分:             ");
	Pos(46, 15); printf("║             %d               ", max);
	Pos(46, 16); printf("║                              ");
	Pos(46, 17); printf("║                              ");
	Pos(46, 18); printf("║                              ");
	Pos(46, 19); printf("║                              ");
	Pos(46, 20); printf("╚════════════════════════════════╝");

}

/*函数的主要功能是用检查玩家输入的棋子是否有效,
通过使用两个棋盘来存储信息,一个board用来检查是
否存在空格,moves用来保存移动*/
int valid_moves(char board[][SIZE], bool moves[][SIZE], char player)
{
	int rowdelta = 0;				//行的空间
	int coldelta = 0;				//列的空间
	int x = 0;						//行搜索
	int y = 0;						//列搜索
	int no_moves = 0;			   //无效移动

	char opponent = (player == player_c) ? comp_c : player_c;//判断对手

															 //使有效移动变成false
	for (size_t row = 0; row < SIZE; ++row)
		for (size_t col = 0; col < SIZE; ++col)
			moves[row][col] = false;

	//寻找有效的移动方式,遍历整个棋盘
	for (size_t row = 0; row < SIZE; ++row)
	{
		for (size_t col = 0; col < SIZE; ++col)
		{
			//如何找到的是空白棋子,选择下一个位置
			if (board[row][col] != ' ')
				continue;
			//检查所有对手下过的空间
			for (rowdelta = -1; rowdelta <= 1; ++rowdelta)  //行
			{
				for (coldelta = -1; coldelta <= 1; ++coldelta) //列
				{
					//防止遍历的范围超过二维数组的空间,然后继续遍历
					if ((row == 0 && rowdelta == -1) || row + rowdelta >= SIZE ||
						(col == 0 && coldelta == -1) || col + coldelta >= SIZE ||
						(rowdelta == 0 && coldelta == 0))
						continue;

					//判断对手所在的位置
					if (board[row + rowdelta][col + coldelta] == opponent)
					{
						x = row + rowdelta; //移动在他的附近
						y = col + coldelta; //选择空间
											//寻找对手周围的空间
						for (;;)
						{
							x += rowdelta;
							y += coldelta;

							//如果超出数组范围,将结束寻找
							if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y] == ' ')
								break;
							if (board[x][y] == player)
							{
								moves[row][col] = true;
								no_moves++;
								break;
							}
						}
					}
				}
			}
		}
	}
	return no_moves;
}

///移动方式
void make_move(char board[][SIZE], size_t row, size_t col, char player)
{
	int rowdelta = 0;						// 行
	int coldelta = 0;						//列
	size_t x = 0;							//搜索XY.
	size_t y = 0;
	char opponent = (player == player_c) ? comp_c : player_c;		//确定
	board[row][col] = player;		//计数

									//确定所有对手在棋盘中的移动
	for (rowdelta = -1; rowdelta <= 1; ++rowdelta)  //嵌套索引
	{
		for (coldelta = -1; coldelta <= 1; ++coldelta)
		{
			if ((row == 0 && rowdelta == -1) || row + rowdelta >= SIZE ||
				(col == 0 && coldelta == -1) || col + coldelta >= SIZE ||
				(rowdelta == 0 && coldelta == 0))
				continue;
			//找到对手棋子
			if (board[row + rowdelta][col + coldelta] == opponent)
			{
				x = row + rowdelta;  //移动
				y = col + coldelta;
				for (;;)
				{
					x += rowdelta;	//再次移动
					y += coldelta;
					if (x >= SIZE || y >= SIZE || board[x][y] == ' ')// 如果超出空间,则结束移动.
						break;
					//如果找到对手则在其附近找位置，将对手的棋子变成自己的
					if (board[x][y] == player)
					{
						//确定下棋棋子
						while (board[x -= rowdelta][y -= coldelta] == opponent)//是不是对手的棋子

							board[x][y] = player; //将他改变，变成自己的
						break;
					}
				}
			}
		}
	}
}
int get_score(char board[][SIZE], char player)			//分数统计
{
	return player_counters(board, player) - player_counters(board, (player == player_c) ? comp_c : player_c);
}

int best_move(char board[][SIZE], bool moves[][SIZE], char player) //最好的移动方式
{
	char new_board[SIZE][SIZE] = { 0 };     //复制一个新棋盘
	int score = 0;							//最好分数
	int new_score = 0;						//现在的分数
	for (size_t row = 0; row < SIZE; ++row)
	{
		for (size_t col = 0; col < SIZE; ++col)
		{
			if (!moves[row][col])			//如果没有遇到有效的移动
				continue;

			//复制棋盘
			memcpy(new_board, board, sizeof(new_board));
			//得到的移动在棋盘上
			make_move(new_board, row, col, player);
			//比较分数
			new_score = get_score(new_board, player);
			if (score < new_score)
				score = new_score;
		}
	}
	return score;
}

void computer_move(char board[][SIZE], bool moves[][SIZE], char player)		//电脑移动的方式
{
	size_t best_row = 0;					//最好的索引
	size_t best_col = 0;
	int new_score = 0;						//新的分数
	int score = SIZE * SIZE;					//分数最大值
	char temp_board[SIZE][SIZE];			//复制的棋盘
	bool temp_moves[SIZE][SIZE];			//有效移动的棋盘
											//确认对手
	char opponent = (player == player_c) ? comp_c : player_c;

	//经过所有有效的移动去寻找最佳的算法
	for (size_t row = 0; row < SIZE; ++row)
	{
		for (size_t col = 0; col < SIZE; ++col)
		{
			if (!moves[row][col])
				continue;
			//复制棋盘数组
			memcpy(temp_board, board, SIZE*SIZE);

			//将移动的位置复制到这个棋盘中
			make_move(temp_board, row, col, player);

			//每次下子都需要调用这个函数,去寻找有效的移动在对手移动后
			valid_moves(temp_board, temp_moves, opponent);

			//比较棋盘的分数,从而选择一个最佳的移动方式
			new_score = best_move(temp_board, temp_moves, opponent);
			if (new_score < score)
			{
				//保存移动方式
				score = new_score;
				best_row = row;
				best_col = col;
			}
		}
	}
	make_move(board, best_row, best_col, player);
}

//步数统计
int player_counters(char board[][SIZE], char player)
{
	int count = 0;
	for (size_t row = 0; row < SIZE; ++row)
		for (size_t col = 0; col < SIZE; ++col)
			if (board[row][col] == player)
				++count;
	return count;
}


//设置光标位置
void Pos(int x, int y)
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}



int main()
{

	int i;

	while (1)
	{
		flash();

		system("title 欢迎来到黑白棋人机大战的世界1.0");//设置cmd窗口标题
		system("mode con cols=80 lines=40");//窗口宽度高度
		system("color F4");					//颜色
		Pos(3, 1);
		system("date /T");
		Pos(3, 2);
		system("TIME /T");

		Pos(23, 5);  printf("╔══════════════════════════════════╗");
		Pos(23, 6);  printf("║      欢迎来到黑白棋的世界        ║");
		Pos(23, 7);  printf("╠══════════════════════════════════╣");
		Pos(23, 8);  printf("║       1.进入游戏                 ║");
		Pos(23, 9);  printf("╠══════════════════════════════════╣");
		Pos(23, 10); printf("║       2.最高分记录               ║");
		Pos(23, 11); printf("╠══════════════════════════════════╣");
		Pos(23, 12); printf("║       3.游戏说明                 ║");
		Pos(23, 13); printf("╠══════════════════════════════════╣");
		Pos(23, 14); printf("║       0.退出游戏                 ║");
		Pos(23, 15); printf("╠══════════════════════════════════╣");
		Pos(23, 16); printf("║                                  ║");
		Pos(23, 17); printf("║                                  ║");
		Pos(23, 18); printf("║                     by:大佬带带我║");
		Pos(23, 19); printf("╚══════════════════════════════════╝\n\n");
		i = getchar() - '0';

		system("cls");

		switch (i)
		{
		case 1:	//进入游戏

			game();

			break;
		case 2:	//查看记录
			rank();
			break;
		case 3://游戏说明
			ruel();
			break;
		case 0:
			return 0;
		default:
			printf("\n请选择正确的操作哦~!");
			break;
		}
	}
}

void rank() //排行
{
	max = readMaxs((char*)FilePath);
	int i;
	while (1)
	{
		int c;

		Pos(3, 1);
		system("date /T");
		Pos(3, 2);
		system("TIME /T");
		Pos(23, 5);  printf("╔══════════════════════════════════╗");
		Pos(23, 6);  printf("║                                  ║");
		Pos(23, 7);  printf("╠══════════════════════════════════╣");
		Pos(23, 8);  printf("║                                  ║");
		Pos(23, 9);  printf("╠══════════════════════════════════╣");
		Pos(23, 10); printf("║        玩家最高分:  %d            ║", max);
		Pos(23, 11); printf("╠══════════════════════════════════╣");
		Pos(23, 12); printf("║        输入0返回主页             ║");
		Pos(23, 13); printf("╠══════════════════════════════════╣");
		Pos(23, 14); printf("║                                  ║");
		Pos(23, 15); printf("║                                  ║");
		Pos(23, 16); printf("║                     by:大佬带带我║");
		Pos(23, 17); printf("╚══════════════════════════════════╝\n\n");
		Pos(23, 18); printf("\t\t\t\n\n");
		i = getche() - '0';
		switch (i)
		{
		case 0:
			main();
			break;
		default:
			printf("\n请选择正确的操作哦~!");
			break;
		}
	}
}
//游戏操作介绍
void ruel()
{
	system("cls");
	printf("\t\t\t\t   【游戏介绍】\n\t\t\t        按任意键返回菜单\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t游戏操作说明\n");
	printf("\t\t\t\t1、该棋盘为8*8，,输入行(数字)和列(字母)\n");
	printf("\t\t\t\t2、棋盘上谁的棋子多，谁得分就高，谁就获胜\n");
	printf("\t\t\t\t3、棋盘上双方棋子各有多少，谁得分就多少\n");
	printf("\t\t\t\t4、棋盘正中双方各2枚棋子，双方轮流下棋\n");
	printf("\t\t\t\t5、棋盘上无子可下，进入下一回合\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t游戏注意事项\n");
	printf("\t\t\t\t如果下棋位置不在数组里面,或者在棋盘范围之外\n");
	printf("\t\t\t\t\t 表明走错了，重新输入行列\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t最高分说明\n");
	printf("\t\t\t\t游戏只收录了分数最高分\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t游戏结束说明\n");
	printf("\t\t\t\t1、有效棋盘小于2，双方无棋可下，跳下一回合\n");
	printf("\t\t\t\t2、电脑和玩家任一的棋子被翻完时，结束游戏\n");
	_getch();
}

//开场动画
void flash()
{
	system("title 欢迎来到黑白棋人机大战的世界1.0");//设置cmd窗口标题
	system("mode con cols=80 lines=40");//窗口宽度高度
	system("color F4");					//颜色
	const char *cnm[15];
	system("color F4");
	cnm[0] = "　　　   ┏┓     ┏┓\n";
	cnm[1] = "　　　　┏┛┻━━━━━┻┗┓\n";
	cnm[2] = "　　　　┃　　　　　┃\n";
	cnm[3] = "　　　　┃　　 ━    ┃\n";
	cnm[4] = "　　　　┃　┳┛　┗┳　┃\n";
	cnm[5] = "　　　　┃　　　　  ┃\n";
	cnm[6] = "　　　　┃　　┻　   ┃\n";
	cnm[7] = "　　　　┗━┓　　  ┏━┛\n";
	cnm[8] = "　　　　  ┃　    ┃    \n";
	cnm[9] = "　　  ┏━━━┛　　  ┃    \n";
	cnm[10] = "　   ┏┫　　　　  ┃    \n";
	cnm[11] = "　   ┗┓　   　   ┃    \n";
	cnm[12] = "　　  ┗┓┏┳━━━━┓┏┏┛    \n";
	cnm[13] = "　　　 ┣┣┃　  ┣┣┃      \n";
	cnm[14] = "　　　 ┗┻┛　  ┗┻┛      \n";
	int i = 0, j = 0, t = 0;
	while (t < 15)
	{
		system("cls");
		i = 0;
		while (i < 15)
		{
			j = 0;
			while (j < t)
			{
				printf("     ");
				j++;
			}
			printf("%s", cnm[i]);
			i++;
		} //改动的地方

		Sleep(200);
		t++;
	}

}
