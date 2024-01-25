#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H

#include <exception>
#include <vector>


enum class Cell {
    Empty,
    Black,
    White
};


enum class Piece {
    Black,
    White
};


enum class Result {
    Error,
    Ok,
};


enum class GameStatus {
    Continue,
    GameOver,
};


struct Move {
    Piece piece{Piece::Black};
    int row{-1};
    int column{-1};
};


enum class MoveStatus {
    Error,
    Continue,
    ContinueWithSkip,
    GameOver,
};


class Board {
public:
    Board();

    explicit Board(std::vector<std::vector<Cell>> cells);

    [[nodiscard]] int score(Piece piece) const;

    [[nodiscard]] Cell get(int row, int column) const;

    Result put(Piece piece, int row, int column);

    bool operator==(const Board &other);

private:
    std::vector<std::vector<Cell>> _cells = std::vector<std::vector<Cell>>(8, std::vector<Cell>(8, Cell::Empty));
};


class Game {
public:
    Game();

    explicit Game(Board board);

    [[nodiscard]] const Board &board() const;

    [[nodiscard]] GameStatus status() const;

    [[nodiscard]] Piece current_turn() const;

    [[nodiscard]] int move_count() const;

    MoveStatus next_move(Piece piece, int row, int column);

private:
    Board _board;
    Piece _current_turn{Piece::Black};
    int _move_count{0};
    GameStatus _game_status{GameStatus::Continue};
};


class Player {
public:
    virtual ~Player() = default;

    [[nodiscard]] virtual Piece piece() const = 0;

    [[nodiscard]] virtual Move get_next_move(const Game &game) const = 0;
};

class CpuPlayer : public Player {
public:
    explicit CpuPlayer(Piece piece);

    [[nodiscard]] Piece piece() const override;

    [[nodiscard]] Move get_next_move(const Game &game) const override;

private:
    const Piece _piece{Piece::Black};
};

class HumanPlayer : public Player {
public:
    explicit HumanPlayer(Piece piece);

    [[nodiscard]] Piece piece() const override;

    [[nodiscard]] Move get_next_move(const Game &game) const override;

private:
    const Piece _piece{Piece::Black};
};

#endif //REVERSI_REVERSI_H
