
# Personal Data Storage system

A simple database system that allows storing, deleting and retrieving data on artists and musical instruments, and can link artists to their instruments in the database.


## Author

Mohammad Owais

IMT2022102

## PDS_TESTER

### Compilation

To compile the non-interactive pds_tester, use the following command:

```bash
make pds_tester
```

### Execution

To run the pds_tester, use the following command:

```bash
./pds_tester testcases.txt 0/1
```

where `testcases.txt` is the name of the file containing the test cases, and `0/1` is a flag that indicates whether to print the status for each test case, or to print the output of each test case.

### Test Cases

The test cases are written in a file using the following format:

where `command` is one of the following commands with format:

- CREATE:
    - ``` CREATE repo_name PASS/FAIL ``` 
    - ``` CREATE repo_name linked_repo_name PASS/FAIL ``` 
- OPEN:
    - ``` OPEN repo_name PASS/FAIL ``` 
    - ``` OPEN repo_name linked_repo_name PASS/FAIL ``` 
- STORE_ARTIST:
    - ``` STORE_ARTIST artist_id artist_name genre PASS/FAIL ``` 
- STORE_INSTRUMENT:
    - ``` STORE_INSTRUMENT artist_id instrument_name PASS/FAIL ``` 
- SEARCH_BY_ID:
    - ``` SEARCH_BY_ID artist_id PASS/FAIL ``` 
- SEARCH_BY_NAME:
    - ``` SEARCH_BY_NAME artist_name index_at_which artist was found PASS/FAIL ``` 
- DELETE_ARTIST:
    - ``` DELETE_ARTIST artist_id PASS/FAIL ``` 
- LINK:
    - ``` LINK artist_id instrument_id PASS/FAIL ``` 
- GET_INSTRUMENTS_OF:
    - ``` GET_INSTRUMENTS_OF artist_id PASS/FAIL ``` 
- CLOSE:
    - ``` CLOSE repo_name PASS/FAIL ``` 

## LINK_TESTER

### Compilation

To compile the interactive link_tester that uses default data to test linking functionality:

```bash
make link_tester
```

### Execution

To run the link_tester, execute:

```bash
./link_tester
```

### Features

The link_tester is simplistic, with 10 artists with IDs from `101 to 110`, and 10 instruments with IDs from `1001 to 1010`, and these artists can be linked to these instruments.
The tester has a menu based system that will allow you to link records, and see which instruments are linked to which artist.

## Compilation

You can compile both the link and pds testers using:

```bash
make all
```

## Cleaning files

To clean the files that do not contain code or testcases:

```bash
make clean
```
