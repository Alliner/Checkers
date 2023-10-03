#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

void printBoard(int person1[][8], int person2[][8]);
bool checkTryStepFrom(int person[][8], int stateFrom);
bool checkTryStepTo(int person1[][8], int person2[][8], int board[][8], int stateFrom, int stateTo, int trend);
void changeSeat(int person[][8], int strFrom, int stbFrom, int strTo, int stbTo);
bool chanceToCutChip(int person1[][8], int person2[][8], int board[][8], int trend);
bool foundPath(int person1[][8], int person2[][8], int board[][8], int trend);
bool canEnemyKill(int person1[][8], int person2[][8], int str, int stb, int strNow, int stbNow);



void main() {
	setlocale(LC_ALL, "Russian");
	int board[8][8];

	int person1[8][8], person2[8][8];

	//зададим начальное состояние
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			board[i][j] = 0;
			person1[i][j] = 0;
			person2[i][j] = 0;
		}

	for (int i = 0; i < 3; i++)
		for (int j = ((i + 1) % 2); j < 8; j = j + 2) {
			person1[i][j] = 1;
		}
	for (int i = 5; i < 8; i++)
		for (int j = ((i + 1) % 2); j < 8; j = j + 2) {
			person2[i][j] = 1;
		}

	for (int i = 0; i < 8; i++)
		for (int j = (i % 2); j < 8; j = j + 2) {
			board[i][j] = 1;
		}
	//person1[0][3] = 2;
	printBoard(person1, person2);
	bool notwin = true;

	do {
		int stepFrom, stepTo;

		//проверить, можно ли ей сходить
		bool t;
		cout << "Ход игрока 1 (#):\n";
	chooseChecker:
		do {
			cout << "Введите координаты шашки, которой хотите сходить: ";
			cin >> stepFrom;
			t = checkTryStepFrom(person2, stepFrom);
			if (!t) cout << "Неверно выбрана шашка, попробуйте еще раз\n";
		} while (!t);

		do {
			cout << "Введите координаты, куда хотите сходить (или 0 чтобы выбрать другую шашку): ";
			cin >> stepTo;
			if (stepTo == 0) goto chooseChecker;
			t = checkTryStepTo(person2, person1, board, stepFrom, stepTo, -1);
			if (!t) cout << "Неверно выбрано место для хода, попробуйте еще раз\n";
		} while (!t);
		printBoard(person1, person2);
		//после того как сходил человек - сходить автоматически
		//ручной ход другого человека
		/*cout << "Ход игрока 2 (%):\n";
		do {
			cout << "Введите координаты шашки, которой хотите сходить: ";
			cin >> stepFrom;
			t = checkTryStepFrom(person1, stepFrom);
			if (!t) cout << "Неверно выбрана шашка, попробуйте еще раз\n";
		} while (!t);

		do {
			cout << "Введите координаты, куда хотите сходить: ";
			cin >> stepTo;
			t = checkTryStepTo(person1, person2, board, stepFrom, stepTo, 1);
			if (!t) cout << "Неверно выбрано место для хода, попробуйте еще раз\n";
		} while (!t);*/


		//автоматический ход

		//шаг 1: смотрим, нельзя ли кого-то срубить, тогда рубим
		if (chanceToCutChip(person1, person2, board, 1)) {
			cout << "\n\nНашел, что срубить!\n\n";
			printBoard(person1, person2);
			continue;
		}
		//шаг 2: ищем ходы
		foundPath(person1, person2, board, 1);

		//шаг3: оцениваем ходы, не срубят ли сразу

		printBoard(person1, person2);




		//system("cls");
		//notwin = false;
	} while (notwin);








}


void printBoard(int person1[][8], int person2[][8]) {
	int sumP1 = 0, sumP2 = 0;
	cout << "\n     1   2   3   4   5   6   7   8  \n";
	cout << "   ---------------------------------\n";
	for (int i = 0; i < 8; i++) {
		cout << " " << i + 1 << " |";
		for (int j = 0; j < 8; j++) {
			int fl = 0;
			//cout << " ";
			if (person1[i][j] != 0) {
				if (person1[i][j] == 1) cout << " %";
				if (person1[i][j] == 2) cout << "%%%";
				fl = 1;
				sumP1++;
			}
			if (person2[i][j] != 0) {
				if (person2[i][j] == 1) cout << " #";
				if (person2[i][j] == 2) cout << "###";
				fl = 1;
				sumP2++;
			}
			if (fl != 1) cout << "  ";
			if ((person1[i][j] != 2) and (person2[i][j] != 2)) cout << " ";
			cout << "|";
		}
		if (i != 7) cout << "\n   |---|---|---|---|---|---|---|---|\n";
		else cout << "\n   ---------------------------------\n";
	}

	cout << "Игрок 1 (#): " << sumP2 << endl;
	cout << "Игрок 2 (%): " << sumP1 << endl << endl;

}

