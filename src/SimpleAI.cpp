//
//  SimpleAI.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 15/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

// Based on Minimax and Alpha Beta Pruning

#include "../include/SimpleAI.hpp"
#include <iostream>
#include <vector>

using namespace makhos;

// https://www.youtube.com/watch?v=l-hh51ncgDI

SimpleAI::SimpleAI() {
    this->checker = std::make_shared<Checker>();
}

std::vector<BoardGrid> SimpleAI::findBestPossibleMove(std::vector<BoardGrid> gameState, bool isRed, int depth) {
    checker->setGameBoard(gameState);
    std::vector<std::vector<BoardGrid>> childs = findActionableChild(gameState, isRed);
    int bestMove = -5000;
    int bestMoveIdx = 0;
    int curIdx = 0;

    for(auto &child : childs){
        int eval = minimax(child, !isRed, depth, -5000, 5000
                           ,false);
        std::cout << "EVAL: " << eval << '\n';
        if(eval > bestMove) {
            bestMove = eval;
            bestMoveIdx = curIdx;
        }
        curIdx++;
    }

    std::vector<BoardGrid> newMove = childs[bestMoveIdx];
    checker->printBoard(newMove);

    return newMove;
}

int SimpleAI::minimax(std::vector<BoardGrid> gameState, bool isRed, int depth, int alpha, int beta, bool maximizingPlayer) {
    int maxEval;
    int minEval;
    checker->setGameBoard(gameState);

    if(depth == 0 || checker->isGameOver(gameState)) {
        PawnLeft pawnLeft = evalPosition(gameState);
        if(isRed) {
            return pawnLeft.redLeft - pawnLeft.blueLeft;
        }
        return pawnLeft.blueLeft - pawnLeft.redLeft;
    }

    std::vector<std::vector<BoardGrid>> childs = findActionableChild(gameState, isRed);
    if(maximizingPlayer) {
        maxEval = -5000;
        for(auto &child : childs){
            int eval = minimax(child, false, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if(beta <= alpha){
                break;
            }
        }
        return maxEval;
        // find position
    } else {
        minEval = 5000;
        for(auto &child : childs){
            int eval = minimax(child, true, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if(beta <= alpha){
                break;
            }
        }
        return minEval;
    }
}

std::vector<std::vector<BoardGrid>> SimpleAI::findActionableChild(std::vector<BoardGrid> gameState, bool isRed) {
    std::vector<std::vector<BoardGrid>> gameStates;
    std::vector<int> focusPawns;

    if(isRed) {
        focusPawns = checker->getAllPawn(gameState, isRed);
    } else {
        focusPawns = checker->getAllPawn(gameState, isRed);
    }

    std::vector<BoardPosition> eatable = checker->checkAllEatingPossibility(isRed);
    if(eatable.size() > 0) {
        // Eat is possible
        for(auto &pawn : focusPawns) {
            std::vector<BoardPosition> eatables = checker->eatingPossibility(pawn);
            for(auto &eatable : eatables) {
                std::vector<BoardGrid> newGameState = generateGameState(gameState, pawn, eatable);
                gameStates.push_back(newGameState);
            }
        }
    } else {
        // Eat is not possible move other positions
        for(auto &pawn: focusPawns) {
            std::vector<BoardPosition> moveables = checker->movingPossibility(checker->IndexToBoardPosition(pawn));
            for(auto &move: moveables){
                std::vector<BoardGrid> newGameState = generateGameState(gameState, pawn, move);
                gameStates.push_back(newGameState);
            }
        }
    }

    return gameStates;
}


std::vector<BoardGrid> SimpleAI::generateGameState(std::vector<BoardGrid> gameState, int pawn, BoardPosition moveToPos){
    std::vector<BoardGrid> newGameState(gameState);
    checker->setGameBoard(newGameState);
    int moveResult = checker->moveTo(checker->IndexToBoardPosition(pawn), moveToPos);
    while(moveResult == 2){
        std::vector<BoardPosition> eatables = checker->eatingPossibility(moveToPos);
        moveResult = checker->moveTo(moveToPos, eatables[0]);
        moveToPos = eatables[0];
    }
    newGameState = checker->getBoard();
    return newGameState;
}

PawnLeft SimpleAI::evalPosition(std::vector<BoardGrid> &gameState){
    const int redPawnCount = checker->getAllPawn(gameState, true).size();
    const int bluePawnCount = checker->getAllPawn(gameState, false).size();
    return PawnLeft{redPawnCount, bluePawnCount};
}
