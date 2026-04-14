#pragma once
#include <QKeyEvent>
#include <unordered_map>

enum KeyAction
{
    MOVE_FORWARD,
    MOVE_BACK,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ROLL_LEFT,
    ROLL_RIGHT,
    ROTATE_UP,
    ROTATE_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    SCREENSHOT,
    SNAP_POSITION,
    SNAP_ROTATION,
    COUNT
};

// Default key bindings
const std::unordered_map<int, KeyAction> DEFAULT_KEY_BINDINGS = {
    {Qt::Key_W, MOVE_FORWARD},
    {Qt::Key_S, MOVE_BACK},
    {Qt::Key_A, MOVE_LEFT},
    {Qt::Key_D, MOVE_RIGHT},
    {Qt::Key_Space, MOVE_UP},
    {Qt::Key_Shift, MOVE_DOWN},
    {Qt::Key_Q, ROLL_LEFT},
    {Qt::Key_E, ROLL_RIGHT},
    {Qt::Key_I, ROTATE_UP},
    {Qt::Key_K, ROTATE_DOWN},
    {Qt::Key_J, ROTATE_LEFT},
    {Qt::Key_L, ROTATE_RIGHT},
    {Qt::Key_P, SCREENSHOT},
    {Qt::Key_Z, SNAP_ROTATION},
    {Qt::Key_X, SNAP_POSITION}
};