bool checkTryStepFrom(int person[][8], int stateFrom) {
	int str, stb;
	str = stateFrom / 10 - 1;
	stb = stateFrom % 10 - 1;
	//проверяем, есть ли там шашка
	if (person[str][stb] > 0) return true;
	return false;
}

bool checkTryStepTo(int person1[][8], int person2[][8], int board[][8], int stateFrom, int stateTo, int trend) {
	int strFrom, stbFrom, strTo, stbTo;
	strFrom = stateFrom / 10 - 1;
	stbFrom = stateFrom % 10 - 1;
	strTo = stateTo / 10 - 1;
	stbTo = stateTo % 10 - 1;
	//проверяем, можно ли вообще вставать на эту клетку
	//cout << strFrom << "  " << stbFrom << "  " << strTo << "  " << stbTo << endl;

	if (board[strTo][stbTo] == 1) {
		//cout << "Ячейка доски\n"; 
		return false;
	}
	if ((strTo > 7) or (strTo < 0) or (stbTo > 7) or (stbTo < 0)) { //cout << "Выход за границы\n"; 
		return false;
	}
	//проверяем, нет ли там чьей-нибудь шашки
	if ((person1[strTo][stbTo] > 0) or (person2[strTo][stbTo] > 0)) { //cout << "Ячейка занята\n"; 
		return false;
	}
	if (person1[strFrom][stbFrom] == 1) {
		//проверяем, рядом ли эта клетка, если нет, то идем ли мы туда через "срубить"
		if ((strFrom + trend != strTo) or ((stbFrom + 1 != stbTo) and (stbFrom - 1 != stbTo))) {
			//cout << "Не рядом\n";
			//если клетка не находится строго через 1 по диагонали
			if ((abs(strTo - strFrom) != 2) or (abs(stbFrom - stbTo) != 2)) return false;
			//а если находится, то есть ли между ними фишка противника
			if ((person2[strFrom + trend][stbFrom - (stbFrom - stbTo) / 2] == 0) and (person2[strFrom - trend][stbFrom - (stbFrom - stbTo) / 2] == 0)) {
				//cout << "Нет фишки\n"; 
				return false;
			}
			//cout << "Фишка есть, срубить\n";
			person2[strFrom + trend][stbFrom - (stbFrom - stbTo) / 2] = 0;
		}
	}
	if (trend == 1) cout << "Ход противника: " << strFrom + 1 << stbFrom + 1 << "-->" << strTo + 1 << stbTo + 1 << endl;
	changeSeat(person1, strFrom, stbFrom, strTo, stbTo);
	return true;
}
void changeSeat(int person[][8], int strFrom, int stbFrom, int strTo, int stbTo) {
	person[strTo][stbTo] = person[strFrom][stbFrom];
	person[strFrom][stbFrom] = 0;
	if ((strTo == 0) or (strTo == 7)) person[strTo][stbTo] = 2;
}

bool chanceToCutChip(int person1[][8], int person2[][8], int board[][8], int trend) {
	bool cutChip = false;
	for (int i = 7; i >= 0; i--)
		for (int j = 7; j >= 0; j--)
		{
			//берем каждую фишку и смотрим.. так-то во все стороны получается, мало ли. Начать лучше с тех, что дальше зашли
			if (person1[i][j] > 0) {
			tryCutChip:
				try {
					for (int k = 1; k < (8 - max(i, j)); k++) {
						if (person2[i - k][j - k] > 0)
							if (checkTryStepTo(person1, person2, board, ((i + 1) * 10 + j + 1), ((i - k) * 10 + j - k), trend * (-1))) {
								cout << "Ход противника: " << i + 1 << j + 1 << "-->" << i - k << j - k << endl;
								cutChip = true;
								i = i - 1 - k;
								j = j - 1 - k;
								goto tryCutChip;
								//return true;
							}
						if (person1[i][j] == 1) break;

					}
				}
				catch (...) {}
				try {
					for (int k = 1; k < (8 - max(i, j)); k++) {
						if (person2[i - k][j + k] > 0)
							if (checkTryStepTo(person1, person2, board, ((i + 1) * 10 + j + 1), ((i - k) * 10 + j + 2 + k), trend * (-1))) {
								cout << "Ход противника: " << i + 1 << j + 1 << "-->" << i - k << j + 2 + k << endl;
								cutChip = true;
								i = i - 1 - k;
								j = j + 1 + k;
								goto tryCutChip;
								//return true;
							}
						if (person1[i][j] == 1) break;
					}
				}
				catch (...) {}
				try {
					for (int k = 1; k < (8 - max(i, j)); k++) {
						if (person2[i + 1][j - 1] > 0)
							if (checkTryStepTo(person1, person2, board, ((i + 1) * 10 + j + 1), ((i + 2 + k) * 10 + j - k), trend)) {
								cutChip = true;
								i = i + 1 + k;
								j = j - 1 - k;
								goto tryCutChip;
								//return true;
							}
						if (person1[i][j] == 1) break;
					}

				}
				catch (...) {}
				try {
					for (int k = 1; k < (8 - max(i, j)); k++) {
						if (person2[i + 1][j + 1] > 0)
							if (checkTryStepTo(person1, person2, board, ((i + 1) * 10 + j + 1), ((i + 2 + k) * 10 + j + 2 + k), trend)) {
								cutChip = true;
								i = i + 1 + k;
								j = j + 1 + k;
								goto tryCutChip;
								//return true;
							}
						if (person1[i][j] == 1) break;
					}
				}
				catch (...) {}
				if (cutChip) return true;
			}
		}
	return false;
}

