#define CATCH_CONFIG_MAIN

#include <iostream>

#include "catch_amalgamated.hpp"
#include "reversi.h"

SCENARIO("Get cell content from Board", "[Board]") {
    GIVEN("default new Board") {
        Board board;

        THEN("can't get cell with row outside range") {
            REQUIRE_THROWS(board.get
                (-1, 0));
            REQUIRE_THROWS(board.get(8, 0));
        }

        THEN("can't get cell with column outside range") {
            REQUIRE_THROWS(board.get(0, -1));
            REQUIRE_THROWS(board.get(0, 8));
        }

        THEN("board has 4 disks with two whites and two blacks on the center") {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    auto cell = board.get(i, j);

                    if (i == 3 && j == 3) {
                        REQUIRE(cell == Cell::White);
                    } else if (i == 4 && j == 3) {
                        REQUIRE(cell == Cell::Black);
                    } else if (i == 3 && j == 4) {
                        REQUIRE(cell == Cell::Black);
                    } else if (i == 4 && j == 4) {
                        REQUIRE(cell == Cell::White);
                    } else {
                        REQUIRE(cell == Cell::Empty);
                    }
                }
            }
        }

        THEN("get cell of other positions will return empty") {
            REQUIRE(board.get(2, 2) == Cell::Empty);
            REQUIRE(board.get(2, 3) == Cell::Empty);
            REQUIRE(board.get(2, 5) == Cell::Empty);
            REQUIRE(board.get(4, 6) == Cell::Empty);
            REQUIRE(board.get(5, 7) == Cell::Empty);
            REQUIRE(board.get(3, 7) == Cell::Empty);
            REQUIRE(board.get(1, 6) == Cell::Empty);
            REQUIRE(board.get(3, 1) == Cell::Empty);
        }
    }
}

SCENARIO("Put piece onto new board", "[Board]") {
    GIVEN("new Board") {
        Board board;

        THEN("can't put piece onto row outside range") {
            REQUIRE(board.put(Piece::Black, -1, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 8, 0) == Result::Error);
        }

        THEN("can't put piece onto column outside range") {
            REQUIRE(board.put(Piece::Black, 0, -1) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 8) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, -1) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, 8) == Result::Error);
        }

        THEN("Can't put piece onto corner") {
            REQUIRE(board.put(Piece::White, 0, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, 7) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 7) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 7) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 7) == Result::Error);
        }

        THEN("can't put piece onto occupied cells") {
            REQUIRE(board.put(Piece::White, 3, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 3, 4) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 4) == Result::Error);
        }
    }
}

SCENARIO("Put piece onto board pre-filled with ally neighbors", "[Board]") {
    GIVEN("A board with only single white piece") {
        Board board{
            {
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        THEN("can't put ally piece beside") {
            REQUIRE(board.put(Piece::White, 2, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 2, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 2, 4) == Result::Error);
            REQUIRE(board.put(Piece::White, 3, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 3, 4) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 4) == Result::Error);
        }

        THEN("can't put enemy piece beside") {
            REQUIRE(board.put(Piece::Black, 2, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 2, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 2, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 4) == Result::Error);
        }
    }

    GIVEN("A board with only single black piece") {
        Board board{
            {
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        THEN("can't put ally piece beside") {
            REQUIRE(board.put(Piece::Black, 2, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 2, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 2, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 4) == Result::Error);
        }

        THEN("can't put enemy piece beside") {
            REQUIRE(board.put(Piece::White, 2, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 2, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 2, 4) == Result::Error);
            REQUIRE(board.put(Piece::White, 3, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 3, 4) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 4) == Result::Error);
        }
    }

    GIVEN("A board with black pieces near the corner") {
        Board board{
            {
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        THEN("can't put ally piece on the corner") {
            REQUIRE(board.put(Piece::Black, 0, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 7) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 7) == Result::Error);
        }

        THEN("can't put enemy piece on the corner") {
            REQUIRE(board.put(Piece::White, 0, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, 7) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 7) == Result::Error);
        }
    }

    GIVEN("A board with white pieces near the corner") {
        Board board{
            {
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::White, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::White, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        THEN("can't put ally piece on the corner") {
            REQUIRE(board.put(Piece::White, 0, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, 7) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 7) == Result::Error);
        }

        THEN("can't put enemy piece on the corner") {
            REQUIRE(board.put(Piece::Black, 0, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 7) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 7) == Result::Error);
        }
    }

    GIVEN("A board with black pieces near the edges") {
        Board board{
            {
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Black, Cell::Black, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black, Cell::Black, Cell::Black, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        THEN("can't put piece of same color on the edge") {
            REQUIRE(board.put(Piece::Black, 0, 1) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 2) == Result::Error);
            REQUIRE(board.put(Piece::Black, 0, 3) == Result::Error);
            REQUIRE(board.put(Piece::Black, 1, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 2, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 3, 0) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 4) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 5) == Result::Error);
            REQUIRE(board.put(Piece::Black, 7, 6) == Result::Error);
            REQUIRE(board.put(Piece::Black, 4, 7) == Result::Error);
            REQUIRE(board.put(Piece::Black, 5, 7) == Result::Error);
            REQUIRE(board.put(Piece::Black, 6, 7) == Result::Error);
        }
    }

    GIVEN("A board with white pieces near the edges") {
        Board board{
            {
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::White, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::White, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        THEN("can't put piece of same color on the edge") {
            REQUIRE(board.put(Piece::White, 0, 1) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, 2) == Result::Error);
            REQUIRE(board.put(Piece::White, 0, 3) == Result::Error);
            REQUIRE(board.put(Piece::White, 1, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 2, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 3, 0) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 4) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 5) == Result::Error);
            REQUIRE(board.put(Piece::White, 7, 6) == Result::Error);
            REQUIRE(board.put(Piece::White, 4, 7) == Result::Error);
            REQUIRE(board.put(Piece::White, 5, 7) == Result::Error);
            REQUIRE(board.put(Piece::White, 6, 7) == Result::Error);
        }
    }
}

