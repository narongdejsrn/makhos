//
//  Checker.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 18/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//
#include <iostream>
#include <cmath>
#include <algorithm>

#include "../include/Checker.hpp"

using namespace makhos;

int BoardPositionToIndex(BoardPosition boardPosition) {
    if(boardPosition.row >= 8 || boardPosition.row < 0 || boardPosition.col >= 8 || boardPosition.col < 0) {
        return -1;
    }

    return (boardPosition.row * 8) + boardPosition.col;
}

BoardPosition Checker::IndexToBoardPosition(int idx) {
    return BoardPosition{idx / 8, idx % 8};
}

Checker::Checker() {
    this->resetBoard();
}

/**
* Move the board position
*/
int Checker::moveTo(BoardPosition moveFrom, BoardPosition moveTo) {
    return this->moveTo(gameBoard, moveFrom, moveTo);
}


int Checker::moveTo(std::vector<BoardGrid> &boardGrid, BoardPosition moveFrom, BoardPosition moveTo) {
    if(boardGrid[BoardPositionToIndex(moveFrom)] == NONPLAYABLE_GRID || boardGrid[BoardPositionToIndex(moveFrom)] == PLAYABLE_GRID) {
        return 0;
    }
    
    if(boardGrid[BoardPositionToIndex(moveTo)] != BoardGrid::PLAYABLE_GRID) {
        return 0;
    }
    
    // check eatable moves
    bool isRed = isRedPawn(moveFrom);
    std::vector<BoardPosition> eatable = checkAllEatingPossibility(isRed);
    if(eatable.size() > 0) {
        std::vector<BoardPosition>::iterator it = std::find_if(eatable.begin(), eatable.end(), [moveTo](BoardPosition const& n){
            return n.row == moveTo.row && n.col == moveTo.col;
        });
        
        if(it == eatable.end()) {
            return 0;
        }
    }
    
    int rowDiff = std::abs(moveTo.row - moveFrom.row);
    int eatingMove = 0;
    
    if(rowDiff == 0) return 0;
    
    Blocker blocker = getBlocker(moveFrom);
    
    
    // validate moves
    switch(boardGrid[BoardPositionToIndex(moveFrom)]){
        case NONPLAYABLE_GRID:
            break;
        case PLAYABLE_GRID:
            break;
        case BoardGrid::RED_PAWN_GRID:
            if(rowDiff > 2) return 0; // not possible;
            
            // normal single move
            if(rowDiff == 1 && (moveTo.row - moveFrom.row != 1 || abs(moveFrom.col - moveTo.col) != 1)) {
                return 0;
            } else if(rowDiff == 2) {
                if(moveFrom.col == moveTo.col) {
                    return 0;
                }
                
                int idxDiff = BoardPositionToIndex(moveTo) - BoardPositionToIndex(moveFrom);
                if(!(idxDiff == 14 || idxDiff == 18)) {
                    return 0;
                }
                
                if(!isBluePawn(BoardPositionToIndex(moveTo) - (idxDiff / 2))) {
                    return 0;
                }
                
                eatingMove = BoardPositionToIndex(moveTo) - (idxDiff / 2);
            }
            break;
        case BoardGrid::BLUE_PAWN_GRID:
            if(rowDiff > 2) return 0; // not possible;
            
            // normal single move
            if(rowDiff == 1 && (moveFrom.row - moveTo.row != 1 || abs(moveFrom.col - moveTo.col) != 1)) {
                return 0;
            } else if(rowDiff == 2) {
                if(moveFrom.col == moveTo.col) {
                    return 0;
                }
            
                
                int idxDiff = BoardPositionToIndex(moveFrom) - BoardPositionToIndex(moveTo);
                if(!(idxDiff == 14 || idxDiff == 18)) {
                    return 0;
                }
                
                
                if(!isRedPawn(BoardPositionToIndex(moveTo) + (idxDiff / 2))) {
                    return 0;
                }
                
                eatingMove = BoardPositionToIndex(moveTo) + (idxDiff / 2);
            }
            
            break;
        case RED_KING_GRID:
        case BLUE_KING_GRID:
            eatable = eatingPossibility(moveFrom);
            if(abs(moveTo.col - moveFrom.col) != abs(moveTo.row - moveFrom.row)) {
                return 0;
            }
            
            if(eatable.size() > 0) {
                std::vector<BoardPosition>::iterator it = std::find_if(eatable.begin(), eatable.end(), [moveTo](BoardPosition const& n){
                    return n.row == moveTo.row && n.col == moveTo.col;
                });
                
                if(it != eatable.end()) {
                    if(moveTo.col < moveFrom.col && moveTo.row > moveFrom.row) {
                        // bottom left
                        BoardPosition eatMove = moveTo;
                        eatMove.row -= 1;
                        eatMove.col += 1;
                        eatingMove = BoardPositionToIndex(eatMove);
                    } else if(moveTo.col < moveFrom.col && moveTo.row < moveFrom.row) {
                        // top left
                        BoardPosition eatMove = moveTo;
                        eatMove.row += 1;
                        eatMove.col += 1;
                        eatingMove = BoardPositionToIndex(eatMove);
                    } else if(moveTo.col > moveFrom.col && moveTo.row < moveFrom.row) {
                        // top right
                        BoardPosition eatMove = moveTo;
                        eatMove.row += 1;
                        eatMove.col -= 1;
                        eatingMove = BoardPositionToIndex(eatMove);
                    } else if(moveTo.col > moveFrom.col && moveTo.row > moveFrom.row) {
                        // bottom right
                        BoardPosition eatMove = moveTo;
                        eatMove.row -= 1;
                        eatMove.col -= 1;
                        eatingMove = BoardPositionToIndex(eatMove);
                    }
                }
            } else {
                if((moveFrom.col - moveTo.col > 0) && moveFrom.row < moveTo.row) {
                    if(moveTo.row > blocker.bottomLeftBlocking) {
                        return 0;
                    }
                } else if ((moveFrom.col - moveTo.col < 0) && moveFrom.row < moveTo.row) {
                    if(moveTo.row > blocker.bottomRightBlocking) {
                        return 0;
                    }
                } else if ((moveFrom.col - moveTo.col > 0) && moveFrom.row > moveTo.row) {
                    if(moveTo.row < blocker.topLeftBlocking) {
                        return 0;
                    }
                } else if ((moveFrom.col - moveTo.col < 0) && moveFrom.row > moveTo.row) {
                    if(moveTo.row < blocker.topRightBlocking) {
                        return 0;
                    }
                }
            }
            
            break;
    }
    
    boardGrid[BoardPositionToIndex(moveTo)] = boardGrid[BoardPositionToIndex(moveFrom)];
    boardGrid[BoardPositionToIndex(moveFrom)] = BoardGrid::PLAYABLE_GRID;
    
    if(eatingMove != 0) {
        boardGrid[eatingMove] = BoardGrid::PLAYABLE_GRID;
        
        // don't switch turn if the player still able to eat from the current possition
        std::vector<BoardPosition> eatables = eatingPossibility(moveTo);
        if(eatables.size() > 0){
            return 2;
        }
    }
    
    checkKings();
    
    return 1;
}

