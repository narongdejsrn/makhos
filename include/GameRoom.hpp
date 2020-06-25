//
//  GameRoom.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 10/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef GameRoom_hpp
#define GameRoom_hpp

#include <stdio.h>
#include <vector>
#include <memory>

#include "Checker.hpp"
#include "Scene.hpp"
#include "ZTexture.hpp"
#include "PawnObject.hpp"
#include "Primitive.hpp"
#include "../src/later.cpp"
#include "GridObject.hpp"
#include "TextButton.hpp"
#include "SimpleAI.hpp"


namespace makhos {

struct EatLocation {
    int eater;
    int moveToCol;
    int moveToRow;
    int kill;
};

class GameRoom: public Scene {
public:
    GameRoom(SDL_Window* window, GameLoop *gameLoop);
    ~GameRoom();
    void draw();
    
private:
    // Textures
    ZTexture *darkGrid;
    ZTexture *checkerPawns;
    ZTexture *textTexture;
    
    SDL_Rect checkerClip[8];
    
    std::shared_ptr<Checker> checker;
    
    // GameObject
    std::vector<PawnObject *> pawns;
    std::vector<GridObject *> gameGrids;
    
    BoardPosition *forceEating;
    
    // Buttons
    TextButton *resetButton;
    TextButton *gameStateButton;
    
    // Fonts
    TTF_Font *boardPositionFont = nullptr;
    
    // Pawns
    int redPawnLeft;
    int bluePawnLeft;
    int gameState[64];
    Uint32 lastClicked;
    
    // AI
    SimpleAI *simpleAI;
    
    bool gameOver;
    bool eatingMove;
    bool holdingAPawn;
    bool redTurn; // we use this to keep track of player turn
    PawnObject *pawnInHand;
    
    void drawBoard();
    void drawPawns();
    void createBoard();
    void resetPlayerPositions();
    void createPawns();
    void holdAPawn(PawnObject *pawnObj, bool force = false);
    void moveToPosition(GridObject *gameObject);
    std::vector<BoardPosition> eatingPossibility();
    BoardPosition findPosition(PawnObject *pawnObject);
    bool isFree(TempBoardPosition boardPosition);
    PawnObject *locatePawn(TempBoardPosition boardPosition);
    PawnObject *locatePawn(BoardPosition boardPosition);
    GridObject *locateGridObj(TempBoardPosition boardPosition);
    GridObject *locateGridObj(BoardPosition boardPosition);
    void checkKings();
    void setActionableGrid();
    Blocker getBlocker(PawnObject *pawnObj);
    void checkGameOver();
    void updateGameState();
    void printGameState();
    void botMove();
};

}

#endif /* GameRoom_hpp */