bool foundPath(int person1[][8], int person2[][8], int board[][8], int trend) {
	//создадим матрицу, где оценим, есть ли фишки, которые могут сходить и их не срубят
	int copy[8][8];
	int fl = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			copy[i][j] = 0;
	for (int i = 6; i >= 0; i--)
		for (int j = 7; j > 0; j--)
			if (person1[i][j] > 0) {
				//cout << "Найдена фишка в "<<i+1<<j+1<<"\n";
				if (j > 0) {
					//проверим, пустая ли клетка вниз (или вверх, зависит от тренда) и влево
					if ((person1[i + trend][j - 1] == 0) and (person2[i + trend][j - 1] == 0)) {
						//cout << "пустая клетка вниз влево " << i + 1 + trend << j + 1 -1 << "\n";
						if (canEnemyKill(person1, person2, i + trend, j - 1, i, j) == false) {
							//cout << "false " << i + 1 + trend << j + 1 - 1 << "\n";
							copy[i][j] = person1[i][j];
							fl = 1;
							cout << "Ход противника: " << i + 1 << j + 1 << "-->" << i + trend + 1 << j;
							changeSeat(person1, i, j, i + trend, j - 1);
							return true;
						}

					}
				}
				if (j < 7) {
					//проверим, пустая ли клетка вниз (или вверх, зависит от тренда) и вправо
					if ((person1[i + trend][j + 1] == 0) and (person2[i + trend][j + 1] == 0)) {
						//cout << "пустая клетка вниз вправо" << i + 1 + trend << j + 1+1 << "\n";
						if (canEnemyKill(person1, person2, i + trend, j + 1, i, j) == false) {
							//cout << "false " << i + 1 + trend << j + 1 + 1 << "\n";
							copy[i][j] = person1[i][j];
							fl = 1;
							cout << "Ход противника: " << i + 1 << j + 1 << "-->" << i + trend + 1 << j + 2;
							changeSeat(person1, i, j, i + trend, j + 1);
							return true;
						}
					}
				}
			}

	//если да, то среди них ищем кратчайший путь и снова проверяем
	if (fl == 1) {
		cout << "Дошел до условия\n";

	}

	//если нет...
	if (fl == 0) {
		cout << "Дошел до условия\n";
	}


}

bool canEnemyKill(int person1[][8], int person2[][8], int str, int stb, int strNow, int stbNow) {
	try {
		if (person2[str + 1][stb - 1] > 0) {
			cout << str + 1 << stb + 1 << "-->" << str + 1 + 1 << stb - 1 + 1 << "  Слева снизу кто-то есть, проверяем " << str - 1 + 1 << stb + 1 + 1 << "\n";
			if ((person1[str - 1][stb + 1] == 0) and (person2[str - 1][stb + 1] == 0)) return true;
			if (((str - 1) == strNow) and ((stb + 1) == stbNow)) return true;
		}
	}
	catch (...) {}

	try {
		if (person2[str + 1][stb + 1] > 0) {
			if ((person1[str - 1][stb - 1] == 0) and (person2[str - 1][stb - 1] == 0)) return true;
			if (((str - 1) == strNow) and ((stb - 1) == stbNow)) return true;
		}
	}
	catch (...) {}

	try {
		if (person2[str - 1][stb - 1] > 0) {
			if ((person1[str + 1][stb + 1] == 0) and (person2[str + 1][stb + 1] == 0)) return true;
			if (((str + 1) == strNow) and ((stb + 1) == stbNow)) return true;
		}
	}
	catch (...) {}

	try {
		if (person2[str - 1][stb + 1] > 0) {
			if ((person1[str + 1][stb - 1] == 0) and (person2[str + 1][stb - 1] == 0)) return true;
			if (((str + 1) == strNow) and ((stb - 1) == stbNow)) return true;
		}
	}
	catch (...) {}
	return false;
}