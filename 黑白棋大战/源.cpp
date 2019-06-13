#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>
#include<Windows.h>
#include<conio.h>




#define SIZE 8
#define FilePath "maxs.dat"					//�ļ��洢
const char comp_c = 'X';					//�������
const char player_c = 'O';					//�Լ�


int max = 0;								//ȷ����߷�
char maxs[10];
int temp;


int readMaxs(char *path);					//��д�ļ�
void writeMaxs(int max, char *path);

void Pos(int x, int y);						//���ù��λ��
void display(char board[][SIZE]);			//����չʾ
void reset_board(char board[][SIZE]);		//��������
int valid_moves(char board[][SIZE], bool moves[][SIZE], char player);     //�ж���Ч�ƶ�
int player_counters(char board[][SIZE], char player);					  //��������ƶ�
void make_move(char board[][SIZE], size_t row, size_t col, char player);  //�ƶ�
void computer_move(char board[][SIZE], bool moves[][SIZE], char player);  //�����ƶ�,����
int best_move(char board[][SIZE], bool moves[][SIZE], char player);		  //��õ��ƶ�
int get_score(char board[][SIZE], char player);							  //���ŵ÷�

																		  //չʾ����
void flash();
void rank();
void ruel();
//�ļ��Ķ�д
int readMaxs(char *path)
{
	FILE *fp;			//�ļ�ָ��
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




//��Ϸ
int game(void)			//������
{
	FILE *fp;
	//��������,һ��ԭʼ����,һ����������
	char board[SIZE][SIZE] = { 0 };			//��ά����,ԭʼ����,����Ƿ��пյ�����
	bool moves[SIZE][SIZE] = { false };		//��¼��Ч����
	int no_moves = 0;						//
	int invalid_moves = 0;
	char again = 0;
	char y = 0;
	size_t x = 0;
	int s = 0;
	int key = 0;

	//������

	bool next_player = true;
	//����ѭ��,�õ��Ժ����������
	//��Ϸ��ѭ������

	do
	{

		reset_board(board);				//��ʼ������
		next_player = !next_player;

		no_moves = 4;			//����������

								//��Ϸ��ʼ
		do

		{

			system("cls");

			display(board);				//��������

			Sleep(200);

			if (true == (next_player = !next_player))  //��һ������,˫���ֻ�����
			{
				printf("��ESC�˳���Ϸ,�������������Ϸ");
				int ch;
				ch = getch();
				if (27 == ch)
				{
					s = player_counters(board, player_c); //�Ƚ���߷�

					if (s > temp)
					{
						max = s;
						writeMaxs(max, (char*)FilePath);
					}
					return 0;
				}
				if (valid_moves(board, moves, player_c)) //�ж���ҵ���Ч����
				{
					for (;;)
					{

						printf("\n��������(����)����(��ĸ):");
						getchar();
						scanf("%d %c", &x, &y);
						fflush(stdin);   //����ֵΪ��ֵ,��ȡ
										 //����Ӽ�����ת��Ϊ����ֵ
						y = tolower(y) - 'a';
						--x;
						if (y < 0 || y >= SIZE || x >= SIZE || !moves[x][y])
						{
							//�������λ�ò�����������,���������̷�Χ֮��
							fflush(stdin);
							system("cls");
							display(board);
							printf("\n��,���ߴ���Ŷ,�����԰�.\n");
							printf("��ESC�˳���Ϸ,�������������Ϸ");
							int ch;
							ch = getch();
							if (27 == ch)
							{
								s = player_counters(board, player_c); //�Ƚ���߷�
								if (s > temp)
								{
									max = s;
									writeMaxs(max, (char*)FilePath);
								}
								return 0;
							}
							continue;
						}
						//�����¼
						make_move(board, x, y, player_c);
						++no_moves;
						//ÿ�ƶ�һ��,��+1
						break;
					}
				}
				else
				{
					//��Ч����С��2 ������Ϸ  
					if (++invalid_moves < 2)
					{
						printf("\n��ô��,û�еط���������Ү,����������һ���غϰ�");
						scanf("%c", &again, sizeof(again));
						fflush(stdin);
					}
					else
						printf("\n˫����������£�����Ϸ����");
				}
			}

			else
			{
				//��������
				if (valid_moves(board, moves, comp_c))
				{
					invalid_moves = 0;
					computer_move(board, moves, comp_c); //�����ƶ�
					++no_moves;
				}
				else
				{
					if (++invalid_moves < 2)
						printf("\n ��û��λ��������");
					else
						printf("\n���²���������Ү,��Ϸ������Ŷ");
				}
			}
		} while (no_moves < SIZE*SIZE&&invalid_moves < 2); //��Ϸ����
		display(board);									//չʾ����������
		printf("���յ÷��ǣ�\n");

		s = player_counters(board, player_c); //�Ƚ���߷�
		if (s > temp)
		{
			max = s;
			writeMaxs(max, (char*)FilePath);
		}
		printf("���Եķ��� %d\n ��ҵķ��� %d \n\n", player_counters(board, comp_c), player_counters(board, player_c));
		if (player_counters(board, comp_c) > player_counters(board, player_c))
			printf("����Ӯ��Ŷ\n");
		else
			printf("��������Ӯ��\n");

		printf("����Ҫ����һ��ô(y/n):");
		scanf(" %c", &again, sizeof(again));
		fflush(stdin);
	} while (tolower(again) == 'y');
	printf("\nGoodbye~\n");
	return 0;
}
//������ʼ������.
void reset_board(char board[][SIZE])
{

	//��������,���������̱�ɿո�
	for (size_t row = 0; row < SIZE; ++row)
		for (size_t col = 0; col < SIZE; ++col)
		{
			board[row][col] = ' ';
		}
	//���ݶ����SIZE���ĸ����ӷ����м�
	size_t mid = SIZE / 2;
	board[mid - 1][mid - 1] = board[mid][mid] = player_c;
	board[mid - 1][mid] = board[mid][mid - 1] = comp_c;
}


//������Ƶ�����
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

	Pos(46, 1);  printf("�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[");
	Pos(46, 2);  printf("�U          �ڰ����˻���ս      ");
	Pos(46, 3);  printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
	Pos(46, 4);  printf("�U   �������Ӧ���к��п�ʼ���� ");
	Pos(46, 5);  printf("�U                              ");
	Pos(46, 6);  printf("�U                              ");
	Pos(46, 7);  printf("�U       ���Եķ���:  %d        ", com);
	Pos(46, 8);  printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
	Pos(46, 9);  printf("�U       ��ҵķ���:  %d        ", play);
	Pos(46, 10); printf("�U                              ");
	Pos(46, 11); printf("�U                              ");
	Pos(46, 12); printf("�U                              ");
	Pos(46, 13); printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
	Pos(46, 14); printf("�U      �����߷�:             ");
	Pos(46, 15); printf("�U             %d               ", max);
	Pos(46, 16); printf("�U                              ");
	Pos(46, 17); printf("�U                              ");
	Pos(46, 18); printf("�U                              ");
	Pos(46, 19); printf("�U                              ");
	Pos(46, 20); printf("�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a");

}

