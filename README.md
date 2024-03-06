
# Project Battlebot INF-1B
Project battlebot is a project revolves around a racetrack in which the battlebots must compete for the fastest time and the cleanest lap.



## Line Follower
<details>
<summary>
    Sebastiaan
</summary>

### Using the Robot
To use the robot, place it in the parking space with the color sensor facing the large black square, and the sensor should be just within the parking space.

## Features

The robot has the following functions:
- to be implemented

</details>

## Linemaze
<details>
<summary>
    Heinrich
</summary>

### Using the Robot

To use the robot, place it in the parking space with the color sensor facing the large black square, and the sensor should be just within the parking space.

Then, turn on both switches of the robot. From now on, you can observe for any improvements.
## Features
The robot has the following functions:
- Start procedure: using a distance sensor to detect the previous robot and calibration
- In the start procedure, an object must be picked up with the gripper
- Dynamic calibration of the motors using a rotation counter.
- Line following using light sensors
- Making 90-degree turns to the left and right
- Turns are executed using the rotation sensors
- At the end, the robot must drop its object onto another large black square
</details>

## Physical Maze
<details>
<summary>
    Gilbert
</summary>

### Using the Robot
To use the robot, place it in the parking space with the color sensor facing the large black square, and the sensor should be just within the parking space.

Then, turn on both switches to start the robot. Subsequently, the robot will calibrate itself on the starting line and navigate through the maze.

## Features
The robot has the following functions:
- Start procedure: using a distance sensor to detect the previous robot and calibration
- In the start procedure, an object must be picked up with the gripper
- Line following using light sensors
- Completing the maze using a sonar sensor
- Making 90-degree turns to the left and right
- Turns are executed using the sonar sensor and the position in the maze
- At the end, the robot must drop its object onto another large black square (the finish)
</details>

# Conventions

All conventions written down here are to be followed within your code. All code that does not match the conventions to a certain extend will not be accepted in a pull request.

### camelCase

All variable names should match the lowerCamelCase convention. This ensures a unified approach to creating variable names.

usage:
```java
// Correct usage
int counterThatCounts;

// Incorrect usage
int CounterThatCounts;
int counter_that_counts;
```

### Leave Space Between 'if', 'for' etc. and '()' for Clarity

Leave a space between 'if and '()' so there is better visibility on where the if statement opens and where it closes.

Usage:
```java
// Correct usage
if (true)

// Incorrect usage
if(true)
for(;;)
```

### Variable language
All variables should be named in English.

### Use separate lines for each condition in an 'if' statement
Using a seperate line for each condition in an 'if' statements leaves room for clarity, readability and if needed to expand the if statement.

Usage:
```java
// Correct usage
if (true &&
    true ||
    true)

// Incorrect usage
if (true && true || true)
```
### Comments
Comments in code must not exceed 1 line and must not break the border in [visual studio code](https://code.visualstudio.com/) or any similair IDE. A comment must always contain a single space at the beginning for clarity.

Usage:
```java
// This is the correct usage

// This 
//Is 
//    Incorrect
```

### Brackets
A bracket after an 'if' statement should always be opened on a new line. This ensures that you can clearly see the difference between the conditions in the 'if' statement.

Usage:
```java
// Correct usage
if (true)
{

}

// Incorrect usage
if (true){

}
```