/**
* Reset the board position to default
*/
void Checker::resetBoard() {
    bool playableGrid = false;
    
    this->gameBoard.clear();
    
    for(int i = 0; i < 2; i++){
        for(int k = 0; k < 8; k++){
            if(playableGrid) {
                gameBoard.push_back(BoardGrid::RED_PAWN_GRID);
            } else {
                gameBoard.push_back(BoardGrid::NONPLAYABLE_GRID);
            }
            playableGrid = !playableGrid;
        }
        playableGrid = !playableGrid;
    }
    
    for(int i = 2; i < 6; i++){
        for(int k = 0; k < 8; k++){
            if(playableGrid) {
                gameBoard.push_back(BoardGrid::PLAYABLE_GRID);
            } else {
                gameBoard.push_back(BoardGrid::NONPLAYABLE_GRID);
            }
            playableGrid = !playableGrid;
        }
        playableGrid = !playableGrid;
    }
    
    for(int i = 6; i < 8; i++){
        for(int k = 0; k < 8; k++){
            if(playableGrid) {
                gameBoard.push_back(BoardGrid::BLUE_PAWN_GRID);
            } else {
                gameBoard.push_back(BoardGrid::NONPLAYABLE_GRID);
            }
            playableGrid = !playableGrid;
        }
        playableGrid = !playableGrid;
    }
    
//    int col = 5;
//    int row = 0;
//    gameBoard[(row * 8) + col] = PLAYABLE_GRID;
//    
//    col = 6;
//    row = 1;
//    gameBoard[(row * 8) + col] = BLUE_PAWN_GRID;
//    
//    col = 7;
//    row = 6;
//    gameBoard[(row * 8) + col] = RED_PAWN_GRID;
//    
//    col = 6;
//    row = 7;
//    gameBoard[(row * 8) + col] = PLAYABLE_GRID;
}

