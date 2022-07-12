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
        normal = normal * function(specialMag, specialGuess, specialDenominator);
        errorSum += normal * normal;
        // printf("%i, errorSum: %f\n", i, errorSum);
    }
    return errorSum;
}

magEllipsoid getEllipsoidValues(int numberMagValues, int steps, float learningRate){
    // Initialize
    magEllipsoid guess = {220.0, 100.0, 60.0, 250.0, 250.0, 250.0};
    printf("step, x0, y0, z0, a, b, c, learningRate\n");
    // Repeat 2 and 3
    for (int i = 0; i < steps; i++) {
        // Calculate Gradient
        float gradientX0 = errorGradient(guess, numberMagValues, specialPartUpper, magData[i][0], guess.x0, guess.a);
        float gradientY0 = errorGradient(guess, numberMagValues, specialPartUpper, magData[i][1], guess.y0, guess.b);
        float gradientZ0 = errorGradient(guess, numberMagValues, specialPartUpper, magData[i][2], guess.z0, guess.c);
        float gradientA = errorGradient(guess, numberMagValues, specialPartLower, magData[i][0], guess.x0, guess.a);
        float gradientB = errorGradient(guess, numberMagValues, specialPartLower, magData[i][1], guess.y0, guess.b);
        float gradientC = errorGradient(guess, numberMagValues, specialPartLower, magData[i][2], guess.z0, guess.c);
        printf("%i, %f, %f, %f, %f, %f, %f, %1.13f\n", i, gradientX0, gradientY0, gradientZ0, gradientA, gradientB, gradientC, learningRate);
        guess.x0 -= gradientX0 * learningRate;
        guess.y0 -= gradientY0 * learningRate;
        guess.z0 -= gradientZ0 * learningRate;
        guess.a -= gradientA * learningRate;
        guess.b -= gradientB * learningRate;
        guess.c -= gradientC * learningRate;
        // printf("%i, %f, %f, %f, %f, %f, %f, %f\n", i, guess.x0, guess.y0, guess.z0, guess.a, guess.b, guess.c, learningRate);
    }
    return guess;
}

int main(){
    magEllipsoid result = getEllipsoidValues(180, 15, 0.00000004);
    return 0;
}