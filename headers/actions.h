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

/**
 * NOTE: Простое перечисление, потому что enum class не поддерживает битовые операции
 *
 */
enum EMovementAction
{
    MOVEMENT_ACTION_FORWARD = 1 << 0,
    MOVEMENT_ACTION_BACK    = 1 << 1,
    MOVEMENT_ACTION_LEFT    = 1 << 2,
    MOVEMENT_ACTION_RIGHT   = 1 << 3,
};

#endif // ACTIONS_H