void Checker::printBoard() {
    for(int i = 0; i < 64; i++){
        if(i % 8 == 0) {
            std::cout << '\n';
        }
        std::cout << gameBoard[i] << ' ';
    }
    std::cout << '\n';
}

std::vector<BoardGrid> Checker::getBoard() {
    return this->gameBoard;
}

PawnCount Checker::getPawnLeft() {
    int redPawn = 0;
    int bluePawn = 0;
    for(int i = 0; i < 64; i++){
        if(gameBoard[i] == BLUE_PAWN_GRID || gameBoard[i] == BLUE_KING_GRID) {
            bluePawn++;
        } else if (gameBoard[i] == RED_PAWN_GRID || gameBoard[i] == RED_KING_GRID) {
            redPawn++;
        }
    }
    return PawnCount{redPawn, bluePawn};
}

bool Checker::isRedPawn(BoardPosition boardPosition) {
    if(gameBoard[BoardPositionToIndex(boardPosition)] == RED_PAWN_GRID || gameBoard[BoardPositionToIndex(boardPosition)] == RED_KING_GRID) {
        return true;
    }
    return false;
}

bool Checker::isRedPawn(int boardIndex) {
    return isRedPawn(IndexToBoardPosition(boardIndex));
}

bool Checker::isRedPawn(std::vector<BoardGrid> &boardGrids, int boardIndex) {
    if(boardGrids[boardIndex] == RED_PAWN_GRID || boardGrids[boardIndex] == RED_KING_GRID) {
        return true;
    }
    return false;
}

bool Checker::isBluePawn(BoardPosition boardPosition) {
    if(gameBoard[BoardPositionToIndex(boardPosition)] == BLUE_PAWN_GRID || gameBoard[BoardPositionToIndex(boardPosition)] == BLUE_KING_GRID) {
        return true;
    }
    return false;
}

bool Checker::isBluePawn(int boardIndex) {
    return isBluePawn(IndexToBoardPosition(boardIndex));
}

bool Checker::isBluePawn(std::vector<BoardGrid> &boardGrids, int boardIndex) {
    if(boardGrids[boardIndex] == BLUE_PAWN_GRID || boardGrids[boardIndex] == BLUE_KING_GRID) {
        return true;
    }
    return false;
}


std::vector<BoardPosition> Checker::checkAllEatingPossibility(bool isRed) {
    std::vector<BoardPosition> needToCheck;
    
    for(int i = 0; i < 64; i++){
        BoardPosition boardPos = IndexToBoardPosition(i);
        if(isRed) {
            if(isRedPawn(boardPos)) {
                needToCheck.push_back(boardPos);
            }
        } else {
            if(isBluePawn(boardPos)) {
                needToCheck.push_back(boardPos);
            }
        }
    }
    
    std::vector<BoardPosition> eatables;
    
    for(auto &nc: needToCheck) {
        std::vector<BoardPosition> eatingPos = eatingPossibility(nc);
        eatables.insert(eatables.end(), eatingPos.begin(), eatingPos.end());
    }
    
    return eatables;
}

