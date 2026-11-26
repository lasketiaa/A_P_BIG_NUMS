#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_ARR 1001 // 1000 + \0

// создает число из строки. цифры записываются справа налево: 123 -> [3, 2, 1]
int* create_num(char* str, size_t* len, int* sign);

// освобождает память числа
void free_num(int* num);

// убирает лишние нули в конце массива, в нашем массиве конец это старшие разряды
void remove_zeros(int* num, size_t* len);

// печатает число с учетом его знака
void print_num(int* num, size_t len, int sign);

// сравнивает модули двух чисел: 1 - первое больше, 0 — равны, -1 — второе больше
int compare_abs(int* num_1, size_t len_1, int* num_2, size_t len_2);

// складывает модули двух чисел
int* add_abs(int* num_1, size_t len_1, int* num_2, size_t len_2, size_t* result_len);

// вычитает модули: первое число должно быть больше или равно второму
int* subtract_abs(int* num_1, size_t len_1, int* num_2, size_t len_2, size_t* result_len);

// складывает числа с учетом знаков
int* add_num(int* num_1, size_t len_1, int sign_1,
	int* num_2, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign);

// вычитает второе число из первого
int* subtract_num(int* num_1, size_t len_1, int sign_1,
	int* num_2, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign);

// умножает два числа
int* multiply_num(int* num_1, size_t len_1, int sign_1,
	int* num_2, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign);

// проверяет является ли число нулём
bool is_zero(int* num, size_t len);

// уменьшает положительное число на единицу
int* subtract_one(int* num, size_t len, size_t* result_len);

// возводит число в неотрицательную степень
int* power_num(int* num, size_t len, int sign,
	int* power, size_t power_len,
	size_t* result_len, int* result_sign);

// считает факториал неотрицательного числа
int* factorial_num(int* num, size_t len, size_t* result_len);

// делит положительное длинное число на обычное маленькое число
int* divide_by_int(int* num, size_t len, int divider, size_t* result_len);

// меняет местами два числа вместе с длинами и знаками
void swap_nums(int** num_1, size_t* len_1, int* sign_1,
	int** num_2, size_t* len_2, int* sign_2);

// считает сумму чисел от ферст до секонд вкл
int* sum_from_to(int* first, size_t len_1, int sign_1,
	int* second, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign);

int main(void) {
	int operation = 0;
	char str_1[MAX_SIZE_ARR] = { 0 };
	char str_2[MAX_SIZE_ARR] = { 0 };

	size_t len_1 = 0, len_2 = 0, result_len = 0;
	int sign_1 = 1, sign_2 = 1, result_sign = 1;

	printf("LONG ARITHMETIC\n");
	printf("1 - addition\n");
	printf("2 - subtraction\n");
	printf("3 - multiplication\n");
	printf("4 - power\n");
	printf("5 - factorial\n");
	printf("6 - sum from first to second\n");
	printf("Enter operation: ");
	scanf_s("%d", &operation);

	if (operation < 1 || operation > 6) {
		printf("That operation is not supported\n");
		return 0;
	}

	printf("Enter the first number: ");
	scanf_s("%1000s", str_1, MAX_SIZE_ARR);

	// превращаем строку в массив
	int* num_1 = create_num(str_1, &len_1, &sign_1);

	// во всех операциях(кроме факториала) нужно второе число
	int* num_2 = NULL;

	if (operation != 5) {
		printf("Enter the second number: ");
		scanf_s("%1000s", str_2, MAX_SIZE_ARR);

		num_2 = create_num(str_2, &len_2, &sign_2);
	}

	int* result = NULL;

	if (operation == 1) {
		result = add_num(num_1, len_1, sign_1,
			num_2, len_2, sign_2,
			&result_len, &result_sign);
	}

	if (operation == 2) {
		result = subtract_num(num_1, len_1, sign_1,
			num_2, len_2, sign_2,
			&result_len, &result_sign);
	}

	if (operation == 3) {
		result = multiply_num(num_1, len_1, sign_1,
			num_2, len_2, sign_2,
			&result_len, &result_sign);
	}

	if (operation == 4) {
		// отрицательные степени нельзя, дробный результат
		if (sign_2 < 0) {
			printf("Negative power is not supported\n");
		}
		else {
			result = power_num(num_1, len_1, sign_1,
				num_2, len_2,
				&result_len, &result_sign);

			printf("Result: ");
			print_num(result, result_len, result_sign);
			printf("\n");
		}
	}

	if (operation == 5) {
		if (sign_1 < 0) {
			printf("Factorial of a negative number is not defined\n");
		}
		else {
			result = factorial_num(num_1, len_1, &result_len);

			printf("Result: ");
			print_num(result, result_len, 1);
			printf("\n");
		}
	}

	if (operation == 6) {
		result = sum_from_to(num_1, len_1, sign_1,
			num_2, len_2, sign_2,
			&result_len, &result_sign);

		printf("Result: ");
		print_num(result, result_len, result_sign);
		printf("\n");
	}

	if (operation >= 1 && operation <= 3) {
		printf("Result: ");
		print_num(result, result_len, result_sign);
		printf("\n");
	}

	free_num(num_1);
	free_num(num_2);
	free_num(result);

	return 0;
}

