#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
//====================================================================================================================
enum class state_player
{
	ALIVE, DEATH, FELL, KILL_THE_WAMPUS
};
//====================================================================================================================
// CAVE_LIST
//====================================================================================================================
struct Cave_List
{
	std::vector<std::vector<int>> generate_rooms();	// Генерация комнат

	std::string welcome(int player, std::vector<std::vector<int>> &cave_array);	// Приветствие

	bool shooting(int &player, std::string &str, state_player &cur_state, std::vector<std::vector<int>> &cave_array);	// Стрельба

	void moving(int &player, std::string str, std::vector<std::vector<int>> &cave_array);		// Движение

	void final_round_state(state_player &cur_state);	// Состояние в конце раунда

	void game(std::vector<std::vector<int>> &cave_array);	// Игра

	std::vector<int> bat = { 2, 5, 8, 13};
	std::vector<int> fel = { 1, 12, 18 };
	int wampus = 17;
	int arrow = 5;
};
//====================================================================================================================
std::vector<std::vector<int>> Cave_List::generate_rooms() {
//	std::mt19937 gen(time(nullptr));
	std::vector<std::vector<int>> result(20);

	//	Жёсткая генерация пещеры
	result[0].push_back(7); result[0].push_back(5); result[0].push_back(2);

	result[1].push_back(5); result[1].push_back(14); result[1].push_back(8);

	result[2].push_back(3); result[2].push_back(0); result[2].push_back(4);

	result[3].push_back(10); result[3].push_back(9); result[3].push_back(2);

	result[4].push_back(2); result[4].push_back(6); result[4].push_back(11);

	result[5].push_back(7); result[5].push_back(0); result[5].push_back(1);

	result[6].push_back(4); result[6].push_back(11); result[6].push_back(14);

	result[7].push_back(9); result[7].push_back(0); result[7].push_back(5);

	result[8].push_back(14); result[8].push_back(1); result[8].push_back(11);

	result[9].push_back(12); result[9].push_back(7); result[9].push_back(3);
	
	result[10].push_back(3); result[10].push_back(13); result[10].push_back(16);

	result[11].push_back(8); result[11].push_back(6); result[11].push_back(4);

	result[12].push_back(9); result[12].push_back(13); result[12].push_back(15);

	result[13].push_back(15); result[13].push_back(12); result[13].push_back(10);

	result[14].push_back(1); result[14].push_back(8); result[14].push_back(6);

	result[15].push_back(18); result[15].push_back(13); result[15].push_back(12);

	result[16].push_back(19); result[16].push_back(17); result[16].push_back(10);

	result[17].push_back(16); result[17].push_back(19); result[17].push_back(18);

	result[18].push_back(17); result[18].push_back(19); result[18].push_back(15);

	result[19].push_back(16); result[19].push_back(17); result[19].push_back(18);

	return result;
}
//====================================================================================================================
std::string Cave_List::welcome(int player, std::vector<std::vector<int>> &cave_array)
{
	std::cout << "You are in the room: " << player << '\n';		// Где игрок сейчас

	std::cout << "You are movine to rooms: ";
	std::string stroke = {};
	std::vector<int> current = cave_array[player];

	for (int a : current)
		std::cout << a << " ";

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
	std::cin >> stroke;

	return stroke;
}
//====================================================================================================================
bool Cave_List::shooting(int &player, std::string &str, state_player &cur_state, std::vector<std::vector<int>> &cave_array)
{
	bool miss = true;
	char s = str[0];
	std::vector<int> temp = {};
	std::string res = {};

	if (s == 's') {
		int path_arrow = player;								// Путь стрельбы

		for (int i = 1; i <= str.size(); i++) {
			if (i == str.size() || str[i] == '-') {
				int f = std::stoi(res);
				temp.push_back(f);								// Добавляем в вектор выбранные комнаты куда стрельнули
				res = {};
				continue;
			}
			else
				res += str[i];								// Собираем числа комнат куда стрельнули
		}
		if (temp.size() < 3) {
			std::cout << "Incorrect path input\n";
			return false;
		}

		for (int h : temp)										// Проверяем есть ли связи у всех комнат которые выбрали
		{
			for (int m : cave_array[path_arrow]) {				// Проверяем есть ли связи у всех комнат которые выбрали
				if (h != m) { miss = true; break; }
				else {
					path_arrow = h;
					miss = false;
				}
			}
		}

		for (int g : temp) {
			if (g == wampus)									// Если стрела пролетела через комнату с Вампусом - он мертв!
				cur_state = state_player::KILL_THE_WAMPUS;
		}

		for (int g : temp)
		{
			// ВАМПУС ПРОСНУЛСЯ если стрела пролетела рядом с Вампусом
			for (int wuw : cave_array[wampus]) {

				if (g == wuw)
				{
					wampus = wuw;							// Вампус перешёл в соседнюю комнату
					if (player == wampus)
						cur_state = state_player::DEATH;	// Если в комнату к игроку - то смерть игроку
					else {
						arrow--;												// Уменьшение стрел
						std::cout << "You woke up Wampus!\n";	// Состояние пробуждения вампуса и выброса предупреждения об этом
					}
				}
			}
		}

		if (miss == true) {
			std::cout << "You`re missing! -1 arrow\n";
			arrow--;												// Уменьшение стрел
			return true;
		}
	}
}
//====================================================================================================================
void Cave_List::moving(int &player, std::string str, std::vector<std::vector<int>> &cave_array)
{
	bool correct_room = false;
	std::string res = {};

		for (char m : str) {
			if (std::isdigit(m))
				res += m;
		}

		int m = std::stoi(res);								// Комната в которую мы собрались идти
		
		for (int a : cave_array[player])					// Проверяем что у нас есть связь с комнатой в которую собираемся идти
			if (m != a) continue;
			else
				correct_room = true;
		if (correct_room == true)
			player = m;
		else
			std::cout << "Wrong away\n";	// Проверяем что у нас есть доступ к выбранной пещере.
}
//====================================================================================================================
void Cave_List::final_round_state(state_player &cur_state)
{
	if (cur_state == state_player::FELL)
		std::cout << "You fell into a hole!\n GAME OVER!!!!\n\n";

	if (cur_state == state_player::DEATH)
		std::cout << "You were killed by Wampus\n GAME OVER!!!!\n\n";

	if (cur_state == state_player::KILL_THE_WAMPUS)
		std::cout << "You WIN!\nGAME OVER!!!!\n\n";
}
//====================================================================================================================
void Cave_List::game(std::vector<std::vector<int>> &cave_array)
{
	state_player current_state = state_player::ALIVE;
	int player = 7;

	while (current_state == state_player::ALIVE) {
		bool shot_move = false;

		std::string stroke = welcome(player, cave_array);	// Приветствие(Начало игры)

		char s = stroke[0];

		while (!shot_move) {
			if (s == 's') {
				if (!shooting(player, stroke, current_state, cave_array))		// Стрельба
					shot_move = false;
				else
					shot_move = true;
			}

			else if (s == 'm') {
				moving(player, stroke, cave_array);							// Движение
				shot_move = true;
				}

			else std::cout << "Not found 's' or 'm', please, re-enter: ";
		}

		for (int ba : bat) {					// Если утащила мышь берем рандомную пещеру для игрока
			if (player == ba) player = rand() % 20;
		}

		for (int fe : fel) {					// Проверяем не упал ли наш игрок в дыру, после мыши или зайдя "не туда"
			if (player == fe) current_state = state_player::FELL;
		}

		if (player == wampus) current_state = state_player::DEATH;	// Игрок зашёл к Вампусу или Вампус вышел к игроку


		final_round_state(current_state);	// Проверка состояния в конце раунда
		
	}
}
//====================================================================================================================
int main() {
	std::cout << "Welcome to the GAME 'HUNT THE VAMPUS'" << '\n';
	std::cout << "Move: m1 - move to room 1.\nShot: s1-3-5 - shot the room 1-3-5, if wrong way: -1 arrow!\n";
	Cave_List cave;
	bool gam = true;
	char otvet = {};

	while (gam) {

		std::vector<std::vector<int>> start_rooms = cave.generate_rooms();
		cave.game(start_rooms);

		std::cout << "Retry game?\nY/y or N/n";
		std::cin >> otvet;
		if (otvet == 'Y' || otvet == 'y')
			gam = true;
		if (otvet == 'N' || otvet == 'n')
			gam = false;
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