bool Checker::isValidBoardPosition(BoardPosition boardPosition) {
    if(boardPosition.col >= 8 || boardPosition.col < 0 || boardPosition.row >= 8 || boardPosition.row < 0) {
        return false;
    }
    
    return true;
}

std::vector<BoardPosition> Checker::movingPossibility(BoardPosition fromLocation) {
    std::vector<BoardPosition> results;
    for(int i = 0; i < 64; i++) {
        std::vector<BoardGrid> tempBoard(gameBoard);
        if(moveTo(tempBoard, fromLocation, IndexToBoardPosition(i)) != 0){
            results.push_back(IndexToBoardPosition(i));
        }
    }
    return results;
}

void Checker::checkKings() {
    for(int i = 0; i < 8; i++){
        if(gameBoard[i] == BLUE_PAWN_GRID) {
            gameBoard[i] = BLUE_KING_GRID;
        }
    }
    
    for(int i = 56; i < 64; i++){
        if(gameBoard[i] == RED_PAWN_GRID) {
            gameBoard[i] = RED_KING_GRID;
        }
    }
}

std::vector<BoardPosition> Checker::eatingPossibility(int index){
    return eatingPossibility(BoardPosition{index / 8, index % 8});
}

std::vector<BoardPosition> Checker::eatingPossibility(BoardPosition fromLocation){
    std::vector<BoardPosition> results;
    
    Blocker blockers = getBlocker(fromLocation);
    
    switch(gameBoard[BoardPositionToIndex(fromLocation)]) {
        case RED_PAWN_GRID:
            // red pawn
            // there's a blue pawn on the left and row + 1 is available
            if(isBluePawn(BoardPosition{fromLocation.row + 1, fromLocation.col - 1}) && gameBoard[BoardPositionToIndex(BoardPosition{fromLocation.row + 2, fromLocation.col - 2})] == PLAYABLE_GRID) {
                if(isValidBoardPosition(BoardPosition{fromLocation.row + 2, fromLocation.col - 2})) {
                    results.push_back(BoardPosition{fromLocation.row + 2, fromLocation.col - 2});
                }
            }
            
            // there's a blue pawn on the right and row + 1 is available
            if(isBluePawn(BoardPosition{fromLocation.row + 1, fromLocation.col + 1}) && gameBoard[BoardPositionToIndex(BoardPosition{fromLocation.row + 2, fromLocation.col + 2})] == PLAYABLE_GRID) {
                if(isValidBoardPosition(BoardPosition{fromLocation.row + 2, fromLocation.col + 2})) {
                    results.push_back(BoardPosition{fromLocation.row + 2, fromLocation.col + 2});
                }
            }
            break;
        case BLUE_PAWN_GRID:
            if(isRedPawn(BoardPosition{fromLocation.row - 1, fromLocation.col - 1}) && gameBoard[BoardPositionToIndex(BoardPosition{fromLocation.row - 2, fromLocation.col - 2})] == PLAYABLE_GRID) {
                if(isValidBoardPosition(BoardPosition{fromLocation.row - 2, fromLocation.col - 2})) {
                    results.push_back(BoardPosition{fromLocation.row - 2, fromLocation.col - 2});
                }
            }
            
            // there's a blue pawn on the right and row + 1 is available
            if(isRedPawn(BoardPosition{fromLocation.row - 1, fromLocation.col + 1}) && gameBoard[BoardPositionToIndex(BoardPosition{fromLocation.row - 2, fromLocation.col + 2})] == PLAYABLE_GRID) {
                if(isValidBoardPosition(BoardPosition{fromLocation.row - 2, fromLocation.col + 2})) {
                    results.push_back(BoardPosition{fromLocation.row - 2, fromLocation.col + 2});
                }
            }
            break;
        case BLUE_KING_GRID: {
            // bottom left
            int i = abs(blockers.bottomLeftBlocking - fromLocation.row);
            if(isRedPawn(BoardPosition{blockers.bottomLeftBlocking, fromLocation.col - i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.bottomLeftBlocking + 1, fromLocation.col - i - 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.bottomLeftBlocking + 1, fromLocation.col - i - 1})) {
                    results.push_back(BoardPosition{blockers.bottomLeftBlocking + 1,fromLocation.col - i - 1});
                }
                
            }
            
            // top left
            i = abs(blockers.topLeftBlocking - fromLocation.row);
            if(isRedPawn(BoardPosition{blockers.topLeftBlocking, fromLocation.col - i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.topLeftBlocking - 1, fromLocation.col - i - 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.topLeftBlocking - 1, fromLocation.col - i - 1})) {
                    results.push_back(BoardPosition{blockers.topLeftBlocking - 1, fromLocation.col - i - 1});
                }
        
            }
            
            // top right
            i = abs(blockers.topRightBlocking - fromLocation.row);
            if(isRedPawn(BoardPosition{blockers.topRightBlocking, fromLocation.col + i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.topRightBlocking - 1,  fromLocation.col + i + 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.topRightBlocking - 1, fromLocation.col + i + 1})) {
                    results.push_back(BoardPosition{blockers.topRightBlocking - 1, fromLocation.col + i + 1});
                }
                
            }
            
            // bottom right
            i = abs(blockers.bottomRightBlocking - fromLocation.row);
            if(isRedPawn(BoardPosition{blockers.bottomRightBlocking, fromLocation.col + i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.bottomRightBlocking + 1, fromLocation.col + i + 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.bottomRightBlocking + 1, fromLocation.col + i + 1})) {
                    results.push_back(BoardPosition{blockers.bottomRightBlocking + 1, fromLocation.col + i + 1});
                }
            }
            
            break;
        }
        case RED_KING_GRID: {
            // bottom left
            int i = abs(blockers.bottomLeftBlocking - fromLocation.row);
            if(isBluePawn(BoardPosition{blockers.bottomLeftBlocking, fromLocation.col - i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.bottomLeftBlocking + 1, fromLocation.col - i - 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.bottomLeftBlocking + 1, fromLocation.col - i - 1})) {
                    results.push_back(BoardPosition{blockers.bottomLeftBlocking + 1,fromLocation.col - i - 1});
                }
                
            }
            
            // top left
            i = abs(blockers.topLeftBlocking - fromLocation.row);
            if(isBluePawn(BoardPosition{blockers.topLeftBlocking, fromLocation.col - i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.topLeftBlocking - 1, fromLocation.col - i - 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.topLeftBlocking - 1, fromLocation.col - i - 1})) {
                    results.push_back(BoardPosition{blockers.topLeftBlocking - 1, fromLocation.col - i - 1});
                }
        
            }
            
            // top right
            i = abs(blockers.topRightBlocking - fromLocation.row);
            if(isBluePawn(BoardPosition{blockers.topRightBlocking, fromLocation.col + i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.topRightBlocking - 1,  fromLocation.col + i + 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.topRightBlocking - 1, fromLocation.col + i + 1})) {
                    results.push_back(BoardPosition{blockers.topRightBlocking - 1, fromLocation.col + i + 1});
                }
                
            }
            
            // bottom right
            i = abs(blockers.bottomRightBlocking - fromLocation.row);
            if(isBluePawn(BoardPosition{blockers.bottomRightBlocking, fromLocation.col + i}) &&
               gameBoard[BoardPositionToIndex(BoardPosition{blockers.bottomRightBlocking + 1, fromLocation.col + i + 1})] == PLAYABLE_GRID) {
                
                if(isValidBoardPosition(BoardPosition{blockers.bottomRightBlocking + 1, fromLocation.col + i + 1})) {
                    results.push_back(BoardPosition{blockers.bottomRightBlocking + 1, fromLocation.col + i + 1});
                }
            }
            
            break;
        }
        default:
            break;
    }
    
    return results;
}

