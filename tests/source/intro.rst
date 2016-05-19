Intro
===============================

This is a comprehensive test suite for the Voxel8 Developer's Kit. Each function of the printer will be tested for functionality by sending all appropriate commands and analysing the results.

Setup
===============================

To run the diagnostic test, connect your computer to the RAMBo board on the printer. When the printer is powered, it should appear in the Device Manager (Windows) or equivalent as a communication port. To run the program, use the following syntax:

**python diagnosticsTest.py COM(X)**

where (X) is the RAMBo's com number.

To access the help menu of the program, pass it the -h command:

**python diagnosticsTest.py -h**

Additional debug information can be added to the output of the program using the -v command.

**python diagnosticsTest.py COM(X) -v**

Operation
=================================

The program will open a serial communication line with the printer, and begin to run a series of diagnostics to test each of the functions of the printer. The output of each test will be output to the command line, and documented in a log file (console.log, in the same folder). The log file will contain the level of information shown with the -v command, by default.

Each test has a pass or fail criteria. If any test fails, it will be marked as a failure and the entire test will fail. The number of failures and overall passing status will be displayed at the end of the test. 