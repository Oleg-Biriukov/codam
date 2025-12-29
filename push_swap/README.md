*This project has been created as part of the 42 curriculum by obirukov.*

## Description
This project is about sorting a list of random numbers in ascending order with **O(n * log n)** complexity. I implemented the **Turk algorithm**, based on greedy logic, rule-based decisions, and optimization through constraints and trial-and-error refinement.

In simple terms, the program receives the contents of **stack A**, then pushes two values to **stack B**. While the size of stack A is greater than 3, it rotates, pushes, and swaps both stacks. Elements from stack A are moved to stack B using the cheapest combination of operations to maintain stack B in descending order.
 ## Instruction
 1. Launching the program:
 - make
 - make clean
 - ./push_swap <content_of_stack_a>
 2. Testing the program:
 - make test
 - then write <how many time you wanna to taste it> <limit of operation> <len of stack A>

 make - compiling files
 make clean - delete objects
 make fclean - delete objects and program itself
 make re - do it together
 ## Resources
 For a basic understanding of how algorithms work, I read some articles from Grokking Algorithms, Second Edition by Aditya Y. Bhargava. I also learned the main concepts from this [article](https://medium.com/@ayogun/push-swap-c1f5d2d41e97).