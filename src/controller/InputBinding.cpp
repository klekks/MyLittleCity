#include <SFML/Window.hpp>

#include "InputBinding.hpp"
#include <functional>


bool InputBinding::operator==(const InputBinding& other) const 
{
    return actionType == other.actionType && button == other.button;
}


size_t std::hash<InputBinding>::operator()(const InputBinding& b) const 
{
    return hash<int>()(static_cast<int>(b.actionType)) ^ hash<int>()(static_cast<int>(b.button));
}
