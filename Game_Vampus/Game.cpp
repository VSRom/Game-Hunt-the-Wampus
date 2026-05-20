#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
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
	std::vector<std::vector<int>> generate_rooms_dangers();																// Генерация комнат и опасностей
	std::string first_step_round(int player, std::vector<std::vector<int>> &cave_array);								// Приветствие
	bool shooting(int &player, std::string &str, state_player &cur_state, std::vector<std::vector<int>> &cave_array);	// Стрельба
	void moving(int &player, std::string str, std::vector<std::vector<int>> &cave_array);								// Движение
	void final_step_round(state_player &cur_state);																		// Состояние в конце раунда
	void game(std::vector<std::vector<int>> &cave_array);																// Игра

	std::vector<int> bat = {};
	std::vector<int> fel = {};
	int wampus = 0;
	int arrow = 5;
};
//====================================================================================================================
std::vector<std::vector<int>> Cave_List::generate_rooms_dangers() {

	std::vector<std::vector<int>> result(20);
	std::mt19937 gen(time(nullptr));
	std::uniform_int_distribution<int> distrib(0, 19);

	// Псевдослучайная генерация пещеры
	for (int i = 0; i < 20; i++) {
		while (result[i].size() < 3) {
			bool duplicate = false;
			int temp = distrib(gen);
			if (i == temp) continue;
			for (int go_vec : result[i])
				if (go_vec == temp) {
					duplicate = true;
					break;
				}
			if (!duplicate) {
				result[i].push_back(temp);
				if (result[temp].size() < 3)
					for (int go_temp : result[temp])
						if (go_temp == i)
							duplicate = true;
			}
			if (!duplicate && result[temp].size() < 3)
				result[temp].push_back(i);
		}
	}

	auto generate_danger = [&](std::vector<int> &danger) {
		while (danger.size() < 4) {
			bool duplicate = false;
			int temp = distrib(gen);
			for (int go_bat : danger)
				if (go_bat == temp) {
					duplicate = true;
					break;
				}
			if (!duplicate)
				danger.push_back(temp);
		}
		};

	// Псевдослучайная генерация опасностей(используем лямбда-выражение)
	generate_danger(bat);
	generate_danger(fel);

	bool find_bat = {};
	bool find_fel = {};
	// check repeat dangers
	do {
		wampus = distrib(gen);
		find_bat = false; find_fel = false;

		for (int b : bat) {
			if (b == wampus) find_bat = true;
		}
		for (int f : fel) {
			if (f == wampus) find_fel = true;
		}
	} while (find_bat || find_fel);

	return result;
}
//====================================================================================================================
std::string Cave_List::first_step_round(int player, std::vector<std::vector<int>> &cave_array)
{
	std::cout << "You are in the room: " << player << '\n';		// Где игрок сейчас

	std::cout << "You are movine to rooms: ";
	std::string stroke = {};

	auto check_danger = [&cave_array, player](std::vector<int> danger, std::string message) {
		for (int a : cave_array[player]) {										// Проверяем примыкающие комнаты с игроком на предмет опасности
			for (int b : danger)
				if (a == b) { std::cout << message; return true; }
		}
		return false;
		};

	for (int a : cave_array[player])
		std::cout << a << " ";

	std::cout << '\n';

	check_danger(bat, "I hear a bat\n");
	check_danger(fel, "I feel the breeze\n");
	check_danger(static_cast<std::vector<int>>(wampus), "I smell Wampus\n");

	std::cout << "What are you going to do?\n";
	std::cin >> stroke;

	return stroke;
}
//====================================================================================================================
bool Cave_List::shooting(int &player, std::string &str, state_player &cur_state, std::vector<std::vector<int>> &cave_array)
{
	bool miss = false;
	char s = str[0];
	std::vector<int> temp = {};
	std::string res = {};
	int path_arrow = player;								// Путь стрельбы
	int count = 0;


	auto valider_path = [&cave_array, &count](std::vector<int> path, int parr) {
		for (auto path_var : path) {
			for (auto near_room : cave_array[parr])
				if (path_var == near_room) { parr = path_var; count++; }
		}
		if (count == 3)
			return false;
		else
			return true;
		};

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

	miss = valider_path(temp, path_arrow);

	for (int g : temp) {
		if (g == wampus)									// Если стрела пролетела через комнату с Вампусом - он мертв!
			cur_state = state_player::KILL_THE_WAMPUS;
	}

	for (int g : temp) {
		for (int wuw : cave_array[wampus]) {			// ВАМПУС ПРОСНУЛСЯ если стрела пролетела рядом с Вампусом
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
	return false;
}
//====================================================================================================================
void Cave_List::moving(int &player, std::string str, std::vector<std::vector<int>> &cave_array)
{
	bool correct_room = false;
	std::string res = {};

	auto is_connect = [&cave_array](int from, int to) {		// Проверяем что у нас есть связь с комнатой в которую собираемся идти
		for (int a : cave_array[from]) {
			if (to == a)
				return true;
		}
		return false;
		};

	for (char m : str) {
		if (std::isdigit(m))
			res += m;
	}

	int m = std::stoi(res);									// Комната в которую мы собрались идти

	if (is_connect(player, m))
		player = m;
	else
		std::cout << "Wrong away\n";

}
//====================================================================================================================
void Cave_List::final_step_round(state_player &cur_state)
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
	int player = 0;
	std::mt19937 gen(time(nullptr));
	std::uniform_int_distribution<int> distrib(0, 19);
	bool find_bat = {};
	bool find_fel = {};
	bool find_wampus = {};
	// check player position
	do {
		player = distrib(gen);
		find_bat = false; find_fel = false; find_wampus = false;

		for (int b : bat) {
			if (b == player) find_bat = true;
		}
		for (int f : fel) {
			if (f == player) find_fel = true;
		}
		if (wampus == player) find_wampus = true;

	} while (find_bat || find_fel || find_wampus);

	while (current_state == state_player::ALIVE) {
		bool shot_move = false;

		std::string stroke = first_step_round(player, cave_array);	// Приветствие(Начало игры)

		char s = stroke[0];

		while (!shot_move) {
			if (s == 's')
				shot_move = shooting(player, stroke, current_state, cave_array);		// Стрельба

			else if (s == 'm') {
				moving(player, stroke, cave_array);							// Движение
				shot_move = true;
			}

			else std::cout << "Not found 's' or 'm', please, re-enter: ";
		}

		for (int ba : bat) {					// Если утащила мышь берем рандомную пещеру для игрока
			if (player == ba) player = distrib(gen);
		}

		for (int fe : fel) {					// Проверяем не упал ли наш игрок в дыру, после мыши или зайдя "не туда"
			if (player == fe) current_state = state_player::FELL;
		}

		if (player == wampus) current_state = state_player::DEATH;	// Игрок зашёл к Вампусу или Вампус вышел к игроку

		final_step_round(current_state);	// Проверка состояния в конце раунда
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

		std::vector<std::vector<int>> start_rooms = cave.generate_rooms_dangers();
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
	V	12. Реализуйте версию игры “Охота на Вампуса”(или просто “Вамп”).
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