#include <string>
#include <iostream>
#include <vector>

int Sbox[4] = {1,3,0,2};

std::vector<int> create_block(char *argv){
	std::vector<int> block;
	for (int j = 0; j < strlen(argv); j++){
		if (argv[j] != ' ')
			block.push_back(argv[j]-'0');
	}
	return block;
}

std::vector<int> get_blocks(int argc, char **argv){
	std::vector<int> blocks;
		for (int i = 2; i < argc; i++){
			for (int j = 0; j < strlen(argv[i]); j++){
				if (argv[i][j] != ' ')
				blocks.push_back(argv[i][j]-'0');
			}
		}
	return blocks;
}


void print_vect(std::vector<int> vec){
	for (int i = 0; i < vec.size(); i++){
		std::cout << vec[i];
	}
	std::cout << std::endl;
}

std::vector<int> big_xor(std::vector<int> a, std::vector<int> b){ //если одинаковая длина
	std::vector<int> res;
	for (int i = 0; i < a.size(); i++){
		res.push_back(a[i]^b[i]);
	}
	return res;
}


void extend_block(std::vector<int> *block){
	if (block->size() % 16 != 0){
		int i = 16 - block->size()%16;
		while (block->size() % 16 != 0){
			block->push_back(i%4);
		}
	} else {
		for (int i = 0; i < 16; i++){
			block->push_back(0);
		}
	}
}

void add_contsum(std::vector<int> *block){
	int c_sum[16];
	for (int i = 0; i < 16; i++){
		c_sum[i] = 0;
	}
	int l = 0;
	int c;
	for (int i = 0; i < block->size()/16; i++){
		for (int j = 0; j < 16; j++){
			c = (*block)[i*16+j];
			c_sum[j] ^= Sbox[c ^ l];
			l = c_sum[j];
		}
	}
	for (int i = 0; i < 16; i++){
		block->push_back(c_sum[i]);
	}
}

std::vector<int> hash (std::vector<int> block){
	int X[48];
	for (int i = 0; i < 48; i++){
		X[i] = 0;
	}
	for (int i = 0; i < block.size()/16; i++){
		for (int j = 0; j < 16; j++){
			X[16+j] = block[i*16+j];
			X[32+j] = X[16+j] ^ X[j];
		}
		int t = 0;
		for (int j = 0; j < 18; j++){
			for (int k = 0; k < 48; k++){
				X[k] = X[k] ^ Sbox[t];
				t = X[k];
			}
			t = (t+j)%4;
		}
	}
	std::vector<int> hash;
	for (int i = 0; i < 16; i++){
		hash.push_back(X[i]);
	}
	return hash;
}


std::vector<int> compress (std::vector<int> H, std::vector<int> M, std::vector<int> H_xor_M){
	int A[19][16];
	A[1][0] = 0;
	int B[19][16];
	int C[19][16];
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 16; j++){
			A[i][j] = 0;
			B[i][j] = 0;
			C[i][j] = 0;
		}
	}
	for (int i = 0; i < 16; i++){
		A[0][i] = H[i];
	}
	for (int i = 0; i < 16; i++){
		B[0][i] = M[i];
	}
	for (int i = 0; i < 16; i++){
		C[0][i] = H_xor_M[i];
	}
	for (int i = 1; i < 18; i++){
		if (i != 1){
			A[i][0] = A[i-1][0] ^ Sbox[(C[i-1][15]+i-2)%4];
		}
		for (int j = 1; j < 16; j++){
			A[i][j] = A[i-1][j] ^ Sbox[A[i][j-1]];
		}
		B[i][0] = B[i-1][0] ^ Sbox[A[i][15]];
		for (int j = 1; j < 16; j++){
			B[i][j] = B[i-1][j] ^ Sbox[B[i][j-1]];
		}
		C[i][0] = C[i-1][0] ^ Sbox[B[i][15]];
		for (int j = 1; j < 16; j++){
			C[i][j] = C[i-1][j] ^ Sbox[C[i][j-1]];
		}
	}
	A[18][0] = A[17][0] ^ Sbox[(C[17][15]+18-2)%4];
	for (int j = 1; j < 16; j++){
		A[18][j] = A[17][j] ^ Sbox[A[18][j-1]];
	}
	std::vector<int> res;
	for (int i = 0; i < 16; i++){
		res.push_back(A[18][i]);
	}
	return res;
}

void add_one(int *a, int*b, int*c, int*d){
	if (*d == 3){
		*d = 0;
		if (*c == 3){
			*c = 0;
			if (*b == 3){
				*b = 0;
				(*a)++;
			} else (*b)++;
		} else (*c)++;
	} else (*d)++;
}

void add_one(int *a, int*b, int*c, int*d, int*e, int*f, int*g, int*h){
	if (*h == 3){
		*h = 0;
		if (*g == 3){
			*g = 0;
			if (*f == 3){
				*f = 0;
				if (*e == 3){
					*e = 0;
					if (*d == 3){
						*d = 0;
						if (*c == 3){
							*c = 0;
							if (*b == 3){
								*b = 0;
								(*a)++;
							} else (*b)++;
						} else (*c)++;
					} else (*d)++;
				} else (*e)++;
			} else (*f)++;
		} else (*g)++;
	} else (*h)++;
	return;
}

bool check_value(int up, int left, int cur){
	if ((up ^ Sbox[left]) != cur){
		return false;
	} else return true;
}

int rev_Sbox(int a){
	for (int i = 0; i < 4; i++){
		if (Sbox[i] == a) return i;
	}
	return -1;
}

bool compare(int *A, int *B){
	for (int i = 0; i < 8; i++){
		if (A[i] != B[i]) return false;
	} 
	return true;
}

