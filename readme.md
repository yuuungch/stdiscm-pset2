# Problem Set 2 for STDISCM
**Link to Github Repository:** [Github](https://github.com/yuuungch/stdiscm-pset2)

## Instructions to Run:
- Open with Visual Studio
- Run in Debug mode, can be with (F5) or without debugger (Ctrl + F5)
- Enter values for tank, healer, and DPS

## Config File Contents:
| Variable |         Description      |
|----------|-------------------------:|
| n        | number of threads        |
| tanks    | number of tanks          |
| healers  | number of healers        |
| dps      | number of DPS's          |
| t1       | time lower bound         |
| t2       | time upper bound         |

## File Directory:
| File             | Description                                            |
|------------------|--------------------------------------------------------|
| config           | config file                                            |
| config_reader    | config file reader                                     |
| dungeon_instance | an instance of the dungeon used in the problem set     |
| party            | an instance of a party with 1 tank, 1 healer, and 3 dps|
| stdiscm-pset2    | main function                                          |
