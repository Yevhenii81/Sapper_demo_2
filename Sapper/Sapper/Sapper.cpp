#include <iostream>
#include <windows.h>
#include <conio.h> 
#include <ctime>
#include <vector>
#include <locale>
#include <string>
using namespace std;

//Игра Сапёр
//В этой игре есть такие классы как: Map(игровое поле), Game(Игра), Console(настройка консоли), Cell(клетка), Player(игрок)

enum NumberColor {BLUE = 1, GREEN = 2, RED = 3, DARKBLUE = 4, BROWN = 5, TURQUOISE = 6, BLACK = 7, WHITE = 8}; //раскрас игровых цифр
//enum NumberColor { BLUE , GREEN, RED, DARKBLUE, BROWN, TURQUOISE, BLACK, WHITE };
enum KeyCode {ENTER = 13, SPACE = 27,LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72 }; //управление клавиатурой
enum SapperObject {WALL = 100, HALL = 0, MINE = 10}; //

class Map
{
public:
	int width = 10;
	int height = 10;
	vector<vector<int>> map;
	vector<vector<int>> mask;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
public:
	//функция, которая инициализирует карту
	void InitializeMap()
	{
		for (int i = 0; i < width; i++)
		{
			vector<int> temp;
			for (int j = 0; j < height; j++)
			{
				if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				{
					temp.push_back(WALL);
				}
				else
				{
					temp.push_back(HALL);
				}
			}
			map.push_back(temp);
		}
	}

	//метод, который распечатывает карту
	void PrintMap()
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if (mask[j][i] == HALL)
				{
					//SetConsoleTextAttribute(h, BLACK);
					cout << ".";
					continue;
				}

				if (map[j][i] == WALL)
				{
					SetConsoleTextAttribute(h, WHITE);
					cout << (char)178; // стена
				}
				else if (map[j][i] == HALL)
				{
					SetConsoleTextAttribute(h, BLACK);
					cout << " "; // коридор
				}
				else if (map[j][i] == MINE)
				{
					SetConsoleTextAttribute(h, GREEN);
					cout << "*"; //мина
				}
				else 
				{
					SetConsoleTextAttribute(h, BLACK);
					SetConsoleTextAttribute(h, WHITE);
					cout << map[j][i];
				}
			}
			cout << "\n";
		}
	}

	//метод расстановки чисел на игровом поле
	void NumberSetting()
	{
		for (int i = 1; i < width - 1; i++)
		{
			for (int j = 1; j < height - 1; j++)
			{
				if (map[i][j] != MINE)
				{
					int count = 0;
					if (map[i - 1][j - 1] == MINE) count++;
					if (map[i - 1][j] == MINE) count++;
					if (map[i - 1][j + 1] == MINE) count++;
					if (map[i][j - 1] == MINE) count++;
					if (map[i][j + 1] == MINE) count++;
					if (map[i + 1][j - 1] == MINE) count++;
					if (map[i + 1][j] == MINE) count++;
					if (map[i + 1][j + 1] == MINE) count++;
					map[i][j] = count;
				}
			}
		}
	}

	//инициализируем маску
	void InitializeMask()
	{
		for (int i = 0; i < width; i++)
		{
			vector<int> temp;
			for (int j = 0; j < height; j++)
			{
				if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				{
					temp.push_back(WALL);
				}
				else
				{
					temp.push_back(HALL);
				}
			}
			mask.push_back(temp);
		}
	}

	// Метод для открытия ячейки и обновления маски
	void OpenCell(int x, int y)
	{
		if (mask[x][y] == HALL)  // Если ячейка еще не открыта
		{
			if (map[x][y] == MINE)  // Если это мина, выводим звездочку
			{
				SetConsoleTextAttribute(h, GREEN);
				cout << "*";
			}
			else if (map[x][y] == HALL)  // Если это пустая ячейка, выводим пустоту
			{
				SetConsoleTextAttribute(h, BLACK);
				cout << " ";
			}
			else  // Иначе выводим значение ячейки (цифру)
			{
				SetConsoleTextAttribute(h, BLACK);
				SetConsoleTextAttribute(h, WHITE);
				cout << map[x][y];
			}
		}
	}

	//метод, который проверяет 
	/*bool IsWall(int x, int y)
	{
		if (map[x][y] == WALL)
		{
			return true;
		}
		return false;
	}*/
};

class Cell
{
private:
	bool isMine;
	bool isRevealed;
	int adjacentMines;
public:
	Cell() : isMine(false), isRevealed(false), adjacentMines(0) {}

