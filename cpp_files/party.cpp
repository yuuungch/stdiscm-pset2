#include "../header_files/party.h"

Party::Party(unsigned int id, 
            unsigned int tank, 
            unsigned int healer, 
            unsigned int dps1, 
            unsigned int dps2, 
            unsigned int dps3)
    : id(id), 
    tank(tank), 
    healer(healer), 
    dps1(dps1), 
    dps2(dps2), 
    dps3(dps3) {
}

unsigned int Party::getId() const {
    return id;
}