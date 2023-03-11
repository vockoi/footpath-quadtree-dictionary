# 🧭 Footpath Quadtree Dictionary

This is a C program which
1. Builds a point-region dictionary out of a given footpath dataset, with each footpath’s starting and ending coordinates as the dictionary keys.
2. Takes range queries - accepts 2D coordinates of a rectangular search region, and outputs all footpaths whose starting or ending coordinates’ fall into this region.

## 📋 Data

The footpath dataset was obtained from the City of Melbourne Open Data website, and is a subset of the Footpath Steepness dataset combined with data from the Small Areas for Census of Land Use and Employment (CLUE) dataset. Complex geometric data has been processed by the COMP20003 teaching team into simpler attributes. 

The dataset used has the following 19 fields:
- **footpath_id**: The row number for this footpath in the original full dataset. (integer)
- **address**: A name describing the location of the footpath. (string)
- **clue_sa**: The CLUE small area the footpath is in. (string)
- **asset_type**: The name of the type of footpath. (string)
- **deltaz**: The change in vertical distance along the footpath. (double)
- **distance**: The length of the footpath (full geometry) in metres. (double)
- **grade1in**: The percentage gradient of the footpath (full geometry). (double)
- **mcc_id**: The id number identifying the footpath. (integer)
- **mccid_int**: A second number identifying the road or intersection the footpath borders. (integer)
- **rlmax**: The highest elevation on the footpath. (double)
- **rlmin**: The lowest elevation on the footpath. (double)
- **segside**: The side of the road which the footpath is on. (string)
- **statusid**: The status of the footpath. (integer)
- **streetid**: The ID of the first street in the Address. (integer)
- **street_group**: The footpath_id of one of the footpaths connected to this footpath without a gap. (integer)
- **start_lat**: The latitude (y) of the starting point representing the line approximation of the footpath. (double)
- **start_lon**: The longitude (x) of the starting point representing the line approximation of the footpath. (double)
- **end_lat**: The latitude (y) of the ending point representing the line approximation of the footpath. (double)
- **end_lon**: The longitude (x) of the starting point representing the line approximation of the footpath. (double)

## 🛠️ Compiling the program

Download repository to obtain and then extract ```.zip``` file
Navigate command line to `program_files` folder
Compile C program by running the Makefile by entering ```make dict4``` into your command line
## ▶️ Running the program
Once the program is compiled, it runs when it takes input in the following format:

```./dict4 4 <.csv data file> <output file> <bottom left longitude> <bottom left latitude> <top right longitude> <top right latitude> < <stdin file> > <stdout file>```

Sample:
```./dict4 4 ../tests/dataset_1.csv output.out 144.969 -37.7975 144.971 -37.7955 < ../tests/test9.s4.in > output.stdout.out```

Arguments:
- **4**: ‘Stage number’ - no functionality, just must be 4 for program to run
- **.csv data file**: File path where footpath data will be read from
- **output file**: Where all matching footpath records will be printed to
- **bottom left longitude** to **top right latitude**: Coordinates of corners of the rectangle which bounds **all** footpath coordinates within
- **stdin file**: Contains range queries you’d like to make so they can be read by the program
- **stdout file**: Contains order of quadrants that had to be traversed to find matches for each query

## 🏫 Credits

The University of Melbourne’s COMP20003 teaching team came up with the project tasks and specifications. I implemented these specifications, doing all the code and Makefile for this project.