	//метод, который рандомно расставляет мины
	void RandMines(int numMines, Map& map)
	{
		//проверка на количество мин, которые можно установить на поле
		if (numMines >= (map.width - 2) * (map.width - 2))
		{
			MessageBoxA(0, "Слишком много мин!", "Попробуйте заново!", MB_OK);
			return;
		}
		for (int i = 0; i < numMines; i++)
		{
			int x = 0;
			int y = 0;
			//поиск пустой ячейки, не занятой миной
			do
			{
				x = rand() % (map.width - 2) + 1;
				y = rand() % (map.height - 2) + 1;
			} while (map.map[x][y] == MINE);
			map.map[x][y] = MINE;
		}
	}
};


class Player
{

};

class Console
{
public:
	void Options(int x, int y)
	{
		COORD p = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	}

	//метод, который выводит логотип игры и просит нажать ENTER чтобы начать игру
	void ShowLogo()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		int widthConsole = csbi.dwSize.X;

		int x = (widthConsole - 6) / 2; // Центрируем текст "Sapper"
		int y = 9;

		//не забыть разкоментировать
		/*Options(x, y);
		cout << "Sapper" << "\n";
		cout << "\n\n\t\t\t\t\tPlease, press ENTER to start ";
		while (_getch() != ENTER);
		system("cls");*/
	}
	
	//метод, который скрывает курсор
	void HideCursor(bool vision)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.bVisible = !vision;
		info.dwSize = 100;
		SetConsoleCursorInfo(h, &info);
	}

	//метод, который настравивает курсор
	//void SetCursor(int x, int y, int color, string text) 
	//{
	//	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	//	COORD position;
	//	position.X = x; // настройка конкретных координат
	//	position.Y = y;
	//	SetConsoleCursorPosition(h, position); // установка курсора в заданную точку
	//	SetConsoleTextAttribute(h, color); // смена цвета
	//	cout << text; // вывод строки текста
	//	cout << "\n";
	//}

	void SetCursor(int x, int y)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position;
		position.X = x; // Настройка координат X
		position.Y = y; // Настройка координат Y
		SetConsoleCursorPosition(h, position); // Установка курсора в заданную точку
	}

	//настройка консоли
	void OptionsConsole()
	{
		HWND hwnd = GetConsoleWindow();

		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);

		int window_width = 1360;
		int window_height = 768;

		MoveWindow(hwnd,
			(screen_width - window_width) / 2,
			(screen_height - window_height) / 2,
			window_width,
			window_height,
			1);
		system("title Sapper");
	}
};

class Game
{
public:
	void run()
	{
		Map map;
		Cell cell;
		Console console;
		console.OptionsConsole();
		console.ShowLogo();
		map.InitializeMap();
		map.InitializeMask();
		cell.RandMines(8, map);
		map.NumberSetting();
		map.PrintMap();

		COORD position;
		position.X = 1;
		position.Y = 1;
		console.SetCursor(1, 1);
		console.HideCursor(false);

		while (true)
		{
			int code = _getch();
			if (code == 224)
			{
				code = _getch();
			}

			int tx = position.X;
			int ty = position.Y;

			console.Options(tx, ty);
			//console.SetCursor(position.X, position.Y, BLACK, " ");

			if (code == RIGHT && map.map[position.Y][position.X + 1] != WALL)
			{
				position.X++;
			}
			else if (code == LEFT && map.map[position.Y][position.X - 1] != WALL)
			{
				position.X--;
			}
			else if (code == DOWN && map.map[position.Y + 1][position.X] != WALL)
			{
				position.Y++;
			}
			else if (code == UP && map.map[position.Y - 1][position.X] != WALL)
			{
				position.Y--;
			}
			else if (code == ENTER)
			{
				map.OpenCell(position.X, position.Y);
			}

			console.Options(tx, ty); // Убираем предыдущий курсор
			console.Options(position.X, position.Y); // Перемещаем курсор

			//console.SetCursor(tx, ty, BLACK, " "); // Стираем предыдущий курсор
			//console.SetCursor(position.X, position.Y, WHITE, "_"); // Рисуем новый курсор

			//console.SetCursor(position.X, position.Y, WHITE, "");
			/*if (map.IsWall(position.X, position.Y))
			{
				position.X = tx;
				position.Y = ty;
			}*/
		}
	}
};

int main()
{
	srand(time(NULL));
	
	Game game;
	game.run();
}
