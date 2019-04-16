#include <math.h>
#include <LiquidCrystal_I2C.h>

#define DISTANCE_BETWEEN_LINES 7   // odleglosc miedzy liniami I i II
#define DISTANCE_BETWEEN_SENSORS 1 // odleglosc miedzy dwoma sensorami w rzedzie

LiquidCrystal_I2C lcd(0x3F, 20, 4, 2, 1, 0, 4, 5, 6, 7, 3, 8, POSITIVE);


unsigned long times[10] = {0};      //linia pierwsza 0-4 linia druga 5-9
int AnalogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};

double v_swing = 0;  // predkosc kija
double delta = 0;   // roznica czasu miedzy linia I i linia II
double delta_t = 0; // roznica czasu miedzy rzedami 2 i 4 linii II
double angle = 0;
double angle_st = 0;

double b; // odleglosc miedzy kijem i czujnikiem z rzedu nr 4

bool wasMeasured = false;
bool isBallInPlace = false;

unsigned long startOfMeasurement = 0;
int numOfMeasurements = 0;

int error;
int lightThreshold; // zalezy od kalibracji

/////////////////////////////////////////////////////////////////////////////////////

void resetTimes()
{
    for (int i = 0; i < 10; i++) {
        times[i] = 0;
        wasMeasured = false;
        numOfMeasurements = 0;
    }
}

bool measureStrike() 
{
    // zliczanie pomiarow: 
    // 1 pomiar w L1, 
    // do predkosci - 1 pomiar w L2
    // do kata - 2 pomiary  w L2

    for (int i = 0; i < 10; i++) {
        if (analogRead(AnalogPins(i)) > lightThreshold) {
            if (!startOfMeasurement) {
                startOfMeasurement = milis();
            }

            if (times[i] == 0) {
                times[i] = micros();
                numOfMeasurements++;
            }
            wasMeasured = true;
        }    
    }

    return wasMeasured;    
}

void showResults()
{
    lcd.begin(20, 4); // Inicjalizacja LCD 2x16
    lcd.backlight();     // zalaczenie podwietlenia
    lcd.setCursor(0, 0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
    lcd.print(v_swing);
    lcd.setCursor(0, 1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
    lcd.print(angle_st);
    lcd.setCursor(0, 2);
    lcd.print(error);    
}

void showWelcomeMessage()
{
    lcd.begin(20, 4); // Inicjalizacja LCD 2x16
    lcd.backlight();     // zalaczenie podwietlenia
    lcd.setCursor(3, 0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
    lcd.print("Projekt Golf");
    delay(500);
    lcd.setCursor(0, 1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
    lcd.print("2k19");
}

//////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    Serial.begin(1000000);

    lightThreshold = analogRead(A0) + 50;        // TODO: funkcja do kalibracji, indywidualny threshold

    showWelcomeMessage();
}

void loop()
{
    if (analogRead(A10) > lightThreshold) {
        isBallInPlace = true;
    }
    else {
        isBallInPlace = false;
    }

    if (isBallInPlace == true) { 
        if (milis() - startOfMeasurement >= 1000) {  // resetuje pomiary co sekunde, zapobiega niedokonczonym uderzeniom
            resetTimes();
        }
        wasMeasured = measureStrike();
    }

    if (isBallInPlace == false && wasMeasured == true)
    {
        wasMeasured = false;
        delta = times[2] - times[7];
        v_swing = DISTANCE_BETWEEN_LINES * 3600 / delta; //zamiana z cm/mikrosekunde na km/h

        if (times[7] > times[9]) {
            delta_t = times[7] - times[9]; //wyliczanie czasu wzdluz czujnikow
            error = 1; // clockwise
        }
        else {
            delta_t = times[9] - times[7];
            error = 0;
        }

        b = delta_t * DISTANCE_BETWEEN_LINES / delta;
        angle = atan2(b, 2 * DISTANCE_BETWEEN_SENSORS)
        angle_st = 180 * angle / PI;

        showResults();  // pokazuje wyniki na wyswietlaczu

        ////////////////////////////////////////////////
        Serial.print("Predkosc: ");
        Serial.print(v_swing);
        Serial.print("\t");

        Serial.print("Kat: ");
        Serial.print(angle_st);
        Serial.print("\n");
        //////////////////////////////////////////////////
    }

    //Serial.print("A0:\t\tA1\t\tA2\t\tA3\t\tA4\t\tA5\t\tA6\t\tA7\t\tA8\t\tA9\t\tAng:\tError\n");
    Serial.print(L1[0]);
    Serial.print("\t");
    Serial.print(L1[1]);
    Serial.print("\t");
    Serial.print(L1[2]);
    Serial.print("\t");
    Serial.print(L1[3]);
    Serial.print("\t");
    Serial.print(L1[4]);
    Serial.print("\t");
    Serial.print(L2[0]);
    Serial.print("\t");
    Serial.print(L2[1]);
    Serial.print("\t");
    Serial.print(L2[2]);
    Serial.print("\t");
    Serial.print(L2[3]);
    Serial.print("\t");
    Serial.print(L2[4]);
    Serial.print("\t");
    Serial.print(angle_st);
    Serial.print("\t");
    Serial.print(error);
    Serial.print("\t");
    Serial.println("");
    /**/
}
