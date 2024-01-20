#include <vector>

class EntityManager {
public:
    EntityManager();
    // Allocates and returns a new entity ID
    auto create_entity() -> int;
    // Marks given entity id as unused to be used later
    auto destroy_entity(int entity) -> void;

private:
    std::vector<bool> allocated_entities_;
};
