#pragma once

struct Position {
    int column;
    int row;

    bool operator==(const Position& other) const {
        return other.column == this->column && other.row == this->row;
    }
    bool operator!=(const Position& other) const {
        return other.column != this->column || other.row != this->row;
    }
};
