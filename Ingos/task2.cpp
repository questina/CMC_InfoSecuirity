#include <iostream> 
#include <fstream>
#include <cstdlib>
#include <vector>

/*TASK 2. Magic Ladder*/

bool check_symb (char c){ //проверка, является ли символ цифрой
	if ((c >= '0') && (c <= '9')){
		return true;
	} else {
		return false;
	}
}

int get_num (std::ifstream &file){ //извлекает число, все символы кроме цифр пропускает, символ ']' считается концом вывод.
	char c;
	int num = 0;
	while (!file.eof()){
		c = file.get();
		if (check_symb(c)){
			int tmp = std::stoi(&c);
			num = num*10 + tmp;

		}
		else {
			if (check_symb(file.peek())) {
				return num;
			}
		}
		if (c == ']') return num;
	}
	return -1;
}

int min (int a, int b){
	return a<b? a : b;
}

int analyze_arr(std::ifstream &file){
	std::vector<int> cost;
	cost.push_back(get_num(file));
	cost.push_back(get_num(file));
	if (cost[1] == -1) return cost[0];
	int i = 1;
	int c;
	while (!file.eof()){
		c = get_num(file);
		cost.push_back(min(cost[i-1],cost[i])+c);
		i++;
	}
	if (c == -1) return cost[i]+1;
	return cost[i];
}


int main (int argc, char** argv){
	int num = 0;
	bool num_flg = false;
	int count = 0;
	std::ifstream tex_file;
	tex_file.open(argv[1], std::ios_base::in);
	char c = tex_file.get();
	if (c != '[') {
		std::cerr << "incorrect input, the text must begin with symbol '['!";
		return -1;
	}
	count = analyze_arr(tex_file);
	std::cout << count << std::endl;
	tex_file.close();
}