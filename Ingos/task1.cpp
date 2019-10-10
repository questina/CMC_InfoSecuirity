#include <iostream> 
#include <fstream>
#include <cstdlib>

/*TASK 1. Traveller story*/

bool check_symb (char c){ //проверка, является ли символ цифрой
	if ((c >= '0') && (c <= '9')){
		return true;
	} else {
		return false;
	}
}

int get_symb (std::ifstream &file){ //извлекает число, все символы кроме цифр пропускает, символ ']' считается концом вывод.
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

int analyze_arr(int *p){
	int pik = 0;
	if ((p[0] <= p[1]) && (p[2] <= p[1])){
		pik = 1;
	}
	p[0] = p[1];
	p[1] = p[2];
	return pik;
}


int main (int argc, char** argv){
	int num = 0;
	bool num_flg = false;
	int count = 0;
	std::ifstream tex_file;
	int point[3] = {-1, -1, -1};
	tex_file.open(argv[1], std::ios_base::in);
	char c = tex_file.get();
	if (c != '[') {
		std::cerr << "incorrect input, the text must begin with symbol '['!";
		return -1;
	}
	point[0] = get_symb(tex_file);
	point[1] = get_symb(tex_file);
	if ((point[0] == -1) || (point[1] == -1)){
		std::cerr << "input information is too small to understand something";
		return -1;
	}
	while (!tex_file.eof()){
		point[2] = get_symb(tex_file);
		if (point[2] == -1) break;
		count += analyze_arr(point);
	}
	std::cout << count << std::endl;
	tex_file.close();
}