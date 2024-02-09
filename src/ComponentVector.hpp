#ifndef ICOMPONENTVECTOR_HPP
#define ICOMPONENTVECTOR_HPP

#include <algorithm>
#include <vector>

class IComponentVector {
public:
    virtual ~IComponentVector() = default;
};

template<typename T>
class ComponentVector : public IComponentVector {
public:
    ComponentVector() : component_vector_()
    {
    }

    auto begin() -> const typename std::vector<T>::iterator
    {
        return component_vector_.begin();
    }

    auto end() -> const typename std::vector<T>::iterator
    {
        return component_vector_.end();
    }

    auto find_component(int entity_id) -> T*
    {
        auto result = std::find_if(component_vector_.begin(), component_vector_.end(), [&](const T& component){
            return component.entity_id == entity_id;
        });
        return result == component_vector_.end() ? nullptr : &(*result);
    }

    auto insert_component(const T& component) -> void
    {
        component_vector_.push_back(component);
    }

    // Remove component from component vector while keeping
    // it tightly packed
    auto remove_component(int entity_id) -> void
    {
        for (typename std::vector<T>::size_type i = 0;
             i < component_vector_.size(); i++) {
            if (static_cast<T>(component_vector_[i]).entity_id == entity_id) {
                component_vector_[i] =
                    component_vector_[component_vector_.size() - 1];
                component_vector_.pop_back();
            }
        }
    }

private:
    std::vector<T> component_vector_;
};

#endif // ICOMPONENTVECTOR_HPP
