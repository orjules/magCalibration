#include <stdio.h>
// #include"testData.h"
#include"smallerTestData.h"


// Center will be found by calculating the mean of the pointcloud

float getMeanOf(int magIndex){
    float sum = 0.0;
    for (int i = 0; i < numberMagPoints; i++)
    {
        sum += magData[i][magIndex];
    }
    return sum / numberMagPoints;
}

float xyzParts(float magValue, float center){
    return (magValue - center) * (magValue - center);
}

float gradient(float x0, float y0, float z0, float radius){
    float sum = 0.0;
    for (int i = 0; i < numberMagPoints; i++)
    {
        sum += -4.0  * (xyzParts(magData[i][0], x0) + xyzParts(magData[i][1], y0) + xyzParts(magData[i][2], z0) - (radius * radius)) * radius;
    }
    return sum / numberMagPoints;
}


float fitRadius(float x0, float y0, float z0, int maxIterations, float learningRate){
    float radiusGuess = 249.0;
    for (int i = 0; i < maxIterations; i++)
    {
        float grad = gradient(x0, y0, z0, radiusGuess);
        printf("%i, gradient: %f\n", i, grad);
        radiusGuess -= grad * learningRate;
    }
    return radiusGuess;
}

int main(){
    float centerX = getMeanOf(0);
    float centerY = getMeanOf(1);
    float centerZ = getMeanOf(2);
    float radius = fitRadius(centerX, centerY, centerZ, 20, 0.000003);
    printf("Finished with: x: %f, y: %f, z: %f, r: %f\n", centerX, centerY, centerZ, radius);
    return 0;
}