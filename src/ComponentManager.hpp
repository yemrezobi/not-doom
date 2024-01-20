#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#include <cassert>
#include <map>
#include <memory>
#include <typeindex>

#include "ComponentVector.hpp"

class ComponentManager {
public:
    ComponentManager() : type_to_vector_()
    {
    }

    ~ComponentManager()
    {
    }

    template<typename T>
    auto get_components() const -> ComponentVector<T>&
    {
        assert(type_to_vector_.contains(std::type_index(typeid(T)))
            && "Can not get component that is not registered");

        return *static_cast<ComponentVector<T>*>(type_to_vector_.at(std::type_index(typeid(T))).get());
    }

    template<typename T>
    auto register_component() -> void
    {
        assert(!type_to_vector_.contains(std::type_index(typeid(T)))
            && "Can not register a component that was already registered");

        type_to_vector_[std::type_index(typeid(T))] =
            std::make_unique<ComponentVector<T>>();
    }

private:
    std::map<std::type_index, std::unique_ptr<IComponentVector>>
        type_to_vector_;
};

#endif // COMPONENTMANAGER_HPP
