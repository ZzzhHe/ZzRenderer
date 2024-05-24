#pragma once 

#include <unordered_map>
#include <vector>
#include <string>

template <typename K, typename V>
std::vector<K> getKeys(const std::unordered_map<K, V>& map) {
    std::vector<K> keys;
    keys.reserve(map.size()); // Reserve space to avoid multiple allocations
    for (const auto& pair : map) {
        keys.push_back(pair.first);
    }
    return keys;
}