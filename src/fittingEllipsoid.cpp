#include <stdio.h>
#include"testData.h"

struct magEllipsoid{
    float x0;
    float y0;
    float z0;
    float a;
    float b;
    float c;
};

float normalPart(magEllipsoid guess, float mx, float my, float mz){
    float first = (mx - guess.x0) * (mx - guess.x0)/ guess.a * guess.a;
    float second= (my - guess.y0) * (my - guess.y0)/ guess.b * guess.b;
    float third = (mz - guess.z0) * (mz - guess.z0)/ guess.c * guess.c;
    return 2.0 * (first + second + third - 1.0);
}

float specialPartUpper(float mag, float guess, float denominator){
    return 2.0 * (mag - guess) / (denominator * denominator);
}

float specialPartLower(float mag, float guess, float denominator){
    return - 2.0 * (mag - guess) * (mag - guess) / (denominator * denominator * denominator);
}

float errorGradient(magEllipsoid guess, int numberMagValues, float (*function)(float, float, float),
                    float specialMag, float specialGuess, float specialDenominator){
    float errorSum = 0.0;
    for (int i = 0; i < numberMagValues; i++) {
        float normal = normalPart(guess, magData[i][0], magData[i][1], magData[i][2]);
        // normal = normal / numberMagValues;
        normal = normal * function(specialMag, specialGuess, specialDenominator);
        errorSum += normal;
        // printf("%i, errorSum: %f\n", i, errorSum);
    }
    return errorSum;
}

magEllipsoid getEllipsoidValues(int numberMagValues, int steps, float stepSize){
    // Initialize
    magEllipsoid guess = {220.0, 100.0, 60.0, 250.0, 250.0, 250.0};
    // Repeat 2 and 3
    for (int i = 0; i < steps; i++) {
        // Calculate Gradient
        float gradientX0 = errorGradient(guess, numberMagValues, specialPartUpper, magData[i][0], guess.x0, guess.a);
        float gradientY0 = errorGradient(guess, numberMagValues, specialPartUpper, magData[i][1], guess.y0, guess.b);
        float gradientZ0 = errorGradient(guess, numberMagValues, specialPartUpper, magData[i][2], guess.z0, guess.c);
        float gradientA = errorGradient(guess, numberMagValues, specialPartLower, magData[i][0], guess.x0, guess.a);
        float gradientB = errorGradient(guess, numberMagValues, specialPartLower, magData[i][1], guess.y0, guess.b);
        float gradientC = errorGradient(guess, numberMagValues, specialPartLower, magData[i][2], guess.z0, guess.c);
        printf("%i Gradients: x0: %f, y0: %f, z0: %f, a: %f, b: %f, c: %f\n", i, gradientX0, gradientY0, gradientZ0, gradientA, gradientB, gradientC);
        guess.x0 -= gradientX0 * stepSize;
        guess.y0 -= gradientY0 * stepSize;
        guess.z0 -= gradientZ0 * stepSize;
        guess.a -= gradientA * stepSize;
        guess.b -= gradientB * stepSize;
        guess.c -= gradientC * stepSize;
        printf("%i Guess: x0: %f, y0: %f, z0: %f, a: %f, b: %f, c: %f\n", i, guess.x0, guess.y0, guess.z0, guess.a, guess.b, guess.c);
    }
    return guess;
}

int main(){
    magEllipsoid result = getEllipsoidValues(180, 10, 0.00001);
    printf("x0: %f, y0: %f, z0: %f\n", result.x0, result.y0, result.z0);
    printf("a: %f, b: %f, c: %f\n", result.a, result.b, result.c);
    return 0;
}