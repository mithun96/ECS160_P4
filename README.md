### Peter Swanson
### Mithun Vijayasekar

## Project Overview
The final homework requires you to write a small text processing application, and test it. It
includes an adversarial component, in the sense that there is a game in the homework. You will
be working “against” another team. There are two steps in the “game”, where the second step
includes an auxiliary, “warmup” testing exercise, and an “adversarial” bit. You will work in teams
of two.

For Part 1, you will need to turn in your C program by __Sunday the 11th, at midnight.__ 

## Program Overview
Part 1 : (the “Coding Team”) Write a small text processing application, in C. This application
should consist of:

* 1 .c filmaxTweeter.c.
 
  Calculate the top 10 tweeters (by volume of tweets) in a given CSV file of tweets. Specifically, your program should take one command line argument,
the path of the csv file. It should gracefully handle both valid and invalid inputs. 

 For valid inputs
it should produce 10 lines of output:
```<tweeter>: <count of tweets>```
Where these are put in decreasing order from the most frequent to the 10th most frequent
tweeter. In the case of an invalid input the program should output “Invalid Input Format”. A
valid input csv file has columns separated with “,”’s, but you cannot assume the location of the
tweeter column will be fixed to a particular location (like 8 for instance). 

## Input Rules
A valid csv file will have: 

* A header, and the tweeter column will be called “name”.

* You may also assume that the maximum line length of a valid csv file will not be longer than the longest line in the csv file provided for Homework 3.

* The length of the file will not exceed 20,000 lines, and that the maximum number of tweeters will not be larger than the set of tweeters in the cl­tweets­short.csv file. 

* Finally, for this assignment, a valid file will not have additional commas inside the tweet. However, your program should not crash on any input, even invalid ones.

## Environment Rules
__Make sure your program works within the provided Ubuntu afl docker__, because that’s
where it will be fuzz tested by your assigned testing team, and also sanity tested by us.
You will do this in teams of two, and will thoroughly test and review it. 

__You will receive 10 points for correctly passing all test cases on this__ (we will use a few tests on valid input files to sanity check your submission). This grading is non­adversarial. The code should be clean, well­written, well­commented, well­structured with good use of functions, and well­formatted. If
your testing team complains that it is not, we will review it ourselves, and remove up to 5 points for poor coding practices.

For this part of the assignment, __you will submit your C program to a public repository on GitHub.__
In addition to the C program, you should have a README.md file in the top level directory with
both team members names at the top of the file. When you submit to canvas, both team members should submit the same link the GitHub repository with the specific sha of the your
latest version of your program. The link will look something like this:
```https://github.com/<account_name>/<repo_name>/commit/<commit_sha>```