int* create_num(char* str, size_t* len, int* sign) {
	size_t str_len = strlen(str);
	size_t start = 0;

	*sign = 1; // по умолчанию число положительное

	// пропускаем минус
	if (str[0] == '-') {
		*sign = -1;
		start = 1;
	}

	if (start == str_len) {
		*len = 1;
		return (int*)calloc(1, sizeof(int));
	}

	*len = str_len - start;

	// выделяем столько, сколько цифр в числе
	int* num = (int*)calloc(*len, sizeof(int));

	// записываем цифры справа налево
	for (size_t i = 0; i < *len; i++) {
		num[i] = str[str_len - 1 - i] - '0';
	}

	remove_zeros(num, len);

	// число -0 превращаем в 0
	if (*len == 1 && num[0] == 0) {
		*sign = 1;
	}

	return num;
}

void free_num(int* num) {
	if (num != NULL) {
		free(num);
	}
}

void remove_zeros(int* num, size_t* len) {
	// старшие разрды лежат в конце массива
	while (*len > 1 && num[*len - 1] == 0) {
		(*len)--;
	}
}

void print_num(int* num, size_t len, int sign) {
	// чтобы ноль не выводлся как -0
	if (sign < 0 && !is_zero(num, len)) {
		printf("-");
	}

	// печатаем с конца, тк цифры хранятся наоборот
	for (size_t i = len; i > 0; i--) {
		printf("%d", num[i - 1]);
	}
}

int compare_abs(int* num_1, size_t len_1, int* num_2, size_t len_2) {
	// число с большим количеством цифр больше
	if (len_1 > len_2) return 1;
	if (len_1 < len_2) return -1;

	// если длины одинаковы, сравниваем старшие цифры
	for (size_t i = len_1; i > 0; i--) {
		if (num_1[i - 1] > num_2[i - 1]) return 1;
		if (num_1[i - 1] < num_2[i - 1]) return -1;
	}

	return 0;
}

int* add_abs(int* num_1, size_t len_1, int* num_2, size_t len_2, size_t* result_len) {
	size_t max_len = len_1 > len_2 ? len_1 : len_2;

	// +1 нужен на случай переноса
	int* result = (int*)calloc(max_len + 1, sizeof(int));
	int carry = 0;

	for (size_t i = 0; i < max_len; i++) {
		int digit_1 = i < len_1 ? num_1[i] : 0;
		int digit_2 = i < len_2 ? num_2[i] : 0;

		result[i] = (digit_1 + digit_2 + carry) % 10;
		carry = (digit_1 + digit_2 + carry) / 10;
	}

	result[max_len] = carry;
	*result_len = max_len + 1;

	remove_zeros(result, result_len);

	return result;
}

