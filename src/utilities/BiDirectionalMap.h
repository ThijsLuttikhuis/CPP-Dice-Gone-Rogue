//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_BIDIRECTIONALMAP_H
#define DICEGONEROGUE_BIDIRECTIONALMAP_H

#include <map>
#include <vector>
#include <string>

namespace DGR {

template<typename T, typename U>
class BiDirectionalMap {
private:
    std::map<T, U> tToU{};
    std::map<U, T> uToT{};

public:
    BiDirectionalMap() = default;

    explicit BiDirectionalMap(std::vector<std::pair<T, U>> tus) {
        for (auto &pair : tus) {
            insert(pair.first, pair.second);
        }
    }

    explicit BiDirectionalMap(std::pair<T, U> pair) {
        insert(pair.first, pair.second);
    }

    void insert(T t, U u);

    void insert(U u, T t);

    T &at(U u);

    U &at(T t);

};

template<typename T, typename U>
void BiDirectionalMap<T, U>::insert(T t, U u) {
    tToU[t] = u;
    uToT[u] = t;
}

template<typename T, typename U>
void BiDirectionalMap<T, U>::insert(U u, T t) {
    tToU[t] = u;
    uToT[u] = t;
}

template<typename T, typename U>
T &BiDirectionalMap<T, U>::at(U u) {
    return uToT[u];
}

template<typename T, typename U>
U &BiDirectionalMap<T, U>::at(T t) {
    return tToU[t];
}

}

#endif //DICEGONEROGUE_BIDIRECTIONALMAP_H
