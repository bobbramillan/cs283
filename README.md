# CSV Library

## Overview
This is a simple Java library for reading, writing, and managing CSV (Comma-Separated Values) files.  
It supports custom delimiters, optional headers, and basic error handling.

## Features
- Read CSV files into memory
- Write CSV data back to files
- Retrieve rows and columns
- Add new rows dynamically
- Handles different delimiters and optional headers

## How to Use
1. **Run `TestCSV.java`** to automatically create a sample `test.csv` file  
2. `CSVHandler` reads the file and prints row/column data  
3. A new row is added, and the file is written to `output.csv`  

## Requirements
- Java 17 or later
- IntelliJ IDEA or any Java IDE

## Known Issues & Limitations
- **No support for multi-line fields** → If a field contains a newline inside quotes, it won't be parsed correctly  
- **Does not handle escaped quotes (`""`) inside values** → Example: `"Hello, ""World"""` is not parsed properly  
- **Cannot detect incorrect column counts in rows** → If a row has more or fewer columns than expected, it won’t be flagged as an error  
- **No built-in support for large files** → Processing large CSV files may consume significant memory since all data is stored in a `List<List<String>>`  

## Author
- [Bavanan Bramillan]
