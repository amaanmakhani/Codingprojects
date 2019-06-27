# Sierra Wireless, Manufactoring Test Engineering co-op
- This file contains sample or test programs created for use in larger projects. 
- Some code contains reg keys or locations specific to the company, to keep these private '!' with nothing else around will replace them. The number of '!' will be explained at the top of the program.
- See projects section for files, and file descriptions.
- All code in this file is non-confidential, and are soley tester programs.
- The code is in C#.
- XAML files are not included as they were part of large projects, and not enough information could be removed without loss of work to show.
## MFGT Test(Manufactoring Test)
The Manufactoring Test Engineering team specializes in the testing of embedded modules to ensure their quality prior to use. My tasks were to devlop applications that are increase the efficiency of this testing process, and reduce the time needed.
## Projects
- ChangeRegValue.cs: takes in a key, a name, and a value to change it in the registry.
- Implemented_protocol.cs: This code checks all the communication protocols needed for server connection. Backstory, this can be done easily with Microsoft.SqlServer.Management.Smo.Wmi,however, there was a repated error that could not be resolved. That lead to this convuluted code.
- UnitConfig.cs: takes in two configuration files and updates their data to be configured for the tests that are about to take place
- Version.cs: Uses Windows Registry to get the driver version of a specific driver.
- WMISettingsControl.cs: takes in properties and sets them to the wanted values passed in through the wmi query.
