#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
//====================================================================================================================
enum class state_player
{
	ALIVE, DEATH, FELL, KILL_THE_VAMPUS, MISS
};
//====================================================================================================================
// CAVE_LIST
//====================================================================================================================
struct Cave_List
{
	std::vector<std::vector<int>> generate_rooms();	// Генерация комнат
	void game(std::vector<std::vector<int>> cave_array);
	//			std::vector<std::vector<int>> cave;


	std::vector<int> bat = { 2, 5, 8, 13, 16, 19 };
	std::vector<int> fel = { 1, 6, 9, 12, 15, 18 };
	int wampus = 17;
};
//====================================================================================================================
std::vector<std::vector<int>> Cave_List::generate_rooms() {
//	std::mt19937 gen(time(nullptr));
	std::vector<std::vector<int>> result(20);

	//	Жёсткая генерация пещеры
	result[0].push_back(7);
	result[0].push_back(5);
	result[0].push_back(2);

	result[1].push_back(5);
	result[1].push_back(14);
	result[1].push_back(8);

	result[2].push_back(3);
	result[2].push_back(0);
	result[2].push_back(4);

	result[3].push_back(10);
	result[3].push_back(9);
	result[3].push_back(2);

	result[4].push_back(2);
	result[4].push_back(6);
	result[4].push_back(11);

	result[5].push_back(7);
	result[5].push_back(0);
	result[5].push_back(1);

	result[6].push_back(4);
	result[6].push_back(11);
	result[6].push_back(14);

	result[7].push_back(9);
	result[7].push_back(0);
	result[7].push_back(5);

	result[8].push_back(14);
	result[8].push_back(1);
	result[8].push_back(11);

	result[9].push_back(12);
	result[9].push_back(7);
	result[9].push_back(3);
	
	result[10].push_back(3);
	result[10].push_back(13);
	result[10].push_back(16);

	result[11].push_back(8);
	result[11].push_back(6);
	result[11].push_back(4);

	result[12].push_back(9);
	result[12].push_back(13);
	result[12].push_back(15);

	result[13].push_back(15);
	result[13].push_back(12);
	result[13].push_back(10);

	result[14].push_back(1);
	result[14].push_back(8);
	result[14].push_back(6);

	result[15].push_back(18);
	result[15].push_back(13);
	result[15].push_back(12);

	result[16].push_back(19);
	result[16].push_back(17);
	result[16].push_back(10);

	result[17].push_back(16);
	result[17].push_back(19);
	result[17].push_back(18);

	result[18].push_back(17);
	result[18].push_back(19);
	result[18].push_back(15);

	result[19].push_back(16);
	result[19].push_back(17);
	result[19].push_back(18);

	return result; }
