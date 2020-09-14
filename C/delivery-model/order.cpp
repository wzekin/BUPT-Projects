#include "order.h"

Order::Order() : id(-1), time(0) {}

Order::Order(const unsigned id) : id(id), time(0) {}

Order::Order(const unsigned id, const Point restaurant, const Point diner,
             const unsigned time)
    : id(id), restaurant(restaurant), dinner(diner), time(time){}

Order::~Order() {}

bool Order::operator==(const Order& o) const { return id == o.id; }

bool Order::operator<(const Order& o) const { return id < o.id; }

Order Order::operator=(const Order& b) { return b; }
