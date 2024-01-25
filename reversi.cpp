#include "reversi.h"

#include <iostream>

struct RelativePosition {
    int row_diff;
    int col_diff;
};


int calculate_valid_moves(const Board &board, Piece current_turn) {
    int valid_move_count = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.get(i, j) == Cell::Empty) {
                Board copy_board = board;

                if (copy_board.put(current_turn, i, j) == Result::Ok) {
                    valid_move_count++;
                }
            }
        }
    }

    return valid_move_count;
}


std::vector<RelativePosition>
get_enemy_positions(const std::vector<std::vector<Cell>> &cells, Cell put_cell, int row, int column) {
    std::vector<RelativePosition> enemy_positions{};

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if ((row + i < 0) || (row + i >= 8) || (column + j < 0) || (column + j >= 8)) {
                continue;
            }

            if ((i == 0) && (j == 0)) {
                continue;
            }

            if ((cells[row + i][column + j] != Cell::Empty) && (cells[row + i][column + j] != put_cell)) {
                enemy_positions.push_back(RelativePosition{.row_diff = i, .col_diff = j});
            }
        }
    }

    return enemy_positions;
}

int flip_cells(
    std::vector<std::vector<Cell>> &cells,
    const std::vector<RelativePosition> &enemy_positions,
    Cell put_cell,
    int row,
    int column
) {
    int flipped_cell_count = 0;

    for (auto &position: enemy_positions) {
        int i = 1;
        while (true) {
            if ((row + (i * position.row_diff) < 0) || (row + (i * position.row_diff) >= 8) ||
                (column + (i * position.col_diff) < 0) || (column + (i * position.col_diff) >= 8)) {
                break;
            }

            if (cells[row + (i * position.row_diff)][column + (i * position.col_diff)] == Cell::Empty) {
                break;
            }

            if (cells[row + (i * position.row_diff)][column + (i * position.col_diff)] == put_cell) {
                for (int j = 1; j < i; j++) {
                    cells[row + (j * position.row_diff)][column + (j * position.col_diff)] = put_cell;
                    flipped_cell_count++;
                }
                break;
            }

            i++;
        }
    }

    return flipped_cell_count;
}


Board::Board() {
    _cells[3][3] = Cell::White;
    _cells[3][4] = Cell::Black;
    _cells[4][3] = Cell::Black;
    _cells[4][4] = Cell::White;
}

Board::Board(std::vector<std::vector<Cell>> cells) : _cells{std::move(cells)} {
    if (_cells.size() != 8) {
        throw std::invalid_argument("cells should have 8 rows");
    }

    for (auto &row: _cells) {
        if (row.size() != 8) {
            throw std::invalid_argument("cells should have 8 columns");
        }
    }
}

Cell Board::get(int row, int column) const {
    if (row < 0 || row >= 8) {
        throw std::out_of_range("row should be between 0 and 7");
    }

    if (column < 0 || column >= 8) {
        throw std::out_of_range("column should be between 0 and 7");
    }

    return _cells[row][column];
}

Result Board::put(Piece piece, int row, int column) {
    if (row < 0 || row >= 8) {
        return Result::Error;
    }

    if (column < 0 || column >= 8) {
        return Result::Error;
    }

    if (_cells[row][column] != Cell::Empty) {
        return Result::Error;
    }

    auto put_cell = Cell::Empty;

    if (piece == Piece::White) {
        put_cell = Cell::White;
    } else if (piece == Piece::Black) {
        put_cell = Cell::Black;
    }

    std::vector<RelativePosition> enemy_positions = get_enemy_positions(_cells, put_cell, row, column);

    if (enemy_positions.empty()) {
        return Result::Error;
    }

    auto flipped_cell_count = flip_cells(_cells, enemy_positions, put_cell, row, column);

    if (flipped_cell_count == 0) {
        return Result::Error;
    }

    _cells[row][column] = put_cell;

    return Result::Ok;
}

int Board::score(Piece piece) const {
    int score = 0;

    Cell cell_to_count = Cell::Empty;

    if (piece == Piece::White) {
        cell_to_count = Cell::White;
    } else if (piece == Piece::Black) {
        cell_to_count = Cell::Black;
    }

    for (auto &row: _cells) {
        for (auto &cell: row) {
            if (cell == cell_to_count) {
                score++;
            }
        }
    }

    return score;
}

bool Board::operator==(const Board &other) {
    return _cells == other._cells;
}

Game::Game() : _board{Board{}} {}

Game::Game(Board board) : _board{std::move(board)} {}

const Board &Game::board() const {
    return _board;
}

GameStatus Game::status() const {
    return _game_status;
}

Piece Game::current_turn() const {
    return _current_turn;
}

int Game::move_count() const {
    return _move_count;
}

MoveStatus Game::next_move(Piece piece, int row, int column) {
    if (_current_turn != piece) {
        return MoveStatus::Error;
    }

    if (_board.put(piece, row, column) == Result::Error) {
        return MoveStatus::Error;
    }

    if (_current_turn == Piece::Black) {
        _current_turn = Piece::White;
    } else {
        _current_turn = Piece::Black;
    }

    _move_count++;

    _game_status = GameStatus::Continue;
    auto move_status = MoveStatus::Continue;

    if (calculate_valid_moves(_board, _current_turn) == 0) {
        if (_current_turn == Piece::Black) {
            _current_turn = Piece::White;
        } else {
            _current_turn = Piece::Black;
        }

        _game_status = GameStatus::Continue;
        move_status = MoveStatus::ContinueWithSkip;

        if (calculate_valid_moves(_board, _current_turn) == 0) {
            _game_status = GameStatus::GameOver;
            move_status = MoveStatus::GameOver;
        }
    }

    return move_status;
}

CpuPlayer::CpuPlayer(Piece piece) : _piece{piece} {}

Piece CpuPlayer::piece() const {
    return _piece;
}

Move CpuPlayer::get_next_move(const Game &game) const {
    auto highestScore = 0;
    auto next_move = Move{};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board().get(i, j) != Cell::Empty) {
                continue;
            }

            Board copy_board = game.board();

            if (copy_board.put(_piece, i, j) != Result::Ok) {
                continue;
            }

            auto score = copy_board.score(_piece);

            if (score > highestScore) {
                highestScore = score;
                next_move = Move{.piece = _piece, .row = i, .column = j};
            }
        }
    }

    return next_move;
}


HumanPlayer::HumanPlayer(Piece piece) : _piece{piece} {}

Piece HumanPlayer::piece() const {
    return _piece;
}

Move HumanPlayer::get_next_move(const Game &game) const {
    auto row{0};
    auto column{0};

    while (true) {
        std::cout << "Enter move (e.g. A5): ";
        std::string input;
        std::cin >> input;

        if (input.length() != 2) {
            std::cout << "Invalid move " << input << std::endl;
            continue;
        }

        if (input[0] < 'A' || input[0] > 'H') {
            std::cout << "Invalid move " << input << std::endl;
            continue;
        }

        if (input[1] < '1' || input[1] > '8') {
            std::cout << "Invalid move " << input << std::endl;
            continue;
        }

        row = input[1] - '1';
        column = input[0] - 'A';

        auto copy_board = game.board();

        if (copy_board.put(_piece, row, column) != Result::Ok) {
            std::cout << "Invalid move " << input << std::endl;
            continue;
        }

        break;
    }

    return Move{.piece = _piece, .row = row, .column = column};
}