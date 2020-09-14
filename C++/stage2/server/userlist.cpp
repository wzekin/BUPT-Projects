#include "userlist.h"

QVector<User*>& UserList::get() {
    std::shared_lock<std::shared_mutex> lock(__mutex);
    return this->__users;
}

QJsonArray UserList::getJson() const {
    std::shared_lock<std::shared_mutex> lock(__mutex);
    QJsonArray array;
    for (auto const& it: this->__users){
        array.push_back(it->toJsonObject());
    }
    return array;
}

void UserList::push(User *u) {
    std::unique_lock<std::shared_mutex> lock(__mutex);
    this->__users.push_back(u);
}

void UserList::del(const User *u) {
    std::unique_lock<std::shared_mutex> lock(__mutex);
    for (auto it = this->__users.begin();it != this->__users.end();){
        if ((*it)->uuid() == u->uuid()){
            this->__users.erase(it);
            break;
        }else{
            it++;
        }
    }
}

void UserList::clear() {
    std::unique_lock<std::shared_mutex> lock(__mutex);
    for (auto const& u: this->__users){
        delete u;
    }
    this->__users.clear();
}
