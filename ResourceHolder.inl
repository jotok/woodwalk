template <typename K, typename V>
void ResourceHolder<K, V>::load(K k, const std::string &filename) {
    // Create and load resource
    std::unique_ptr<V> resource(new V());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    // If loading successful, insert resource to map
    insertResource(k, std::move(resource));
}

template <typename K, typename V>
template <typename Parameter>
void ResourceHolder<K, V>::load(K k, const std::string &filename, const Parameter &secondParam) {
    // Create and load resource
    std::unique_ptr<V> resource(new V());
	if (!resource->loadFromFile(filename, secondParam))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insertResource(k, std::move(resource));
}

template <typename K, typename V>
V& ResourceHolder<K, V>::get(K k)
{
	auto found = _map.find(k);
	assert(found != _map.end());

	return *found->second;
}

template <typename K, typename V>
const V& ResourceHolder<K, V>::get(K k) const
{
	auto found = _map.find(k);
	assert(found != _map.end());

	return *found->second;
}

template <typename K, typename V>
void ResourceHolder<K, V>::insertResource(K k, std::unique_ptr<V> resource) 
{
	// Insert and check success
	auto inserted = _map.insert(std::make_pair(k, std::move(resource)));
	assert(inserted.second);
}
