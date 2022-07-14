# magCalibration
A repo for my tries and problems of implementing a magnetometer calibration algorithm.

## The Problem

While implementing a sensor fusion filter using an accelerometer, gyroscope and magnetometer, I found the mag to be unreliable. 
This is most likely because of the lack of calibration. So I set out to write an algorithm to do just that.

An uncalibrated mag readings when turning the sensor around all axis give an ellipsoid (soft iron error) with an offset center (hard iron error). But the result of reading the sensor is just a point cloud. To get the parameters of center offset and the three radii, a best fitting ellipsoid has to be found from the point cloud.

## The Solution

One of the most versatile optimization algorithms seems to be gradient descent.
For the ellipsoid 6 parameters are necessary.
- x0 - the x component of the center
- y0 - the y component of the center
- z0 - the z component of the center
- a - the radius in the x direction
- b - the radius in the y direction
- c - the radius in the z direction

For gradient descent an error (or loss) function has to be defined. In this case I started with the general formula for an ellipsoid. Then I changed it to be a difference (or residual), which I squared and took the sum of. Leading to the following:

![](images/LossFunktionEllipsoid.jpeg)

As the name implies the gradient descent algorithm needs the gradient of the loss function. 

![](images/GradientEllipsoid.jpeg)

Then I went about implementing it in c++ so it could run on the arduino device to which the mag sensor was attached.
The code can be found in [src/fittingEllipsoid.cpp](src/fittingEllipsoid.cpp)
To run it use the two commands:

```
gcc fittingEllipsoid.cpp -o ../target/ellipsoidFit

../target/ellipsoidFit
```

## The new Problem

For gradient descent there are two parameters that can be changed in the main function of the program. 
- steps - the number of iterations of the algorithm
- learning rate - a factor for controlling the rate of convergence

When trying different learning rates with the first implementation, none of them converged, as seen in the following plots:

![](images/ellipsoidPlots/1e-08.png)
![](images/ellipsoidPlots/4e-08.png)
![](images/ellipsoidPlots/5e-08.png)

With larger learning rates the values approach infinity even more quickly.
This implies that something is wrong with this first implementation.

## Improvements

Looking over the basic steps I realized, that my first differential was wrong. 
After fixing this I tested the algorithm without very different results.
Just for fun I tried a different set of initial guess values, far away from the actual centre of the ellipsoid.
The Plots were very interesting, because the gradients seemed to overshoot (depending on the learning rate more or less) and then approach zero. 

![](images/ellipsoidPlots/1e-10.png)
![](images/ellipsoidPlots/1e-11.png)
![](images/ellipsoidPlots/1e-12.png)

The resulting values were still far from the actual centre. 