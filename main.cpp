#include <map>
#include <iostream>
#include "reversi.h"


void print_board(const Board &board) {
    std::cout << "  A B C D E F G H" << std::endl;
    std::cout << " +-+-+-+-+-+-+-+-+" << std::endl;

    for (int i = 0; i < 8; i++) {
        std::cout << i + 1 << "|";

        for (int j = 0; j < 8; j++) {
            if (board.get(i, j) == Cell::Empty) {
                std::cout << " |";
            } else if (board.get(i, j) == Cell::Black) {
                std::cout << "X|";
            } else if (board.get(i, j) == Cell::White) {
                std::cout << "O|";
            }
        }

        std::cout << std::endl;
        std::cout << " +-+-+-+-+-+-+-+-+" << std::endl;
    }
}

int main() {
    //Game of reversi with options of CPU vs Human, and Human vs Human (2 players)

    Game game;

    std::cout << "Welcome to Reversi!" << std::endl;
    std::cout << "Please choose your players:" << std::endl;
    std::cout << "0. CPU vs CPU" << std::endl;
    std::cout << "1. Human vs CPU" << std::endl;
    std::cout << "2. Human vs Human" << std::endl;
    std::cout << "Enter your players choice: ";
    int players_choice;
    std::cin >> players_choice;

    std::map<Piece, std::unique_ptr<Player>> players;

    if (players_choice == 0) {
        players[Piece::Black] = std::make_unique<CpuPlayer>(Piece::Black);
        players[Piece::White] = std::make_unique<CpuPlayer>(Piece::White);
    } else if (players_choice == 1) {
        players[Piece::Black] = std::make_unique<HumanPlayer>(Piece::Black);
        players[Piece::White] = std::make_unique<CpuPlayer>(Piece::White);
    } else if (players_choice == 2) {
        players[Piece::Black] = std::make_unique<HumanPlayer>(Piece::Black);
        players[Piece::White] = std::make_unique<HumanPlayer>(Piece::White);
    }

    print_board(game.board());
    auto black_score = game.board().score(Piece::Black);
    auto white_score = game.board().score(Piece::White);
    std::cout << "Black score: " << black_score << std::endl;
    std::cout << "White score: " << white_score << std::endl;

    while (game.status() == GameStatus::Continue) {
        std::cout << std::endl;
        std::cout << "Move " << game.move_count() + 1 << std::endl;
        std::cout << "Current turn: " << (game.current_turn() == Piece::Black ? "Black" : "White") << std::endl;

        auto move = players[game.current_turn()]->get_next_move(game);
        auto move_status = game.next_move(move.piece, move.row, move.column);

        if (move_status == MoveStatus::Error) {
            std::cout << "Invalid move!" << std::endl;
        }

        print_board(game.board());
        black_score = game.board().score(Piece::Black);
        white_score = game.board().score(Piece::White);
        std::cout << "Black score: " << black_score << std::endl;
        std::cout << "White score: " << white_score << std::endl;
    }


    if (black_score > white_score) {
        std::cout << "Black wins!" << std::endl;
    } else if (white_score > black_score) {
        std::cout << "White wins!" << std::endl;
    } else {
        std::cout << "Draw!" << std::endl;
    }

    return 0;
}
