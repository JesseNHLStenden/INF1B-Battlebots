# Project Battlebot INF-1B

This is to inform a user of the code convention we use within the project.


## Conventions

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
