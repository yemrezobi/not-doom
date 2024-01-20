#include <gtest/gtest.h>

#include "EntityManager.hpp"

TEST(EntityManagerTest, IncrementalEntityId) {
    EntityManager entity_manager = EntityManager();
    int entity0 = entity_manager.create_entity();
    EXPECT_EQ(entity0, 0);
    int entity1 = entity_manager.create_entity();
    EXPECT_EQ(entity1, 1);
    int entity2 = entity_manager.create_entity();
    EXPECT_EQ(entity2, 2);
}

TEST(EntityManagerTest, DestroyValidEntity) {
    EntityManager entity_manager;
    int new_entity = entity_manager.create_entity();
    entity_manager.destroy_entity(new_entity);
    SUCCEED();
}
