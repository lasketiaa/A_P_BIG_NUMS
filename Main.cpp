#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE_ARR 1000

size_t max(size_t one, size_t two); // просто находит максимум

void fill_arr(int *pnum_1, int* pnum_2, size_t len_1, size_t len_2, 
	size_t max_len, char *s_num_1, char *s_num_2); // три блока if, процедура заполняет выделеннуую память введенными значениями (DO!)

int main(void) {
	size_t len_1 = 0, len_2 = 0, max_len = 0; // это для будущего размера массива 
	int cnt = 0, buff = 0;
	char s_num_1[MAX_SIZE_ARR] = { 0 }, s_num_2[MAX_SIZE_ARR] = { 0 }; /* тут создаются списки,
	чтобы туда вводились числа, а {0} заполняет перый символ 0, все осальнеы тож будут 0 */
	bool natural_num_1, natural_num_2;

	scanf_s("%s", s_num_1, MAX_SIZE_ARR);
	scanf_s("%s", s_num_2, MAX_SIZE_ARR);

	natural_num_1 = s_num_1[0] != '-'; // если в начале был -, то число отриц. False
	natural_num_2 = s_num_2[0] != '-';

	len_1 = strlen(s_num_1); //strlen() возывращает длину, 
	len_2 = strlen(s_num_2);
	max_len = max(len_1, len_2);

	// объявляем указатели, выбирается макс. — это количесвто символов, вторым аргументом размерность этих элементов
	// выделенная память заполнена нулями
	int *pnum_1 = (int*)calloc(max_len, sizeof(int));
	int *pnum_2 = (int*)calloc(max_len, sizeof(int));

	fill_arr(pnum_1, pnum_1, len_1, len_2, max_len, s_num_1, s_num_2);

	// указатель, в нем на 1 разряд больше должен быть на всякий случай
	int* p_result = (int*)calloc(max_len+1, sizeof(int));
	
	// перебираем элементы в массивах, которые заполняли перед этим
	for (int i = max_len; i >= 0; i--) {
		buff = pnum_1[i - 1] + pnum_2[i - 1] + cnt; // сумма

		if (buff > 9 && i == 1) { // если у нас последняя сумма, и при этом число > 9, 
			p_result[i] = buff % 10; // то запишем вручную сначала остаток, а потом 1
			p_result[i - 1] = cnt; 
			break; // можно и без брейк, на одну итерацию больше будет
		}
		else if (buff > 9) { // если сумма больше 9, то запишем разряд единиц и запомним 1
			p_result[i] = buff % 10;
			cnt = 1;
		}
		else cnt = 0;
	}

	return 0;
}

size_t max(size_t one, size_t two) {
	if (one > two) return one;
	return two;
}

void fill_arr(int* pnum_1, int* pnum_2, size_t len_1, size_t len_2,
	size_t max_len, char *s_num_1, char *s_num_2) {
	if (len_1 == len_2) {
		for (int i = 0; i < max_len; i++) {
			pnum_1[i] = s_num_1[i] - '0';
			pnum_2[i] = s_num_2[i] - '0';
		}
	}

	if (len_1 > len_2) {
		for (int i = 0; i < max_len; i++) {
			if (i >= max_len - len_2) {
				pnum_2[i] = s_num_2[i - (max_len - len_2)] - '0';
			}
			pnum_1[i] = s_num_1[i] - '0';
		}
	}

	if (len_1 < len_2) {
		for (int i = 0; i < max_len; i++) {
			if (i >= max_len - len_1) {
				pnum_1[i] = s_num_1[i - (max_len - len_1)] - '0';
			}
			pnum_2[i] = s_num_2[i] - '0';
		}
	}
}