/*��������Ҫ�������ü���������������Ƿ���Ч,
ͨ��ʹ�������������洢��Ϣ,һ��board���������
����ڿո�,moves���������ƶ�*/
int valid_moves(char board[][SIZE], bool moves[][SIZE], char player)
{
	int rowdelta = 0;				//�еĿռ�
	int coldelta = 0;				//�еĿռ�
	int x = 0;						//������
	int y = 0;						//������
	int no_moves = 0;			   //��Ч�ƶ�

	char opponent = (player == player_c) ? comp_c : player_c;//�ж϶���

															 //ʹ��Ч�ƶ����false
	for (size_t row = 0; row < SIZE; ++row)
		for (size_t col = 0; col < SIZE; ++col)
			moves[row][col] = false;

	//Ѱ����Ч���ƶ���ʽ,������������
	for (size_t row = 0; row < SIZE; ++row)
	{
		for (size_t col = 0; col < SIZE; ++col)
		{
			//����ҵ����ǿհ�����,ѡ����һ��λ��
			if (board[row][col] != ' ')
				continue;
			//������ж����¹��Ŀռ�
			for (rowdelta = -1; rowdelta <= 1; ++rowdelta)  //��
			{
				for (coldelta = -1; coldelta <= 1; ++coldelta) //��
				{
					//��ֹ�����ķ�Χ������ά����Ŀռ�,Ȼ���������
					if ((row == 0 && rowdelta == -1) || row + rowdelta >= SIZE ||
						(col == 0 && coldelta == -1) || col + coldelta >= SIZE ||
						(rowdelta == 0 && coldelta == 0))
						continue;

					//�ж϶������ڵ�λ��
					if (board[row + rowdelta][col + coldelta] == opponent)
					{
						x = row + rowdelta; //�ƶ������ĸ���
						y = col + coldelta; //ѡ��ռ�
											//Ѱ�Ҷ�����Χ�Ŀռ�
						for (;;)
						{
							x += rowdelta;
							y += coldelta;

							//����������鷶Χ,������Ѱ��
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

///�ƶ���ʽ
void make_move(char board[][SIZE], size_t row, size_t col, char player)
{
	int rowdelta = 0;						// ��
	int coldelta = 0;						//��
	size_t x = 0;							//����XY.
	size_t y = 0;
	char opponent = (player == player_c) ? comp_c : player_c;		//ȷ��
	board[row][col] = player;		//����

									//ȷ�����ж����������е��ƶ�
	for (rowdelta = -1; rowdelta <= 1; ++rowdelta)  //Ƕ������
	{
		for (coldelta = -1; coldelta <= 1; ++coldelta)
		{
			if ((row == 0 && rowdelta == -1) || row + rowdelta >= SIZE ||
				(col == 0 && coldelta == -1) || col + coldelta >= SIZE ||
				(rowdelta == 0 && coldelta == 0))
				continue;
			//�ҵ���������
			if (board[row + rowdelta][col + coldelta] == opponent)
			{
				x = row + rowdelta;  //�ƶ�
				y = col + coldelta;
				for (;;)
				{
					x += rowdelta;	//�ٴ��ƶ�
					y += coldelta;
					if (x >= SIZE || y >= SIZE || board[x][y] == ' ')// ��������ռ�,������ƶ�.
						break;
					//����ҵ����������丽����λ�ã������ֵ����ӱ���Լ���
					if (board[x][y] == player)
					{
						//ȷ����������
						while (board[x -= rowdelta][y -= coldelta] == opponent)//�ǲ��Ƕ��ֵ�����

							board[x][y] = player; //�����ı䣬����Լ���
						break;
					}
				}
			}
		}
	}
}
int get_score(char board[][SIZE], char player)			//����ͳ��
{
	return player_counters(board, player) - player_counters(board, (player == player_c) ? comp_c : player_c);
}

int best_move(char board[][SIZE], bool moves[][SIZE], char player) //��õ��ƶ���ʽ
{
	char new_board[SIZE][SIZE] = { 0 };     //����һ��������
	int score = 0;							//��÷���
	int new_score = 0;						//���ڵķ���
	for (size_t row = 0; row < SIZE; ++row)
	{
		for (size_t col = 0; col < SIZE; ++col)
		{
			if (!moves[row][col])			//���û��������Ч���ƶ�
				continue;

			//��������
			memcpy(new_board, board, sizeof(new_board));
			//�õ����ƶ���������
			make_move(new_board, row, col, player);
			//�ȽϷ���
			new_score = get_score(new_board, player);
			if (score < new_score)
				score = new_score;
		}
	}
	return score;
}

void computer_move(char board[][SIZE], bool moves[][SIZE], char player)		//�����ƶ��ķ�ʽ
{
	size_t best_row = 0;					//��õ�����
	size_t best_col = 0;
	int new_score = 0;						//�µķ���
	int score = SIZE * SIZE;					//�������ֵ
	char temp_board[SIZE][SIZE];			//���Ƶ�����
	bool temp_moves[SIZE][SIZE];			//��Ч�ƶ�������
											//ȷ�϶���
	char opponent = (player == player_c) ? comp_c : player_c;

	//����������Ч���ƶ�ȥѰ����ѵ��㷨
	for (size_t row = 0; row < SIZE; ++row)
	{
		for (size_t col = 0; col < SIZE; ++col)
		{
			if (!moves[row][col])
				continue;
			//������������
			memcpy(temp_board, board, SIZE*SIZE);

			//���ƶ���λ�ø��Ƶ����������
			make_move(temp_board, row, col, player);

			//ÿ�����Ӷ���Ҫ�����������,ȥѰ����Ч���ƶ��ڶ����ƶ���
			valid_moves(temp_board, temp_moves, opponent);

			//�Ƚ����̵ķ���,�Ӷ�ѡ��һ����ѵ��ƶ���ʽ
			new_score = best_move(temp_board, temp_moves, opponent);
			if (new_score < score)
			{
				//�����ƶ���ʽ
				score = new_score;
				best_row = row;
				best_col = col;
			}
		}
	}
	make_move(board, best_row, best_col, player);
}

//����ͳ��
int player_counters(char board[][SIZE], char player)
{
	int count = 0;
	for (size_t row = 0; row < SIZE; ++row)
		for (size_t col = 0; col < SIZE; ++col)
			if (board[row][col] == player)
				++count;
	return count;
}


//���ù��λ��
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

		system("title ��ӭ�����ڰ����˻���ս������1.0");//����cmd���ڱ���
		system("mode con cols=80 lines=40");//���ڿ�ȸ߶�
		system("color F4");					//��ɫ
		Pos(3, 1);
		system("date /T");
		Pos(3, 2);
		system("TIME /T");

		Pos(23, 5);  printf("�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[");
		Pos(23, 6);  printf("�U      ��ӭ�����ڰ��������        �U");
		Pos(23, 7);  printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 8);  printf("�U       1.������Ϸ                 �U");
		Pos(23, 9);  printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 10); printf("�U       2.��߷ּ�¼               �U");
		Pos(23, 11); printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 12); printf("�U       3.��Ϸ˵��                 �U");
		Pos(23, 13); printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 14); printf("�U       0.�˳���Ϸ                 �U");
		Pos(23, 15); printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 16); printf("�U                                  �U");
		Pos(23, 17); printf("�U                                  �U");
		Pos(23, 18); printf("�U                     by:���д����ҨU");
		Pos(23, 19); printf("�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a\n\n");
		i = getchar() - '0';

		system("cls");

		switch (i)
		{
		case 1:	//������Ϸ

			game();

			break;
		case 2:	//�鿴��¼
			rank();
			break;
		case 3://��Ϸ˵��
			ruel();
			break;
		case 0:
			return 0;
		default:
			printf("\n��ѡ����ȷ�Ĳ���Ŷ~!");
			break;
		}
	}
}