Blocker Checker::getBlocker(BoardPosition fromLocation) {
    std::vector<int> topLeftBlocking;
    std::vector<int> topRightBlocking;
    std::vector<int> bottomLeftBlocking;
    std::vector<int> bottomRightBlocking;
    
    // find pawn in diagonally
    for(int i = 0; i < 64; i++){
        if(abs(fromLocation.col - IndexToBoardPosition(i).col) == abs(fromLocation.row -  IndexToBoardPosition(i).row)) {
            if((fromLocation.col - IndexToBoardPosition(i).col) > 0 && fromLocation.row < IndexToBoardPosition(i).row){
                // bottom left
                if(gameBoard[i] != PLAYABLE_GRID){
                    bottomLeftBlocking.push_back(IndexToBoardPosition(i).row);
                }
            } else if((fromLocation.col - IndexToBoardPosition(i).col) < 0 && fromLocation.row < IndexToBoardPosition(i).row){
                // bottom right
                if(gameBoard[i] != PLAYABLE_GRID){
                    bottomRightBlocking.push_back(IndexToBoardPosition(i).row);
                }
            } else if((fromLocation.col - IndexToBoardPosition(i).col) > 0 && fromLocation.row > IndexToBoardPosition(i).row){
                // top left
                if(gameBoard[i] != PLAYABLE_GRID){
                    topLeftBlocking.push_back(IndexToBoardPosition(i).row);
                }
            } else if((fromLocation.col - IndexToBoardPosition(i).col) < 0 && fromLocation.row > IndexToBoardPosition(i).row){
                // top left
                if(gameBoard[i] != PLAYABLE_GRID){
                    topRightBlocking.push_back(IndexToBoardPosition(i).row);
                }
            }
        }
    }
    
    int maxTopLeftRow = 0;
    int maxTopRightRow = 0;
    int maxBottomLeftRow = 8;
    int maxBottomRightRow = 8;
    
    std::sort(topLeftBlocking.begin(), topLeftBlocking.end(), std::greater <>());
    std::sort(topRightBlocking.begin(), topRightBlocking.end(), std::greater <>());
    std::sort(bottomLeftBlocking.begin(), bottomLeftBlocking.end());
    std::sort(bottomRightBlocking.begin(), bottomRightBlocking.end());
    
    if(topLeftBlocking.size() > 0)
        maxTopLeftRow = topLeftBlocking[0];
    
    if(topRightBlocking.size() > 0)
        maxTopRightRow = topRightBlocking[0];
    
    if(bottomLeftBlocking.size() > 0)
        maxBottomLeftRow = bottomLeftBlocking[0];
    
    if(bottomRightBlocking.size() > 0)
        maxBottomRightRow = bottomRightBlocking[0];
    
    return {maxTopLeftRow, maxTopRightRow, maxBottomLeftRow, maxBottomRightRow};
}

std::vector<int> Checker::getAllPawn(std::vector<BoardGrid> &boardGrids, bool isRed) {
    std::vector<int> result;
    for(int i = 0; i < 64; i++){
        if(isRed && isRedPawn(boardGrids, i)) {
            result.push_back(i);
        } else if(!isRed && isBluePawn(boardGrids, i)) {
            result.push_back(i);
        }
    }
    return result;
}

bool Checker::isGameOver(std::vector<BoardGrid> &boardGrids) {
    std::vector<int> redPawns = getAllPawn(boardGrids, true);
    std::vector<int> bluePawns = getAllPawn(boardGrids, false);
    
    if(redPawns.size() == 0 || bluePawns.size() == 0){
        return true;
    }
    return false;
}

void Checker::setGameBoard(std::vector<BoardGrid> newGameBoard) {
    this->gameBoard = newGameBoard;
}

void Checker::printBoard(std::vector<BoardGrid> &boardGrids) {
    std::cout << "\n---------------";
    for(int i = 0; i < 64; i++){
        if(i % 8 == 0){
            std::cout << '\n';
        }
        std::cout << boardGrids[i] << ' ';
    }
    std::cout << "\n---------------\n";
}
