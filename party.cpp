#include "party.h"

Party::Party(int id, int tank, int healer, int dps1, int dps2, int dps3)
    : id(id), tank(tank), healer(healer), dps1(dps1), dps2(dps2), dps3(dps3) {
}

int Party::getId() const {
    return id;
}