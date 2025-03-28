# Problem Set 2 for STDISCM
**Link to Github Repository:** [Github](https://github.com/yuuungch/stdiscm-pset2)

## Instructions to Run:
- Open with Visual Studio
- Run in Debug mode, can be with (F5) or without debugger (Ctrl + F5)

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

## Test Cases:
| Test File           | Description                                     | Expected Behavior                                   |
|---------------------|-------------------------------------------------|-----------------------------------------------------|
| all_valid.txt       | All values within valid ranges                  | Program runs normally                               |
| invalid_n.txt       | n > 10 (max dungeons)                           | Error: Invalid number of dungeons                   |
| invalid_t1.txt      | t1 > t2                                         | Error: Invalid time range                           |
| invalid_t2.txt      | t2 > 15 seconds                                 | Error: T2 resets to 15, program runs normally       |
| overflow_tank.txt   | Tank count > unsigned int max                   | Error: Invalid tank count                           |
| overflow_healer.txt | Healer count > unsigned int max                 | Error: Invalid healer count                         |
| overflow_dps.txt    | DPS count > unsigned int max                    | Error: Invalid DPS count                            |
| negative_tank.txt   | Tank count is negative                          | Error: Invalid tank count                           |
| negative_healer.txt | Healer count is negative                        | Error: Invalid healer count                         |
| negative_dps.txt    | DPS count is negative                           | Error: Invalid DPS count                            |
| imbalanced_roles.txt| Imbalanced role counts                          | Program runs normally with leftover player roles    |

