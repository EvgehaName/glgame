#ifndef ACTIONS_H
#define ACTIONS_H

enum class EInputScreenDirection
{
    Left,
    Right,
    Up,
    Down,
};

enum class EInputAxis
{
    Vertical,
    Horizontal,
};

enum class EMovementAction
{
    kForwardStrafe,
    kBackStrafe,
    kLeftStrafe,
    kRightStrafe
};

#endif // ACTIONS_H