int* subtract_abs(int* num_1, size_t len_1, int* num_2, size_t len_2, size_t* result_len) {
	// здесь num_1 должен быть не меньше num_2 (!!!!)
	int* result = (int*)calloc(len_1, sizeof(int));
	int buff = 0;

	for (size_t i = 0; i < len_1; i++) {
		int digit_1 = num_1[i] - buff;
		int digit_2 = i < len_2 ? num_2[i] : 0;

		// занимаем десяток у след разряда
		if (digit_1 < digit_2) {
			digit_1 += 10;
			buff = 1;
		}
		else {
			buff = 0;
		}

		result[i] = digit_1 - digit_2;
	}

	*result_len = len_1;
	remove_zeros(result, result_len);

	return result;
}

int* add_num(int* num_1, size_t len_1, int sign_1,
	int* num_2, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign) {

	// есл одинаковые знаки складываем модули
	if (sign_1 == sign_2) {
		int* result = add_abs(num_1, len_1, num_2, len_2, result_len);

		*result_sign = sign_1;

		if (is_zero(result, *result_len)) {
			*result_sign = 1;
		}

		return result;
	}

	// если разные знаки, вычитаем меньший модуль из большего
	int compare = compare_abs(num_1, len_1, num_2, len_2);

	if (compare == 0) {
		*result_len = 1;
		*result_sign = 1;
		return (int*)calloc(1, sizeof(int));
	}

	if (compare > 0) {
		*result_sign = sign_1;
		return subtract_abs(num_1, len_1, num_2, len_2, result_len);
	}

	*result_sign = sign_2;
	return subtract_abs(num_2, len_2, num_1, len_1, result_len);
}

int* subtract_num(int* num_1, size_t len_1, int sign_1,
	int* num_2, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign) {

	// a - b == a + (-b)
	return add_num(num_1, len_1, sign_1,
		num_2, len_2, -sign_2,
		result_len, result_sign);
}

int* multiply_num(int* num_1, size_t len_1, int sign_1,
	int* num_2, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign) {

	// если один из множителей ноль — результат ноль
	if (is_zero(num_1, len_1) || is_zero(num_2, len_2)) {
		*result_len = 1;
		*result_sign = 1;
		return (int*)calloc(1, sizeof(int));
	}

	// максимум цифр в произведении -> len_1 + len_2
	int* result = (int*)calloc(len_1 + len_2, sizeof(int));

	// умножение столбиком
	for (size_t i = 0; i < len_1; i++) {
		for (size_t j = 0; j < len_2; j++) {
			result[i + j] += num_1[i] * num_2[j];
		}
	}

	// переносим десятки в старшие разряды
	for (size_t i = 0; i < len_1 + len_2 - 1; i++) {
		result[i + 1] += result[i] / 10;
		result[i] %= 10;
	}

	*result_len = len_1 + len_2;
	*result_sign = sign_1 * sign_2;

	remove_zeros(result, result_len);

	return result;
}

bool is_zero(int* num, size_t len) {
	return len == 1 && num[0] == 0;
}

int* subtract_one(int* num, size_t len, size_t* result_len) {
	// выделяем место под копию числа
	int* result = (int*)calloc(len, sizeof(int));

	// копируем цифры
	for (size_t i = 0; i < len; i++) {
		result[i] = num[i];
	}

	// вычитаем единицу справа налево
	for (size_t i = 0; i < len; i++) {
		if (result[i] > 0) {
			result[i]--;
			break;
		}

		result[i] = 9;
	}

	*result_len = len;
	remove_zeros(result, result_len);

	return result;
}

int* power_num(int* num, size_t len, int sign,
	int* power, size_t power_len,
	size_t* result_len, int* result_sign) {

	// рез начинается с единицы
	size_t current_len = 1;
	int current_sign = 1;
	int* result = (int*)calloc(1, sizeof(int));
	result[0] = 1;

	// делаем копию степени, чтобы уменьшать ее
	int* current_power = (int*)calloc(power_len, sizeof(int));

	for (size_t i = 0; i < power_len; i++) {
		current_power[i] = power[i];
	}

	// пока степень не стала 0
	while (!is_zero(current_power, power_len)) {
		size_t new_len = 0;
		int new_sign = 1;

		int* new_result = multiply_num(result, current_len, current_sign,
			num, len, sign, &new_len, &new_sign);

		int* new_power = subtract_one(current_power, power_len, &power_len);

		free(result);
		free(current_power);

		result = new_result;
		current_power = new_power;
		current_len = new_len;
		current_sign = new_sign;
	}

	free(current_power);

	*result_len = current_len;
	*result_sign = current_sign;

	return result;
}