std::string create_string_4 (int a, int b, int c, int d){
	std::string str;
	str += a + '0';
	str += b + '0';
	str += c + '0';
	str += d + '0';
	return str;
}

std::string create_string_8 (int a, int b, int c, int d, int e, int f, int g, int h){
	std::string str;
	str += a + '0';
	str += b + '0';
	str += c + '0';
	str += d + '0';
	str += e + '0';
	str += f + '0';
	str += g + '0';
	str += h + '0';
	return str;
}

void sort_arr (std::string *T[65535]){
	std::string temp = "";
	for (int i = 0; i < 65535; i++){
		for (int j = i; j < 65535; j++){
			if (T[i][0] > T[j][0]){
				temp = T[i][0];
				T[i][0] = T[j][0];
				T[j][0] = temp;
			}
		}
	}
}

void preimage (std::vector<int> H, std::vector<int> H_xor_M){
	int A[19][16];
	int T1[65535][8];
	int T2[65535][8];
	int B[19][16];
	int C[19][16];
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 16; j++){
			A[i][j] = 0;
			B[i][j] = 0;
			C[i][j] = 0;
		}
	}
	for (int i = 0; i < 16; i++){
		A[0][i] = H[i];
		if (i != 0) A[1][i] = A[0][i] ^ Sbox[A[1][i-1]];
		else A[1][i] = A[0][i] ^ Sbox[0]; 
		A[18][i] = H_xor_M[i];
	}
	int col = 1;
	int row = 17;
	while (col < 16){
		for (int i = col; i < 16; i++){
			A[row][i] =  A[row+1][i] ^ Sbox[A[row+1][i-1]];
		}
		col++;
		row--;
	}
	for (int l = 0; l < 4; l++){
		A[2][0] = l;
		for (int j = 1; j < 16; j++){
			A[2][j] = A[1][j] ^ Sbox[A[2][j-1]];
		}
		col = 14;
		for (int k = 3; k < 18; k++){
			for (int j = col; j >= 0; j--){
				A[k][j] = rev_Sbox(A[k-1][j+1] ^ A[k][j+1]);
			}
			col--;
		}
		for (int i = 1; i < 5; i++){
				C[i][15] = (abs(rev_Sbox(A[i+1][0] ^ A[i][0]) - i + 1))%4;
		}
		for (int k = 0; k < 256; k++){
			for (int t = 0; t < 65535; t++){
				for (int i = 0; i < 8; i++){
					C[0][i] = A[0][i] ^ B[0][i];
				}
				for (int i = 1; i < 5; i++){
					C[i][0] = Sbox[B[i][15]] ^ C[i-1][0];
					B[i][0] = Sbox[A[i][15]] ^ B[i-1][0];
				}
				for (int i = 1; i < 5; i++){
					for (int j = 1; j < 8; j++){
						C[i][j] = Sbox[C[i][j-1]] ^ C[i-1][j];
						B[i][j] = Sbox[B[i][j-1]] ^ B[i-1][j];
					}
				}
				for (int i = 1; i < 5; i++){				
					T1[t][i-1] = B[i][7];
					T1[t][i+3] = C[i][7];
				}
				add_one(&B[0][0],&B[0][1],&B[0][2],&B[0][3],&B[0][4],&B[0][5],&B[0][6],&B[0][7]);
			}
			for (int i = 0; i < 16; i++){
				B[0][i] = 0;
				C[0][i] = 0;
			}
			for (int t = 0; t < 65535; t++){
				for (int i = 8; i < 16; i++){
					C[0][i] = A[0][i] ^ B[0][i];
				}
				for (int i = 1; i < 5; i++){
					for (int j = 14; j > 6; j--){
						C[i][j] = rev_Sbox(C[i-1][j+1] ^ C[i][j+1]);
						B[i][j] = rev_Sbox(B[i-1][j+1] ^ B[i][j+1]);
					}
				}
				for (int i = 1; i < 5; i++){
					T2[t][i-1] = B[i][7];
					T2[t][i+3] = C[i][7];
				}
				add_one(&B[0][8],&B[0][9],&B[0][10],&B[0][11],&B[0][12],&B[0][13],&B[0][14],&B[0][15]);
			}
			for (int i = 0; i < 16; i++){
				B[0][i] = 0;
				C[0][i] = 0;
			}
			add_one(&B[1][15], &B[2][15], &B[3][15], &B[4][15]);
			for (int i = 0; i < 65535; i++){
				for (int j = i; j < 65535; j++){
					if (compare(T1[i],T2[j])) {
						std::cout << i << " " << j << std::endl;
						if ((i == 3748) && (j == 3318)) std::cout << "found!" << std::endl;
					}
				}
			}

		}
		for (int i = 1; i < 5; i++){
			B[i][15] = 0;
		}
	}
}


int main(int argc, char **argv){
	/* a) MD2 */
	if (!strcmp(argv[1],"md2")){
		std::vector<int> block;
		block = get_blocks(argc, argv);
		extend_block(&block);
		add_contsum(&block);
		std::vector<int> md2 = hash(block);
		print_vect(md2);
	}
	/* b) compress */
	if (!strcmp(argv[1], "compress")){
		std::vector<int> H = create_block(argv[2]);
		std::vector<int> M = create_block(argv[3]);
		std::vector<int> H_xor_M = big_xor(H,M);
		print_vect(compress(H,M,H_xor_M));
	}
	/* c) preimage */
	if (!strcmp(argv[1], "preimage")){
		std::vector<int> H = create_block(argv[2]);
		std::vector<int> H_xor_M = create_block(argv[3]);
		preimage(H,H_xor_M);
	}
	return 0;
}