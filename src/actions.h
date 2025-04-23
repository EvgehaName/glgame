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

struct MovementState
{
    bool m_forward;
    bool m_back;
    bool m_left;
    bool m_right;

    explicit MovementState() : m_forward(false), m_back(false), m_left(false), m_right(false) {}
};

#endif // ACTIONS_H