int* factorial_num(int* num, size_t len, size_t* result_len) {
	// результат начинается с единицы
	size_t current_len = 1;
	int current_sign = 1;
	int* result = (int*)calloc(1, sizeof(int));
	result[0] = 1;

	// создаём копию введённого числа
	int* current = (int*)calloc(len, sizeof(int));

	for (size_t i = 0; i < len; i++) {
		current[i] = num[i];
	}

	// пока current не стал равен нулю
	while (!is_zero(current, len)) {
		size_t new_len = 0;
		int new_sign = 1;

		int* new_result = multiply_num(result, current_len, current_sign,
			current, len, 1, &new_len, &new_sign);

		int* new_current = subtract_one(current, len, &len);

		free(result);
		free(current);

		result = new_result;
		current = new_current;
		current_len = new_len;
		current_sign = new_sign;
	}

	free(current);

	*result_len = current_len;

	return result;
}

int* divide_by_int(int* num, size_t len, int divider, size_t* result_len) {
	// создаём массив той же длины
	int* result = (int*)calloc(len, sizeof(int));
	int remainder = 0;

	// деление столбиком идёт от старшей цифры к младшей
	for (size_t i = len; i > 0; i--) {
		int current = remainder * 10 + num[i - 1];

		result[i - 1] = current / divider;
		remainder = current % divider;
	}

	*result_len = len;
	remove_zeros(result, result_len);

	return result;
}

void swap_nums(int** num_1, size_t* len_1, int* sign_1,
	int** num_2, size_t* len_2, int* sign_2) {

	int* temp_num = *num_1;
	size_t temp_len = *len_1;
	int temp_sign = *sign_1;

	*num_1 = *num_2;
	*len_1 = *len_2;
	*sign_1 = *sign_2;

	*num_2 = temp_num;
	*len_2 = temp_len;
	*sign_2 = temp_sign;
}

int* sum_from_to(int* first, size_t len_1, int sign_1,
	int* second, size_t len_2, int sign_2,
	size_t* result_len, int* result_sign) {

	// если границы перепутаны, меняем их местами
	int compare = compare_abs(first, len_1, second, len_2);

	if ((sign_1 > sign_2) ||
		(sign_1 == sign_2 && sign_1 > 0 && compare > 0) ||
		(sign_1 == sign_2 && sign_1 < 0 && compare < 0)) {

		swap_nums(&first, &len_1, &sign_1,
			&second, &len_2, &sign_2);
	}

	// количество чисел second - first + 1
	size_t count_len = 0, one_len = 1;
	int count_sign = 1, one_sign = 1;

	int* count = subtract_num(second, len_2, sign_2,
		first, len_1, sign_1,
		&count_len, &count_sign);

	int* one = (int*)calloc(1, sizeof(int));
	one[0] = 1;

	int* count_plus_one = add_num(count, count_len, count_sign,
		one, one_len, one_sign,
		&count_len, &count_sign);

	// складываем две границы
	size_t borders_len = 0;
	int borders_sign = 1;

	int* borders_sum = add_num(first, len_1, sign_1,
		second, len_2, sign_2,
		&borders_len, &borders_sign);

	// формула суммы: (first + second) * count / 2
	size_t multiply_len = 0;
	int multiply_sign = 1;

	int* multiply_result = multiply_num(borders_sum, borders_len, borders_sign,
		count_plus_one, count_len, count_sign,
		&multiply_len, &multiply_sign);

	int* result = divide_by_int(multiply_result, multiply_len, 2, result_len);

	*result_sign = multiply_sign;

	if (is_zero(result, *result_len)) {
		*result_sign = 1;
	}

	free(count);
	free(one);
	free(count_plus_one);
	free(borders_sum);
	free(multiply_result);

	return result;
}