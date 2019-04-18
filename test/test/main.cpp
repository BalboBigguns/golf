#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#define PI 3.14

#define VELOCITY
#define ANGLE

#define DISTANCE_BETWEEN_LINES 7

unsigned long times[10] = { 0 };

enum rotation { NONE, CLOCKWISE, COUNTERCLOCKWISE };  // typ skrecenia kija przy uderzeniu
enum rotation rotationError = NONE;

#ifdef VELOCITY
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
			//									TODO: tu mozna dopisac flage czy mamy chociaz jedna pare i ewentualnie sprobowac odzyskac po trojkacie
		}
	}

	printf("\nVelocity: Delty:\n");
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

	printf("\nVelocity: Delty posortowane:\n");
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

	printf("\nVelocity: Delty najblizsze:\n");
	printf("%d\t%d\n", delta[j], delta[j + 1]);

	double velocity;

	//w przypadku jak tylko jeden rzad zalapal caly, jest bez sredniej
	//																	TODO:(mozna sprobowac dopasowac po trojkacie zeby odzyskac wiecej wynikow)
	if (delta[j] == 0) {
		velocity = DISTANCE_BETWEEN_LINES / (double)delta[j + 1];
	}
	else if (delta[j + 1] == 0){
		velocity = DISTANCE_BETWEEN_LINES / (double)delta[j];
	}
	else {
		velocity = DISTANCE_BETWEEN_LINES / ((double)(delta[j + 1] + delta[j]) / 2);
	}

	return velocity;
}
#endif

#ifdef ANGLE
double calcAngle()
{
	double delta[4] = { 0 };
	double dSum = 0;
	double lastValue = 0;
	double angle = 0;

	//przeliczanie roznicy czasow
	for (int i = 5; i < 9; i++) {
		if (times[i] != 0) {
			for (int j = i + 1; j < 10; j++) {
				if (times[j] != 0) {
					delta[i - 5] = ((double)times[i] - times[j]) / (j - i);   // prawy timestamp odjac lewy timestamp (delty liczone od prawej do lewej strony)
					dSum += delta[i - 5];
					lastValue = delta[i - 5];
					break;
				}
			}
		}
		else {
			delta[i - 5] = lastValue;
			dSum += delta[i - 5];
		}
	}

	printf("\nAngle: Delty:\n");
	for (int x = 0; x < 4; x++) {
		printf("%lf\n", delta[x]);

	}

	printf("\nAngle: dSum: %lf\n", dSum);

	//ustalenie przewazajacego obrotu
	//														TODO: (mozna dodac jakis margines bledu dla NONE)
	if (dSum < 0) {
		rotationError = COUNTERCLOCKWISE;


	}
	else if (dSum > 0) {
		rotationError = CLOCKWISE;
	}
	else {
		rotationError = NONE;
		angle = 0;
	}

	double angle_st = 180 * angle / PI;

	return angle_st;
}
#endif

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

#ifdef VELOCITY
		double v = calcVelocity();
		printf("\nResult: %lf\n", v);
#endif

#ifdef ANGLE
		double a = calcAngle();
		printf("\nResult: %lf\n", a);
#endif

		system("pause");
	}

	return 0;
}