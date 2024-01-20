#include "EntityManager.hpp"

EntityManager::EntityManager()
{}

auto EntityManager::create_entity() -> int
{
    // return first available entity id
    for (typename std::vector<bool>::size_type i = 0; i < allocated_entities_.size(); i++) {
        if (allocated_entities_[i] == false) {
            [[unlikely]]
            return static_cast<int>(i);
        }
    }

    // no available entity id
    allocated_entities_.push_back(true);
    return allocated_entities_.size() - 1;
}

auto EntityManager::destroy_entity(int entity) -> void
{
    allocated_entities_[entity] = false;
}
