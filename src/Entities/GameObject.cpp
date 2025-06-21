#include "Entities/GameObject.h"

GameObject::GameObject(float x, float y, float width, float height)
    : position(x, y), size(width, height), color(1.0f, 1.0f, 1.0f) {
}