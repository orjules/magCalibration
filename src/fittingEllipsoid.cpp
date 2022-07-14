#include <stdio.h>
#include"testData.h"
// #include"smallerTestData.h"

struct magEllipsoid{
    float x0;
    float y0;
    float z0;
    float a;
    float b;
    float c;
};

float normalPart(magEllipsoid guess, float mx, float my, float mz){
    float first  = (mx - guess.x0) * (mx - guess.x0) / (guess.a * guess.a);
    float second = (my - guess.y0) * (my - guess.y0) / (guess.b * guess.b);
    float third  = (mz - guess.z0) * (mz - guess.z0) / (guess.c * guess.c);
    return (first + second + third - 1.0);
}

float specialPartUpper(float mag, float guess, float denominator){
    return (mag - guess) / (denominator * denominator);
}

float specialPartLower(float mag, float guess, float denominator){
    return (mag - guess) * (mag - guess) / (denominator * denominator * denominator);
}

float errorGradient(magEllipsoid guess, int numberMagValues, float (*function)(float, float, float),
                    float specialMag, float specialGuess, float specialDenominator){
    float errorSum = 0.0;
    for (int i = 0; i < numberMagValues; i++) {
        float normal = normalPart(guess, magData[i][0], magData[i][1], magData[i][2]);
        errorSum += normal * function(specialMag, specialGuess, specialDenominator);
        // printf("%i, errorSum: %f\n", i, errorSum);
    }
    return -4.0 * errorSum;
}

magEllipsoid getEllipsoidValues(int numberMagValues, int steps, float learningRate){
    // Initialize
    // magEllipsoid guess = {200.0, 100.0, 50.0, 100.0, 100.0, 100.0};
    magEllipsoid guess = {2.0, 1.0, 5.0, 1.0, 1.1, 1.0};
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
    magEllipsoid result = getEllipsoidValues(numberMagPoints, 50, 0.0000000000001);
    // printf("%f, %f, %f, %f, %f, %f\n", result.x0, result.y0, result.z0, result.a, result.b, result.c);
    return 0;
}