#include <gtest/gtest.h>
// IWYU pragma: no_include <memory> // for allocator
// IWYU pragma: no_include "gtest/gtest.h" // for DeathTest, Message, TestPartResult

#include "ComponentManager.hpp"
#include "TransformComponent.hpp"

TEST(ComponentManagerTest, RegisterComponent)
{
    ComponentManager component_manager;
    component_manager.register_component<TransformComponent>();
    SUCCEED();
}

TEST(ComponentManagerTest, RegisterExistingComponent)
{
    ComponentManager component_manager;
    component_manager.register_component<TransformComponent>();
    EXPECT_DEATH(component_manager.register_component<TransformComponent>(), "already registered");
}

TEST(ComponentManagerTest, GetComponents)
{
    ComponentManager component_manager;
    component_manager.register_component<TransformComponent>();
    component_manager.get_components<TransformComponent>();
    SUCCEED();
}

TEST(ComponentManagerTest, GetNonexistingComponents)
{
    ComponentManager component_manager;
    EXPECT_DEATH(component_manager.get_components<TransformComponent>(), "not registered");
}
