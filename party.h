#ifndef PARTY_H
#define PARTY_H

class Party {
public:
    Party(int id, int tank, int healer, int dps1, int dps2, int dps3);
    int getId() const;

private:
    int id;
    int tank;
    int healer;
    int dps1;
    int dps2;
    int dps3;
};

#endif // PARTY_H