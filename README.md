Welcome to our project (Not complete yet) ! This is a command-line spreadsheet program that allows users to manage and interact with a grid of cells. Each cell can hold an integer value, set directly or calculated using formulas. The program supports dynamic updates, recalculations, and an intuitive interface for seamless navigation.  

## Features

1. **Dynamic Cell Management**  
   - Set values directly: `A1=5`  
   - Use formulas: `B1=A1+3`, `C1=SUM(A1:B1)`  
   - Functions: `MIN`, `MAX`, `AVG`, `SUM`, `STDEV`, `SLEEP`  

2. **Navigation**  
   - Scroll through large sheets:  
     - `w`: Up  
     - `a`: Left  
     - `s`: Down  
     - `d`: Right  
   - Displays a 10x10 grid view for efficient interaction.  

3. **Automatic Recalculation**  
   - Recalculates dependent cells dynamically when values change.  
   - Optimized to recalculate only necessary cells for efficiency.  

4. **Error Handling**  
   - Handles invalid commands gracefully, displaying error messages for:  
     - Incorrect program execution: `./sheet blah invalid`  
     - Invalid cell references: `A3=10` in a 2x2 grid  
     - Circular dependencies: `A1=A1+1`  
     - Undefined operations: `A1=5/0`  

5. **Performance Metrics**  
   - Displays the time taken to execute commands.  

## WORK LEFT:
1. Creating the dependency graph for automatic recalculation of dependent cells when values change.
2. Creating the sleep function and linking it with input.
3. Some error handling, like circular loops in dependency graph etc.
4. Preparation of test suite, trying to test all types of different inputs the user can provide.
5. Preparing report and short demo video etc.

