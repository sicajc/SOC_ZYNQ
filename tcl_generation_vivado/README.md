https://www.youtube.com/watch?v=BXhLFecEPc8&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=41&pp=iAQB
# TCL generation
1. Must perform minor modification, assuming you design your system, you must makes sure things work fine
2. Go to file, write the project into tcl file
3. Get a folder, create the system tcl script.

## Recreate block design using tcl
1. Remember to copy the source check it!
2. Then find the tcl file you generated.

## TCL Absolute file directory
1. Since within the tcl, we use absolute path, thus one must convert the file path into relative path after generating the tcl file.
2. Usually we would create an HDL folder and another constraint folder.
3. Remember to copy the design srcs and the wrapper file, put them into the hdl src folder.
4. Add the constraint xdc file into the constraint folder.


## Search for the directory of Src file access & constraints
1. Change all the file directory path for your designs srcs, wrappers and constraints.


## Opening the project
1. Run the tcl script from run the tcl in the vivado.
2. Or change the directory into where the tcl is then sourcing it.
3. Sourcing the tcl to run the whole block design. This is how we regenerate the system.


## Version Mismatch
1. To run the script generated in an older version, how to bypass it?
2. TCL scripts also includes the new version of IP core, thus one must update the versions.
3. Go ahead and search for where the IP error occurs within your tcl.
4. One must do all the adjustment to the IP version within the tcl file.
5. Do not directly rerun. Error occurs. Solving it through 2 method
- Delete the folder of your project.
- Add the -force command in the create project command within the tcl, thus it would recreate the project if it already exists.


## Run the whole design from command prompt
- We can run the system script from the command prompt after moving into the system script directory.
- Vivado has 3 mode, bash mode, gui mode and tcl mode.
- The same things happens, everything would happens a lot faster.
```
    vivado -mode tcl -source sysGEN.tcl
```