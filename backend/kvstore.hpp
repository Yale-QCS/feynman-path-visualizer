#ifndef KVSTORE_HPP_
#define KVSTORE_HPP_

#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>

class KVStore {
 public:
    void put(const std::string &key, std::vector<unsigned char> &&val) {
        std::lock_guard<std::mutex> guard(mtx);
        kvstore.insert(std::make_pair(key, val));
    }

    std::vector<unsigned char> get(const std::string &key) {
        std::lock_guard<std::mutex> guard(mtx);
        return kvstore.at(key);
    }
 private:
    std::unordered_map<std::string, std::vector<unsigned char>> kvstore;
    std::mutex mtx;
};

#endif
