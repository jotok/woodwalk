#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename K, typename V>
class ResourceHolder {
    public:
        void load(K k, const std::string &filename);

        template <typename Parameter>
        void load(K k, const std::string &filename, const Parameter &secondParam);
        V& get(K k);
        const V& get(K k) const;

    private:
        void insertResource(K k, std::unique_ptr<V> resource);

    private:
        std::map<K, std::unique_ptr<V>> _map;
};

#include "ResourceHolder.inl"
