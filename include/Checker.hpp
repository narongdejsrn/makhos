#pragma once
//
//  Checker.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 18/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef Checker_hpp
#define Checker_hpp

#include <stdio.h>
#include <vector>
#include <memory>

namespace makhos {

enum BoardGrid {
    NONPLAYABLE_GRID = 0,
    PLAYABLE_GRID = 1,
    RED_PAWN_GRID = 2,
    BLUE_PAWN_GRID = 3,
    RED_KING_GRID = 4,
    BLUE_KING_GRID = 5
};

struct PawnCount {
    int redPawn;
    int bluePawn;
};

struct BoardPosition {
    int row;
    int col;
};

struct Blocker {
    int topLeftBlocking;
    int topRightBlocking;
    int bottomLeftBlocking;
    int bottomRightBlocking;
};


class Checker {
public:
    Checker();
    void resetBoard();
    
    int moveTo(BoardPosition moveFrom, BoardPosition moveTo);
    int moveTo(std::vector<BoardGrid> &boardGrids, BoardPosition moveFrom, BoardPosition moveTo);
    
    
    std::vector<BoardGrid> getBoard();
    PawnCount getPawnLeft();
    
    std::vector<BoardPosition> eatingPossibility(BoardPosition fromLocation);
    std::vector<BoardPosition> eatingPossibility(int index);
    
    std::vector<BoardPosition> checkAllEatingPossibility(bool isRed);
    
    std::vector<BoardPosition> movingPossibility(BoardPosition fromLocation);
    
    std::vector<int> getAllPawn(std::vector<BoardGrid> &boardGrids, bool isRed);
    
    bool isGameOver(std::vector<BoardGrid> &boardGrids);
    BoardPosition IndexToBoardPosition(int idx);

    void printBoard(std::vector<BoardGrid> &boardGrids);

    void setGameBoard(std::vector<BoardGrid> newGameBoard);
private:
    bool isRedPawn(BoardPosition boardPosition);
    bool isRedPawn(int boardIndex);
    bool isRedPawn(std::vector<BoardGrid> &boardGrids, int boardIndex);

    bool isBluePawn(BoardPosition boardPosition);
    bool isBluePawn(int boardIndex);
    bool isBluePawn(std::vector<BoardGrid> &boardGrids, int boardIndex);

    std::vector<BoardGrid> gameBoard;
    void printBoard();
    bool isValidBoardPosition(BoardPosition boardPosition);
    void checkKings();
    Blocker getBlocker(BoardPosition boardPosition);
};

}


#endif /* Checker_hpp */