//====================================================================================================================
void Cave_List::game(std::vector<std::vector<int>> cave_array) {
	state_player current_state;
	current_state = state_player::ALIVE;
	int move = 0;
	int player = 7;
	int arrow = 5;
	std::vector<int> current = {};
	std::string stroke = {};

	while (current_state == state_player::ALIVE) {

		current = cave_array[player];

		std::cout << "You are in the room: " << player << '\n';		// Где игрок сейчас

		std::cout << "You are movine to rooms: ";

		for (int a : current)
			std::cout << a << "   ";
			
		std::cout << '\n';

		for (int a : current)										// Проверяем примыкающие комнаты с игроком на предмет опасности
		{
			for (int b : bat)
				if (a == b) std::cout << "I hear a bat\n";

			for (int c : fel)
				if (a == c) std::cout << "I feel the breeze\n";

			if (a == wampus) std::cout << "I smell Wampus\n";
		}

		std::cout << "What are you going to do?\n";
		std::cout << "Move: m1 - move to room 1.\nShot: s1-3-5 - shot the room 1-3-5, if wrong way: -1 arrow!\n";
		std::cin >> stroke;

		std::string res = {};
		char s = stroke[0];
		std::vector<int> temp = {};

		if (s == 's')
		{
			arrow--;
			int path_arrow = player;

			for (int i = 1; i < stroke.size(); i++) {
				if (stroke[i] == '-') {
					int f = std::stoi(res);
					temp.push_back(f);
					continue;
				}
				else
					res += stroke[i];
			}
			for (int g : temp)
				if (g == wampus)
					current_state = state_player::KILL_THE_VAMPUS;

			for (int h : temp)
			{
				if (current_state == state_player::MISS)
					break;

				for (int m : cave_array[path_arrow]) {
					if (h == m) {
						path_arrow = h;
						break;
					}
					else
						current_state = state_player::MISS;
				}
			}
			// Еще нужна проверка нет ли Вампуса рядом с комнатами где пролетает стрела
		}

		if (s == 'm') {
			for (char m : stroke) {
				if (std::isdigit(m))
					res += m;
			}
			int m = std::stoi(res);
			player = m;
		}

		for (int ba : bat)
			if (player == ba) player = rand() % 20;
		for (int fe : fel)
			if (player == fe) current_state = state_player::FELL;
		if (player == wampus) current_state = state_player::DEATH;

		if (current_state == state_player::FELL)
			std::cout << "You fell into a hole!\n";

		if (current_state == state_player::DEATH)
			std::cout << "You were killed by Wampus\n";

		if (current_state == state_player::KILL_THE_VAMPUS)
			std::cout << "You`re WIN!\n";
	}
}
//====================================================================================================================
int main() {
	std::cout << "Welcome to the GAME 'HUNT THE VAMPUS'" << '\n';
	Cave_List cave;

	while (true) {
		std::vector<std::vector<int>> start_rooms = cave.generate_rooms();
		cave.game(start_rooms);
	}

	return 0;
}
//====================================================================================================================
/*
	12. Реализуйте версию игры “Охота на Вампуса”(или просто “Вамп”).
Это простая компьютерная(не графическая) игра, изобретенная Грегори Йобом(Gregory Yob).

- Цель этой игры — найти довольно смышленого монстра, прячущегося в темном пещерном лабиринте.

- Ваша задача — убить вампуса с помощью лука и стрел.Кроме вампуса, пещера таит еще две опасности : бездонные ямы и гигантские
летучие мыши.

- Если вы входите в комнату с бездонной ямой, то игра для вас закончена.

- Если вы входите в комнату с летучей мышью, то она вас хватает и перебрасывает в другую комнату.

- Если же вы входите в комнату с вампусом или он входит в комнату, где находитесь вы, он вас съедает.

- Входя в комнату, вы должны получить предупреждение о грозящей опасности.

** “Я чувствую запах вампуса” — значит, он в соседней комнате.
** “Я чувствую ветерок” — значит, в соседней комнате яма.
** “Я слышу летучую мышь” — значит, в соседней комнате живет летучая мышь.

- Для вашего удобства комнаты пронумерованы.Каждая комната соединена туннелями с тремя другими.

- Когда вы входите в комнату, то получаете сообщение, например : “Вы в комнате номер 12; отсюда
идут туннели в комнаты 1, 13 и 4; идти или стрелять ? ” Возможные ответы : m13(“Переход в комнату
номер 13”) и s13–4–3(“Стрелять через комнаты с номерами 13, 4 и 3”).
- Стрела может пролететь через три комнаты.В начале игры у вас есть пять стрел.
Загвоздка со стрельбой заключается в том, что вы можете разбудить вампуса и он войдет в комнату,
соседнюю с той, где он спал, — она может оказаться вашей комнатой.

- Вероятно, самой сложной частью этого упражнения является программирование пещеры и выбор комнат,
связанных с другими комнатами.Возможно, вы захотите использовать датчик случайных чисел, чтобы при разных запусках программы
использовались разные пещеры и разное расположение летучих мышей и вампуса.Подсказка : используйте режим
отладки для проверки состояния лабиринта.
*/
//====================================================================================================================