void rank() //����
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
		Pos(23, 5);  printf("�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[");
		Pos(23, 6);  printf("�U                                  �U");
		Pos(23, 7);  printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 8);  printf("�U                                  �U");
		Pos(23, 9);  printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 10); printf("�U        �����߷�:  %d            �U", max);
		Pos(23, 11); printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 12); printf("�U        ����0������ҳ             �U");
		Pos(23, 13); printf("�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g");
		Pos(23, 14); printf("�U                                  �U");
		Pos(23, 15); printf("�U                                  �U");
		Pos(23, 16); printf("�U                     by:���д����ҨU");
		Pos(23, 17); printf("�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a\n\n");
		Pos(23, 18); printf("\t\t\t\n\n");
		i = getche() - '0';
		switch (i)
		{
		case 0:
			main();
			break;
		default:
			printf("\n��ѡ����ȷ�Ĳ���Ŷ~!");
			break;
		}
	}
}
//��Ϸ��������
void ruel()
{
	system("cls");
	printf("\t\t\t\t   ����Ϸ���ܡ�\n\t\t\t        ����������ز˵�\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t��Ϸ����˵��\n");
	printf("\t\t\t\t1��������Ϊ8*8��,������(����)����(��ĸ)\n");
	printf("\t\t\t\t2��������˭�����Ӷ࣬˭�÷־͸ߣ�˭�ͻ�ʤ\n");
	printf("\t\t\t\t3��������˫�����Ӹ��ж��٣�˭�÷־Ͷ���\n");
	printf("\t\t\t\t4����������˫����2ö���ӣ�˫����������\n");
	printf("\t\t\t\t5�����������ӿ��£�������һ�غ�\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t��Ϸע������\n");
	printf("\t\t\t\t�������λ�ò�����������,���������̷�Χ֮��\n");
	printf("\t\t\t\t\t �����ߴ��ˣ�������������\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t��߷�˵��\n");
	printf("\t\t\t\t��Ϸֻ��¼�˷�����߷�\n");
	printf("\t\t\t\t-----------------\n");
	printf("\t\t\t\t��Ϸ����˵��\n");
	printf("\t\t\t\t1����Ч����С��2��˫��������£�����һ�غ�\n");
	printf("\t\t\t\t2�����Ժ������һ�����ӱ�����ʱ��������Ϸ\n");
	_getch();
}

//��������
void flash()
{
	system("title ��ӭ�����ڰ����˻���ս������1.0");//����cmd���ڱ���
	system("mode con cols=80 lines=40");//���ڿ�ȸ߶�
	system("color F4");					//��ɫ
	const char *cnm[15];
	system("color F4");
	cnm[0] = "������   ����     ����\n";
	cnm[1] = "�������������ߩ����������ߩ���\n";
	cnm[2] = "����������������������\n";
	cnm[3] = "�������������� ��    ��\n";
	cnm[4] = "�������������ש������ס���\n";
	cnm[5] = "������������������  ��\n";
	cnm[6] = "���������������ߡ�   ��\n";
	cnm[7] = "������������������  ������\n";
	cnm[8] = "��������  ����    ��    \n";
	cnm[9] = "����  ��������������  ��    \n";
	cnm[10] = "��   ���ϡ�������  ��    \n";
	cnm[11] = "��   ������   ��   ��    \n";
	cnm[12] = "����  �������ש���������������    \n";
	cnm[13] = "������ �ǩǩ���  �ǩǩ�      \n";
	cnm[14] = "������ ���ߩ���  ���ߩ�      \n";
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
		} //�Ķ��ĵط�

		Sleep(200);
		t++;
	}

}