SCENARIO("Put piece onto board pre-filled with enemy neighbors", "[Board]") {
    GIVEN("A board with rectangle white pieces and black pieces on outer side") {
        Board board{
            {
                {Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::White, Cell::White, Cell::Empty, Cell::White, Cell::White, Cell::Black, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        WHEN("a black piece is put on the middle") {
            auto result = board.put(Piece::Black, 2, 2);

            THEN("result is ok") {
                REQUIRE(result == Result::Ok);
            }

            THEN("piece is put") {
                REQUIRE(board.get(2, 2) == Cell::Black);
            }

            THEN("flip white pieces on the right") {
                REQUIRE(board.get(2, 3) == Cell::Black);
                REQUIRE(board.get(2, 4) == Cell::Black);
            }

            THEN("flip white pieces on the bottom") {
                REQUIRE(board.get(3, 2) == Cell::Black);
                REQUIRE(board.get(4, 2) == Cell::Black);
            }

            THEN("flip white pieces on the bottom right") {
                REQUIRE(board.get(3, 3) == Cell::Black);
                REQUIRE(board.get(4, 4) == Cell::Black);
            }

            THEN("don't flip other white pieces on the top left") {
                REQUIRE(board.get(0, 0) == Cell::White);
                REQUIRE(board.get(1, 1) == Cell::White);
            }

            THEN("don't flip other white pieces on the left") {
                REQUIRE(board.get(2, 0) == Cell::White);
                REQUIRE(board.get(2, 1) == Cell::White);
            }

            THEN("don't flip other white pieces on the bottom left") {
                REQUIRE(board.get(3, 1) == Cell::White);
                REQUIRE(board.get(4, 0) == Cell::White);
            }

            THEN("don't flip other white pieces on the top") {
                REQUIRE(board.get(0, 2) == Cell::White);
                REQUIRE(board.get(1, 2) == Cell::White);
            }

            THEN("don't flip other white pieces on the top right") {
                REQUIRE(board.get(1, 3) == Cell::White);
                REQUIRE(board.get(0, 4) == Cell::White);
            }
        }
    }

    GIVEN("A board with rectangle white pieces and some black pieces on outer side") {
        Board board{
            {
                {Cell::Black, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Black, Cell::White, Cell::White, Cell::Empty, Cell::White, Cell::White, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty},
                {Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
            }
        };

        WHEN("a black piece is put on the middle") {
            auto result = board.put(Piece::Black, 3, 3);

            THEN("result is ok") {
                REQUIRE(result == Result::Ok);
            }

            THEN("piece is put") {
                REQUIRE(board.get(3, 3) == Cell::Black);
            }

            THEN("flip white pieces on top left") {
                REQUIRE(board.get(1, 1) == Cell::Black);
                REQUIRE(board.get(2, 2) == Cell::Black);
            }

            THEN("flip white pieces on top") {
                REQUIRE(board.get(1, 3) == Cell::Black);
                REQUIRE(board.get(2, 3) == Cell::Black);
            }

            THEN("flip white pieces on the left") {
                REQUIRE(board.get(3, 1) == Cell::Black);
                REQUIRE(board.get(3, 2) == Cell::Black);
            }

            THEN("flip white pieces on the bottom left") {
                REQUIRE(board.get(4, 2) == Cell::Black);
                REQUIRE(board.get(5, 1) == Cell::Black);
            }

            THEN("don't flip white pieces on the bottom") {
                REQUIRE(board.get(4, 3) == Cell::White);
                REQUIRE(board.get(5, 3) == Cell::White);
            }

            THEN("don't flip white pieces on the bottom right") {
                REQUIRE(board.get(4, 4) == Cell::White);
                REQUIRE(board.get(5, 5) == Cell::White);
            }

            THEN("don't flip white pieces on the right") {
                REQUIRE(board.get(3, 4) == Cell::White);
                REQUIRE(board.get(3, 5) == Cell::White);
            }

            THEN("don't flip white pieces on the top right") {
                REQUIRE(board.get(2, 4) == Cell::White);
                REQUIRE(board.get(1, 5) == Cell::White);
            }
        }
    }
}

SCENARIO("Calculate score", "[Board]") {
    GIVEN("A board with random pieces") {
        Board board{
            {
                {Cell::Black, Cell::Empty, Cell::Empty, Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Black, Cell::White, Cell::White, Cell::Empty, Cell::White, Cell::White, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::White, Cell::White, Cell::White, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::White, Cell::Empty, Cell::Empty},
                {Cell::Black, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty},
                {Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Empty, Cell::Black},
            }
        };

        THEN("black score is 5") {
            REQUIRE(board.score(Piece::Black) == 5);
        }

        THEN("white score is 16") {
            REQUIRE(board.score(Piece::White) == 16);
        }
    }
}

SCENARIO("Game with board, turns, and players") {
    GIVEN("New game") {
        Game game;

        THEN("has new board") {
            REQUIRE(game.board() == Board{});
        }

        THEN("first turn is black") {
            REQUIRE(game.current_turn() == Piece::Black);
        }

        THEN("move_count is 0") {
            REQUIRE(game.move_count() == 0);
        }

        THEN("game status is continue") {
            REQUIRE(game.status() == GameStatus::Continue);
        }

        WHEN("move is error") {
            auto result = game.next_move(Piece::Black, 4, 4);

            THEN("result is error") {
                REQUIRE(result == MoveStatus::Error);
            }

            THEN("current turn is black") {
                REQUIRE(game.current_turn() == Piece::Black);
            }

            THEN("move_count is 0") {
                REQUIRE(game.move_count() == 0);
            }

            THEN("score of black is 2") {
                REQUIRE(game.board().score(Piece::Black) == 2);
            }

            THEN("score of white is 2") {
                REQUIRE(game.board().score(Piece::White) == 2);
            }

            THEN("game status is continue") {
                REQUIRE(game.status() == GameStatus::Continue);
            }
        }

        WHEN("move out of turn") {
            auto result = game.next_move(Piece::White, 2, 4);

            THEN("result is error") {
                REQUIRE(result == MoveStatus::Error);
            }

            THEN("board has no piece on 2, 4") {
                REQUIRE(game.board().get(2, 3) == Cell::Empty);
            }

            THEN("board has white piece on 3, 3") {
                REQUIRE(game.board().get(3, 3) == Cell::White);
            }

            THEN("current turn is black") {
                REQUIRE(game.current_turn() == Piece::Black);
            }

            THEN("move_count is 0") {
                REQUIRE(game.move_count() == 0);
            }

            THEN("score of black is 2") {
                REQUIRE(game.board().score(Piece::Black) == 2);
            }

            THEN("score of white is 2") {
                REQUIRE(game.board().score(Piece::White) == 2);
            }

            THEN("game status is continue") {
                REQUIRE(game.status() == GameStatus::Continue);
            }
        }

        WHEN("move once") {
            auto result = game.next_move(Piece::Black, 2, 3);

            THEN("result is continue") {
                REQUIRE(result == MoveStatus::Continue);
            }

            THEN("game status is continue") {
                REQUIRE(game.status() == GameStatus::Continue);
            }

            THEN("board has black piece on 2, 3") {
                REQUIRE(game.board().get(2, 3) == Cell::Black);
            }

            THEN("board has black piece on 3, 3") {
                REQUIRE(game.board().get(3, 3) == Cell::Black);
            }

            THEN("current turn is white") {
                REQUIRE(game.current_turn() == Piece::White);
            }

            THEN("move_count is 1") {
                REQUIRE(game.move_count() == 1);
            }

            THEN("score of black is 4") {
                REQUIRE(game.board().score(Piece::Black) == 4);
            }

            THEN("score of white is 1") {
                REQUIRE(game.board().score(Piece::White) == 1);
            }
        }

        WHEN("move twice") {
            game.next_move(Piece::Black, 2, 3);
            auto result = game.next_move(Piece::White, 2, 2);

            THEN("result is continue") {
                REQUIRE(result == MoveStatus::Continue);
            }

            THEN("game status is continue") {
                REQUIRE(game.status() == GameStatus::Continue);
            }

            THEN("board has white piece on 2, 2") {
                REQUIRE(game.board().get(2, 2) == Cell::White);
            }

            THEN("board has white piece on 3, 3") {
                REQUIRE(game.board().get(3, 3) == Cell::White);
            }

            THEN("current turn is black") {
                REQUIRE(game.current_turn() == Piece::Black);
            }

            THEN("move_count is 2") {
                REQUIRE(game.move_count() == 2);
            }

            THEN("score of black is 3") {
                REQUIRE(game.board().score(Piece::Black) == 3);
            }

            THEN("score of white is 3") {
                REQUIRE(game.board().score(Piece::White) == 3);
            }

            THEN("game status is continue") {
                REQUIRE(game.status() == GameStatus::Continue);
            }
        }
    }
}

SCENARIO("Game with skipping turns") {
    GIVEN("Game with black turn before last valid move") {
        Game game{
            Board{
                {
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Empty, Cell::White, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                    {Cell::Empty, Cell::White, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black, Cell::Black},
                }
            }
        };

        REQUIRE(game.current_turn() == Piece::Black);
        REQUIRE(game.status() == GameStatus::Continue);

        WHEN("black moves results in skipping turn") {
            auto result = game.next_move(Piece::Black, 0, 5);

            THEN("current turn back to black") {
                REQUIRE(game.current_turn() == Piece::Black);
            }

            THEN("result is continue with skip") {
                REQUIRE(result == MoveStatus::ContinueWithSkip);
            }

            THEN("game status is continue") {
                REQUIRE(game.status() == GameStatus::Continue);
            }

            THEN("black continues until game over") {
                REQUIRE(game.next_move(Piece::Black, 0, 0) == MoveStatus::ContinueWithSkip);
                REQUIRE(game.status() == GameStatus::Continue);
                REQUIRE(game.next_move(Piece::Black, 1, 0) == MoveStatus::ContinueWithSkip);
                REQUIRE(game.status() == GameStatus::Continue);
                REQUIRE(game.next_move(Piece::Black, 2, 0) == MoveStatus::ContinueWithSkip);
                REQUIRE(game.status() == GameStatus::Continue);
                REQUIRE(game.next_move(Piece::Black, 3, 0) == MoveStatus::ContinueWithSkip);
                REQUIRE(game.status() == GameStatus::Continue);
                REQUIRE(game.next_move(Piece::Black, 4, 0) == MoveStatus::ContinueWithSkip);
                REQUIRE(game.status() == GameStatus::Continue);
                REQUIRE(game.next_move(Piece::Black, 5, 0) == MoveStatus::ContinueWithSkip);
                REQUIRE(game.status() == GameStatus::Continue);
                REQUIRE(game.next_move(Piece::Black, 7, 0) == MoveStatus::GameOver);
                REQUIRE(game.status() == GameStatus::GameOver);
            }
        }
    }
}

SCENARIO("CPU moves legally") {
    GIVEN("new Game") {
        Game game;
        CpuPlayer player1{Piece::Black};
        CpuPlayer player2{Piece::White};

        THEN("CPU moves will never be error and leads to game over") {
            while (game.status() == GameStatus::Continue) {
                Move move;

                if (game.current_turn() == Piece::Black) {
                    move = player1.get_next_move(game);
                } else {
                    move = player2.get_next_move(game);
                }

                auto result = game.next_move(move.piece, move.row, move.column);
                REQUIRE(result != MoveStatus::Error);
            }

            std::cout << "White: " << game.board().score(Piece::White) << std::endl;
            std::cout << "Black: " << game.board().score(Piece::Black) << std::endl;

            REQUIRE(game.status() == GameStatus::GameOver);
        }
    }
}