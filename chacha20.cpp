#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

std::string hex_to_bin(char c){
	switch(c){
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'a': return "1010";
		case 'b': return "1011";
		case 'c': return "1100";
		case 'd': return "1101";
		case 'e': return "1110";
		case 'f': return "1111";
		default: return "err";
	}
}

std::string reverse(std::string str){
	std::string str_res;
	for (int i = 0; i < str.size(); i++){
		str_res += str[str.size()-i-1];
	}
	return str_res;
}

std::string bit_to_string_le(std::bitset<32> b){
	std::bitset<4> b1;
	std::string str;
	int c;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 4; j++){
			b1[j] = b[i*4+j];
		}
		c = b1.to_ulong();
		if (c > 9){
			str += 'a' + c - 10;
		} else {
			str += '0' + c;
		}
	}
	return str;
}

std::string bit_to_string_be(std::bitset<32> b){
	return reverse(bit_to_string_le(b));
}

void init(std::string str, std::bitset<32> *q_r, int block_c = 1){
	std::string temp;
	std::string constant = "61707865";
	for (int i = 0; i < 8; i++){
		temp += hex_to_bin(constant[i]);
	}
	q_r[0] = std::bitset<32>(temp);
	temp = "";
	constant = "3320646e";
	for (int i = 0; i < 8; i++){
		temp += hex_to_bin(constant[i]);
	}
	q_r[1] = std::bitset<32>(temp);
	temp = "";
	constant = "79622d32";
	for (int i = 0; i < 8; i++){
		temp += hex_to_bin(constant[i]);
	}
	q_r[2] = std::bitset<32>(temp);
	temp = "";
	constant = "6b206574";
	for (int i = 0; i < 8; i++){
		temp += hex_to_bin(constant[i]);
	}
	q_r[3] = std::bitset<32>(temp);
	temp = "";
	for (int i = 7; i >= 0; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[4] = std::bitset<32>(temp);
	temp = "";
	for (int i = 15; i >= 8; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[5] = std::bitset<32>(temp);
	temp = "";
	for (int i = 23; i >= 16; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[6] = std::bitset<32>(temp);
	temp = "";
	for (int i = 31; i >= 24; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[7] = std::bitset<32>(temp);
	temp = "";
	for (int i = 39; i >= 32; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[8] = std::bitset<32>(temp);
	temp = "";
	for (int i = 47; i >= 40; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[9] = std::bitset<32>(temp);
	temp = "";
	for (int i = 55; i >= 48; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[10] = std::bitset<32>(temp);
	temp = "";
	for (int i = 63; i >= 56; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[11] = std::bitset<32>(temp);
	q_r[12] = std::bitset<32>(block_c);
	temp = "";
	for (int i = 71; i >= 64; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[13] = std::bitset<32>(temp);
	temp = "";
	for (int i = 79; i >= 72; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[14] = std::bitset<32>(temp);
	temp = "";
	for (int i = 87; i >= 80; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[15] = std::bitset<32> (temp);
}

std::bitset<32> add_32(std::bitset<32> num_1, std::bitset<32> num_2){
	std::bitset<32> block(0);
	std::bitset<33> b3;
	std::bitset<33> b1(0);
	std::bitset<33> b2(0);
	for (int i = 0; i < 32; i++){
		b2[i] = num_1[i];
		b1[i] = num_2[i];
	}
	b3 = b1.to_ulong() + b2.to_ulong();
	for (int i = 0; i < 32; i++){
		block[i] = b3[i];
	}
	return block;
}

std::bitset<32> cycle (std::bitset<32> block, int n){
	for (int i = 0; i < n; i++){
		if (block[31] == 1){
			block <<= 1;
			block[0] = 1;
		} else {
			block <<= 1;
		}
	}
	return block;
}

void quarter_round (std::bitset<32> *q_r, int n1, int n2, int n3, int n4){
	std::bitset<32> a = q_r[n1];
	std::bitset<32> b = q_r[n2];
	std::bitset<32> c = q_r[n3];
	std::bitset<32> d = q_r[n4];
	a = add_32(a,b);
	d ^= a;
	d = cycle(d,16);
	c = add_32(c,d);
	b ^= c;
	b = cycle(b,12);
	a = add_32(a,b);
	d ^= a;
	d = cycle(d,8);
	c = add_32(c,d);
	b ^= c;
	b = cycle(b,7);
	q_r[n1] = a;
	q_r[n2] = b;
	q_r[n3] = c;
	q_r[n4] = d;
}

void print_matrix(std::bitset<32> *q_r){
	for (int i = 0; i < 4; i++){
		std::cout << bit_to_string_be(q_r[i]) << "   ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 4; i++){
		std::cout << bit_to_string_be(q_r[i+4]) << "   ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 4; i++){
		std::cout << bit_to_string_be(q_r[i+8]) << "   ";
	}
	std::cout << std::endl;
	std::cout << bit_to_string_be(q_r[12]) << "   ";
	for (int i = 0; i < 3; i++){
		std::cout << bit_to_string_be(q_r[i+13]) << "   ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

std::string bit_to_string(std::bitset<32> b){
	std::bitset<4> b1;
	std::string str;
	int c;
	std::string temp;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 4; j++){
			b1[j] = b[i*4+j];
		}
		c = b1.to_ulong();
		if (c > 9){
			temp += 'a' + c - 10;
		} else {
			temp += '0' + c;
		}
		if (i%2 == 1){
			temp = reverse(temp);
			str += temp;
			temp = "";
		}
	}
	return str;
}

std::string make_ciphertext(std::bitset<32> *q_r){
	std::string ct;
	for (int i = 0; i < 16; i++){
		ct += bit_to_string(q_r[i]);
	}
	return ct;
}

void twenty_rounds(std::bitset<32> *q_r){
	for (int i = 0; i < 10; i++){
		quarter_round(q_r, 0, 4, 8,12);
	    quarter_round(q_r, 1, 5, 9,13);
	    quarter_round(q_r, 2, 6,10,14);
	    quarter_round(q_r, 3, 7,11,15);
	    quarter_round(q_r, 0, 5,10,15);
	    quarter_round(q_r, 1, 6,11,12);
	    quarter_round(q_r, 2, 7, 8,13);
	    quarter_round(q_r, 3, 4, 9,14);
	}
}

void add(std::bitset<32> *q_r, std::bitset<32> *q_r_20){
	for (int i = 0; i < 16; i++){
		q_r[i] = add_32(q_r_20[i], q_r[i]);
	}
}

void create_cyphertext(std::string str, std::bitset<32> * q_r){
	std::string temp;
	for (int i = 7; i >= 0; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[0] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 15; i >= 8; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[1] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 23; i >= 16; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[2] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 31; i >= 24; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[3] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 39; i >= 32; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[4] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 47; i >= 40; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[5] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 55; i >= 48; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[6] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 63; i >= 56; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[7] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 71; i >= 64; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[8] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 79; i >= 72; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[9] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 87; i >= 80; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[10] ^= std::bitset<32> (temp);
	temp = "";
	for (int i = 95; i >= 88; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[11] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 103; i >= 96; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[12] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 111; i >= 104; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[13] ^= std::bitset<32>(temp);
	temp = "";
	for (int i = 119; i >= 112; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[14] ^= std::bitset<32> (temp);
	temp = "";
	for (int i = 127; i >= 120; i = i-2){
		temp += hex_to_bin(str[i-1]);
		temp += hex_to_bin(str[i]);
	}
	q_r[15] ^= std::bitset<32>(temp);
}

std::string cut_bytes(std::string str, int num_of_bites){
	std::string new_str = "";
	for (int i = 0; i < (str.size() - num_of_bites); i++){
		new_str += str[i+num_of_bites];
	}
	return new_str;
}

std::string inc_bytes(std::string str){
	for (int i = str.size(); i < 128; i++){
		str += '0';
	}
	return str;
}

std::string make_final_ciphertext(std::bitset<32> *q_r, int num_of_bites){
	std::string ct;
	int n = num_of_bites/8;
	for (int i = 0; i < n; i++){
		ct += bit_to_string(q_r[i]);
	}
	std::string str = bit_to_string(q_r[n]);
	str.resize(num_of_bites - 8*n);
	ct += str;
	return ct;
}

int main(int argc, char **argv){
	std::ifstream pt_f(argv[1], std::ios::in | std::ios::binary);
	std::string pt;
	pt_f >> pt;
	pt_f.close();
	std::ifstream key_f(argv[2], std::ios::in | std::ios::binary);
	std::string key;
	key_f >> key;
	key_f.close();
	std::ifstream iv_f(argv[3], std::ios::in | std::ios::binary);
	std::string iv;
	iv_f >> iv;
	iv_f.close();
	key += iv;
	std::bitset<32> q_r[16];
	std::bitset<32> q_r_20[16];
	std::string ct = "";
	int block_c = 1;
	int n = pt.size();
	for (int i = 0; i < n/128; i++){
		init(key, q_r, block_c);
		init(key, q_r_20, block_c);
		twenty_rounds(q_r_20);
		add(q_r, q_r_20);
		create_cyphertext(pt, q_r);
		ct += make_ciphertext(q_r);
		pt = cut_bytes(pt, 128);
		block_c++;
	}
	init(key, q_r, block_c);
	init(key, q_r_20, block_c);
	twenty_rounds(q_r_20);
	add(q_r, q_r_20);
	n = pt.size();
	pt = inc_bytes(pt);
	create_cyphertext(pt, q_r);
	ct += make_final_ciphertext(q_r, n);
	std::ofstream ct_f(argv[4], std::ios::out | std::ios::binary);
	ct_f << ct;
	ct_f.close();
	return 0;
}