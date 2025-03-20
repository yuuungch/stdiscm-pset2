#ifndef PARTY_H
#define PARTY_H

class Party {
public:
    Party(unsigned int id, unsigned int tank, unsigned int healer, unsigned int dps1, unsigned int dps2, unsigned int dps3);
    unsigned int getId() const;

private:
    unsigned int id;
    unsigned int tank;
    unsigned int healer;
    unsigned int dps1;
    unsigned int dps2;
    unsigned int dps3;
};

#endif // PARTY_H