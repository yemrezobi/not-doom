#ifndef DOOM_HPP
#define DOOM_HPP

#include "Game.hpp"
#include "LoggingManager.hpp"
#include "Vector2.hpp"
#include <memory>

class Doom : public Game {
public:
    Doom();

private:
    int player_id_;
    double player_movement_speed;
    LoggingManager logger_;
    struct TesselationResult {
        const std::unique_ptr<SDL_Vertex[]> vertices;
        const int vertices_length;
        const std::unique_ptr<int[]> indices;
        const int indices_length;
    };

    auto setup() -> void override;
    auto setup_systems() -> void;

    // TODO: move this function somefile more relevant
    // take top left, top right, bot left, bot right points respectively
    auto tesselate(const Vector2d v1, const Vector2d v2, const Vector2d v3, const Vector2d v4, const int height_factor, const int width_factor) const -> const TesselationResult;

    // systems
    auto process_physics() -> void;
    auto process_renders() -> void;
};

#endif // DOOM_HPP
