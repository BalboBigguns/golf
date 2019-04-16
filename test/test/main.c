#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISTANCE_BETWEEN_LINES 7

unsigned long times[10] = { 0 };

double calcVelocity()
{
	// funkcja liczy czas przejscia kija nad odpowiadajacymi sensorami
	// wybiera z nich dwa najbardziej podobne i usrednia
	// na podstawie tego czasu liczy predkosc uderzenia
	// [ cm / mikrosekundy]

	unsigned long delta[5] = { 0 };
	unsigned long temp, min;
	int i, j;

	for (i = 0; i < 5; i++) {
		if (times[i] != 0 && times[i + 5] != 0) {
			delta[i] = times[i + 5] - times[i];
		}
	}

	printf("\nDelty:\n");
	for (int x = 0; x < 5; x++) {
		printf("%d\n", delta[x]);

	}

	// stary dobry bubble
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 4 - i; j++) {
			if (delta[j] > delta[j + 1]) {
				temp = delta[j];
				delta[j] = delta[j + 1];
				delta[j + 1] = temp;
			}
		}
	}

	printf("\nDelty posortowane:\n");
	for (int x = 0; x < 5; x++) {
		printf("%d\n", delta[x]);

	}

	j = 0;  // indeks pierwszego z najbardziej podobnych elementow
	min = delta[4];   // min roznicy miedzy wartosciami

	for (i = 0; i < 4; i++) {
		if (delta[i] != 0) {
			temp = delta[i + 1] - delta[i];
			if (temp < min) {
				min = temp;
				j = i;
			}
		}
	}

	printf("\nDelty najblizsze:\n");
	printf("%d\t%d\n", delta[j], delta[j + 1]);

	return DISTANCE_BETWEEN_LINES / (double)(delta[j] + delta[j + 1] / 2);
}

int main()
{
	srand(time(NULL));

	for (;;) {
		for (int i = 0; i < 10; i++) {
			if (i < 5) {
				times[i] = 1000 + rand() % 100;
			}
			else {
				times[i] = 2000 + rand() % 100;
			}
		}

		times[rand() % 10] = 0;
		times[rand() % 10] = 0;

		system("cls");


		for (int i = 0; i < 10; i++) {
			printf("%d\n", times[i]);
		}

		double v = calcVelocity();

		printf("\nResult: %lf\n", v);
		system("pause");
	}

	return 0;
}