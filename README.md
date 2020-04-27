## Navigation-System-Design
Design of a Navigation System Prototype using objected oriented programming. A navigation system consists of several components which are briefly explained below.

### Modules
<<<<<<< HEAD
+ ###### Central System - Navigation System
The core component that integrates all the subcomponents
+ ###### Waypoint
The waypoints between a source and destination
+  ###### Point of Interest
The POIs present between a source and destination
+ ###### Route
Route consists of a cobination of the desired waypoints and pois
+ ###### GPS Sensor
Gets the current position coordinates
+ ###### Databases
The waypoint and poi information is stored in a database.
+ ###### Attribute Extractor
Extracts the required attributes from the specified file.
+ ###### File handlers - CSV and JSON
Reads/Writes data to persistent memory

### Test
- Test cases are added in the respective files
- Code Coverage > 95 % for most of the modules is achieved
=======
###### Central System - Navigation System
###### Waypoint
The waypoints between a source and destination
###### Point of Interest
The POIs present between a source and destination
###### Route
Route consists of a cobination of the desired waypoints and pois
###### GPS Sensor
Gets the current position coordinates
###### Databases
The waypoint and poi information is stored in a database.
###### Attribute Extractor
Extracts the required attributes from the specified file.
###### File handlers - CSV and JSON
Reads/Writes data to persistent memory

### Test
Test cases are added in the respective files
Code Coverage > 95 % for most of the modules is achieved
>>>>>>> 7be13c7... updated readme

### Scripts
Generate the build and make files
`scripts/setup.bash`

### Executable
`./build/bin/nspBin`
