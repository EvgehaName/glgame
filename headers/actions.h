#ifndef ACTIONS_H
#define ACTIONS_H

enum class EDirection
{
    kNone = -1,
    kLeft,
    kRight,
    kUp,
    kDown,
};

enum class EDirectionType
{
    kNone = -1,
    kVertical,
    kHorizontal
};

enum class EGameAction
{
    kForwardStrafe,
    kBackStrafe,
    kLeftStrafe,
    kRightStrafe
};

#endif // ACTIONS_H
