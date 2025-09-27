#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>

using namespace std;

// angles that cause face rotation x, y, z, respectively
float A, B, C;
const float cubeWidth = 60;
const int width = 160, height = 44; // width and height of the screen in chars
float zBuffer[width * height];
char buffer[width * height];  // decide which char to print for each cell of a grid
int backgroundASCIICode = ' ';

// not sure what these do just yet
int distanceFromCam = 100;
float K1 = 40;
float x, y, z;
float ooz; // one over z for zBuffering
int xp, yp;
int idx;

float calculateX (int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) -
        k * cos(A) * sin (B) * cos(C) + 
        j * cos(A) * sin (C) +
        k * sin(A) * sin(C) +
        i * cos(B) * cos(C);
}
float calculateY (int i, int j, int k) {
    return j * cos(A) * cos(C) +
        k * sin(A) * cos(C) -
        j * sin(A) * sin(B) * sin(C) +
        k * cos(A) * sin(B) * sin(C) -
        i * cos(B) * sin(C);
}
float calculateZ (int i, int j, int k) {
    return k * cos(A) * cos(B) -
        j * sin(A) * cos(B) +
        i * sin(B);
}

void calculateForPoint (float i, float j, float k, int ch) {
    x = calculateX(i, j, k);
    y = calculateY(i, j, k);
    z = calculateZ(i, j, k) + distanceFromCam;

    ooz = 1 / z;
    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);
    idx = xp + yp * width;

    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main () {
    cout << "\x1b[2J"; // Clears the terminal screen with ANC escape code

    while (1) { // keep cube spinning
        // need to clear buffers
        memset(buffer, backgroundASCIICode, width * height); // set to backgroundASCIICode
        memset(zBuffer, 0, width * height * 4); // Set to 0, represents a point at infinity

        // iterate over the points of the cube
        for (float i = -cubeWidth / 2; i < cubeWidth / 2; i += 0.50) {
            for (float j = -cubeWidth / 2; j < cubeWidth / 2; j += 0.50) {
                calculateForPoint(i , j, -cubeWidth / 2, '@');
                calculateForPoint(cubeWidth / 2, j, i, '$');
                calculateForPoint(-cubeWidth / 2, j, -i, '~');
                calculateForPoint(-i, j, cubeWidth / 2, '#');
                calculateForPoint(i, -cubeWidth / 2, -j, ';');
                calculateForPoint(i, cubeWidth / 2, j, '+');    
            }
        }
        cout << "\x1b[H";
        
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;

        usleep(8000 * 2);
    }    
    return 0; 
}