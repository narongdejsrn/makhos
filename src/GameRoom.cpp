//
//  GameRoom.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 10/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include <math.h>
#include "../include/GameRoom.hpp"

using namespace makhos;

TTF_Font *debugFont = nullptr;
std::string eatTextDebug;

GameRoom::GameRoom(SDL_Window *window, GameLoop *gameLoop): Scene(window, gameLoop) {
    // Checker
    checker = std::make_shared<Checker>();
    checker->resetBoard();
    
    createPawns();
    createBoard();
    
    this->holdingAPawn = false;
    this->pawnInHand = nullptr;
    
    // Create SpriteSheet clip
    checkerClip[ 0 ].x =   0;
    checkerClip[ 0 ].y =   0;
    checkerClip[ 0 ].w =  32;
    checkerClip[ 0 ].h = 32;
    
    checkerClip[ 1 ].x =  32;
    checkerClip[ 1 ].y =   0;
    checkerClip[ 1 ].w =  32;
    checkerClip[ 1 ].h = 32;
    
    checkerClip[ 2 ].x =   64;
    checkerClip[ 2 ].y =   0;
    checkerClip[ 2 ].w =  32;
    checkerClip[ 2 ].h = 32;
    
    checkerClip[ 3 ].x =  96;
    checkerClip[ 3 ].y =   0;
    checkerClip[ 3 ].w =  32;
    checkerClip[ 3 ].h = 32;
    
    checkerClip[ 4 ].x =   128;
    checkerClip[ 4 ].y =   0;
    checkerClip[ 4 ].w =  32;
    checkerClip[ 4 ].h = 32;
    
    checkerClip[ 5 ].x =  160;
    checkerClip[ 5 ].y =   0;
    checkerClip[ 5 ].w =  32;
    checkerClip[ 5 ].h = 32;
    
    checkerClip[ 6 ].x =   192;
    checkerClip[ 6 ].y =   0;
    checkerClip[ 6 ].w =  32;
    checkerClip[ 6 ].h = 32;
    
    checkerClip[ 7 ].x =  224;
    checkerClip[ 7 ].y =   0;
    checkerClip[ 7 ].w =  32;
    checkerClip[ 7 ].h = 32;
    
    // Blue turn first
    redTurn = false;
    
    // Draw board name
    boardPositionFont = TTF_OpenFont("fonts/GlacialIndifference-Regular.ttf", 15);
    if(boardPositionFont == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError() );
        throw TTF_FONT_ERROR;
    }
    
    // Reset button
    resetButton = new TextButton(this->renderer);
    resetButton->setFont("fonts/GlacialIndifference-Regular.ttf", 20);
    resetButton->backgroundColor = {227, 227, 227, 0};
    resetButton->backgroundHoverColor = {209, 209, 209, 0};
    resetButton->text = "RESET!";
    resetButton->x = 460;
    resetButton->y = 200;
    resetButton->clickEvent = std::bind(&GameRoom::resetPlayerPositions, this);
    
    // Game State button
    gameStateButton = new TextButton(this->renderer);
    gameStateButton->setFont("fonts/GlacialIndifference-Regular.ttf", 20);
    gameStateButton->backgroundColor = {227, 227, 227, 0};
    gameStateButton->backgroundHoverColor = {209, 209, 209, 0};
    gameStateButton->text = "Print game state";
    gameStateButton->x = 460;
    gameStateButton->y = 260;
    gameStateButton->clickEvent = std::bind(&GameRoom::printGameState, this);
    
    // AI
    simpleAI = new SimpleAI();
    lastClicked = SDL_GetTicks();
    
    forceEating = nullptr;
    
    // Debug
    textTexture = new ZTexture(this->renderer);
    debugFont = TTF_OpenFont("fonts/GlacialIndifference-Regular.ttf", 15);
    if(debugFont == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError() );
        throw TTF_FONT_ERROR;
    }
}

void GameRoom::createPawns() {
    checkerPawns = new ZTexture(this->renderer);
    if( !checkerPawns->loadFromFile("assets/checkers.png") ) {
        printf("Failed to load white checker pawn!\n");
        throw ASSET_LOAD_ERROR;
    }
    this->checkerPawns->setRenderSize(46, 46);
    
    
    for(int i = 0; i < 16; i++){
        PawnType type = RED_PAWN;
        if(i >= 8) { // the first 8 pawn is red and the last 8 is blue
            type = BLUE_PAWN;
        }
        PawnObject *pawnObj = new PawnObject(checkerPawns, type);
        pawnObj->attachClickEvent(std::bind(&GameRoom::holdAPawn, this, pawnObj, false));
        this->pawns.push_back(pawnObj);
    }
}


void GameRoom::createBoard() {
    // Load dark texture
    darkGrid = new ZTexture(this->renderer);
    if( !darkGrid->loadFromFile("assets/dark-checker-board.png") ) {
        printf("Failed to load check board!\n");
        throw ASSET_LOAD_ERROR;
    }
    this->darkGrid->setRenderSize(56, 56);
    
    int blackGrid = false;
    int boardPos = 0;
    for(int i = 0; i < 8; i++){
        for(int k = 0; k < 8; k++) {
            if(blackGrid) {
                GridObject *darkGridObj = new GridObject(darkGrid);
                darkGridObj->setPosition(k * 56, i * 56);
                darkGridObj->gridType = BLACK_GRID;
                darkGridObj->attachClickEvent(std::bind(&GameRoom::moveToPosition, this, darkGridObj));
                darkGridObj->position = BoardPosition{i, k};
                this->gameGrids.push_back(darkGridObj);
                boardPos++;
            } else {
                Primitive *rectangle = new Primitive(this->renderer, RECTANGLE, 56, 56, k * 56, i * 56);
                rectangle->setColor(255, 209, 128, 0);
                GridObject *whiteGrid = new GridObject(rectangle);
                whiteGrid->gridType = WHITE_GRID;
                whiteGrid->position = BoardPosition{i, k};
                this->gameGrids.push_back(whiteGrid);
            }
            
            if(k != 7) {
                blackGrid = !blackGrid;
            }
        }
    }
}


void GameRoom::resetPlayerPositions() {
    this->redTurn = false;
    this->gameOver = false;
    checker->resetBoard();
    redPawnLeft = 8;
    bluePawnLeft = 8;
}

void GameRoom::holdAPawn(PawnObject *pawnObj, bool force) {
    if(this->holdingAPawn == true) return; // player shouldn't be able to pick twice
    if(redTurn && pawnObj->pawnType != RED_PAWN) return; // not your turn
    if(!redTurn && pawnObj->pawnType != BLUE_PAWN) return;
    Uint32 currentTime = SDL_GetTicks();
    if(currentTime < lastClicked + 100 && !force) {
        return;
    }
    lastClicked = currentTime;
    
    this->holdingAPawn = true;
    this->pawnInHand = pawnObj;
}

// find position of the object on the board
BoardPosition GameRoom::findPosition(PawnObject *pawnObj){
    for(int i = 0; i < 64; i++) {
        if(this->gameGrids[i]->getX() == (pawnObj->getX() - 5) && this->gameGrids[i]->getY() == (pawnObj->getY() - 5)) {
            return this->gameGrids[i]->position;
        }
    }
    return BoardPosition{-1, -1};
}

bool GameRoom::isFree(TempBoardPosition boardPosition) {
    for(int i = 0; i < 16; i++){
        if(this->pawns[i]->position.col == boardPosition.col && this->pawns[i]->position.row == boardPosition.row) {
            return false;
        }
    }
    return true;
}

PawnObject* GameRoom::locatePawn(TempBoardPosition boardPosition) {
    for(int i = 0; i < 16; i++){
        if(this->pawns[i]->position.col == boardPosition.col && this->pawns[i]->position.row == boardPosition.row) {
            return this->pawns[i];
        }
    }
    return nullptr;
}

PawnObject* GameRoom::locatePawn(BoardPosition boardPosition) {
    for(int i = 0; i < 16; i++){
        if(this->pawns[i]->getPosition().col == boardPosition.col && this->pawns[i]->getPosition().row == boardPosition.row) {
            return this->pawns[i];
        }
    }
    return nullptr;
}

GridObject* GameRoom::locateGridObj(TempBoardPosition boardPosition) {
    for(int i = 0; i < 64; i++){
        if(this->gameGrids[i]->position.col == boardPosition.col && this->gameGrids[i]->position.row == boardPosition.row) {
            return this->gameGrids[i];
        }
    }
    return nullptr;
}

GridObject* GameRoom::locateGridObj(BoardPosition boardPosition) {
    return this->gameGrids[(boardPosition.row * 8) + boardPosition.col];
}

std::vector<BoardPosition> GameRoom::eatingPossibility() {
    std::vector<BoardPosition> eater = checker->checkAllEatingPossibility(redTurn);
    return eater;
}

void GameRoom::moveToPosition(GridObject *gridObject) {
    if(this->holdingAPawn == true) {
        int result = checker->moveTo(pawnInHand->getPosition(), gridObject->getPosition());
        if(result == 1) {
            forceEating = nullptr;
            this->holdingAPawn = false;
            this->pawnInHand = nullptr;
            redTurn = !redTurn;
            if(redTurn) {
                botMove();
            }
        } else if(result == 2) {
            // can eat more, do nothing
            BoardPosition *boardPos = new BoardPosition{gridObject->getPosition().row, gridObject->getPosition().col};
            forceEating = boardPos;
        } else if(forceEating == nullptr) {
            this->holdingAPawn = false;
            this->pawnInHand = nullptr;
        }
    } else {
        this->holdingAPawn = false;
        this->pawnInHand = nullptr;
    }
}

void GameRoom::botMove() {
    if(redTurn) {
        this->updateGameState();
        std::vector<BoardGrid> newGameBoard = simpleAI->findBestPossibleMove(checker->getBoard(), true, 5);
        checker->setGameBoard(newGameBoard);
        redTurn = !redTurn;
    }
}

void GameRoom::drawBoard() {
    int k = 1;
    for(int i = 0; i < 64; i++){
        this->gameGrids[i]->draw(this->renderer);
        if(this->gameGrids[i]->gridType == BLACK_GRID) {
            textTexture->loadFromRenderedText(std::to_string(k), boardPositionFont, {255, 255, 255, 0});
            textTexture->render(this->gameGrids[i]->getX(), this->gameGrids[i]->getY());
            k++;
        }
    }
}

void GameRoom::drawPawns() {
    std::vector<BoardGrid> grids = checker->getBoard();
    
    for(int i = 0; i < 16; i++){
        this->pawns[i]->setPosition(-200, -200);
        this->pawns[i]->dead = true;
    }
    
    int gridNo = 0;
    int redCount = 0;
    int blueCount = 8;
    for(int i = 0; i < 8; i++){
        for(int k = 0; k < 8; k++){
            switch (grids[gridNo]) {
                case RED_PAWN_GRID:
                    this->pawns[redCount]->setPosition(k * 56 + 5, i * 56 + 5);
                    this->pawns[redCount]->isKing = false;
                    this->pawns[redCount]->dead = false;
                    redCount++;
                    break;
                case RED_KING_GRID:
                    this->pawns[redCount]->setPosition(k * 56 + 5, i * 56 + 5);
                    this->pawns[redCount]->isKing = true;
                    this->pawns[redCount]->dead = false;
                    redCount++;
                    break;
                case BLUE_PAWN_GRID:
                    this->pawns[blueCount]->setPosition(k * 56 + 5, i * 56 + 5);
                    this->pawns[blueCount]->isKing = false;
                    this->pawns[blueCount]->dead = false;
                    blueCount++;
                    break;
                case BLUE_KING_GRID:
                    this->pawns[blueCount]->setPosition(k * 56 + 5, i * 56 + 5);
                    this->pawns[blueCount]->isKing = true;
                    this->pawns[blueCount]->dead = false;
                    blueCount++;
                    break;
                default:
                    break;
            }
            gridNo++;
        }
    }
    
    for(int i = 0; i < 8; i++){
        if(this->pawns[i]->dead) continue;
        if(this->pawnInHand == this->pawns[i]) {
            // selected
            if(this->pawns[i]->isKing) {
                this->pawns[i]->draw(&checkerClip[4]);
            } else {
                this->pawns[i]->draw(&checkerClip[6]);
            }
        } else {
            if(this->pawns[i]->isKing) {
                this->pawns[i]->draw(&checkerClip[5]);
            } else {
                this->pawns[i]->draw(&checkerClip[7]);
            }
        }
    }
    
    for(int i = 8; i < 16; i++){
        if(this->pawns[i]->dead) continue;
        if(this->pawnInHand == this->pawns[i]) {
            // selected
            if(this->pawns[i]->isKing) {
                this->pawns[i]->draw(&checkerClip[0]);
            } else {
                this->pawns[i]->draw(&checkerClip[2]);
            }
        } else {
            if(this->pawns[i]->isKing) {
                this->pawns[i]->draw(&checkerClip[1]);
            } else {
                this->pawns[i]->draw(&checkerClip[3]);
            }
        }
    }
}

void GameRoom::checkKings() {
    for(int i = 0; i < 8; i++) {
        if(this->pawns[i]->position.row == 7) {
            this->pawns[i]->isKing = true;
        }
    }
    
    for(int i = 8; i < 16; i++) {
        if(this->pawns[i]->position.row == 0) {
            this->pawns[i]->isKing = true;
        }
    }
}

Blocker GameRoom::getBlocker(PawnObject *pawnObj) {
    std::vector<int> topLeftBlocking;
    std::vector<int> topRightBlocking;
    std::vector<int> bottomLeftBlocking;
    std::vector<int> bottomRightBlocking;
    
    // find pawn in diagonally
    for(int i = 0; i < 64; i++){
        if(abs(pawnObj->position.col - this->gameGrids[i]->position.col) == abs(pawnObj->position.row -  this->gameGrids[i]->position.row)) {
            if((pawnObj->position.col - this->gameGrids[i]->position.col) > 0 && pawnObj->position.row < this->gameGrids[i]->position.row){
                // bottom left
                if(!isFree(TempBoardPosition{this->gameGrids[i]->position.col, this->gameGrids[i]->position.row})){
                    bottomLeftBlocking.push_back(this->gameGrids[i]->position.row);
                }
            } else if((pawnObj->position.col - this->gameGrids[i]->position.col) < 0 && pawnObj->position.row < this->gameGrids[i]->position.row){
                // bottom right
                if(!isFree(TempBoardPosition{this->gameGrids[i]->position.col, this->gameGrids[i]->position.row})){
                    bottomRightBlocking.push_back(this->gameGrids[i]->position.row);
                }
            } else if((pawnObj->position.col - this->gameGrids[i]->position.col) > 0 && pawnObj->position.row > this->gameGrids[i]->position.row){
                // top left
                if(!isFree(TempBoardPosition{this->gameGrids[i]->position.col, this->gameGrids[i]->position.row})){
                    topLeftBlocking.push_back(this->gameGrids[i]->position.row);
                }
            } else if((pawnObj->position.col - this->gameGrids[i]->position.col) < 0 && pawnObj->position.row > this->gameGrids[i]->position.row){
                // top left
                if(!isFree(TempBoardPosition{this->gameGrids[i]->position.col, this->gameGrids[i]->position.row})){
                    topRightBlocking.push_back(this->gameGrids[i]->position.row);
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

void GameRoom::setActionableGrid() {
    // reset actionable
    for(int i = 0; i < 64; i++) {
        this->gameGrids[i]->actionable = false;
    }
    
    std::vector<BoardPosition> eatables = checker->checkAllEatingPossibility(redTurn);
    if(pawnInHand == nullptr && eatables.size() > 0) {
        for(auto &eatable: eatables) {
            locateGridObj(eatable)->actionable = true;
        }
        return;
    }
        
        
    if(this->pawnInHand == nullptr) return;
    // draw actionable
    std::vector<BoardPosition> actionables = checker->movingPossibility(this->pawnInHand->getPosition());
    for(auto &action: actionables) {
        locateGridObj(action)->actionable = true;
    }
}

void GameRoom::checkGameOver() {
    if(this->bluePawnLeft == 0 || this->redPawnLeft == 0) {
        this->gameOver = true;
    }
}

void GameRoom::updateGameState() {
    int gameI = 0;
    for(int i = 0; i < 8; i++){
        for(int k = 0; k < 8; k++){
            if(isFree(TempBoardPosition{k, i})) {
                if(this->gameGrids[gameI]->gridType == WHITE_GRID){
                    this->gameState[gameI] = 0; // not playable
                } else {
                    this->gameState[gameI] = 1; // blank
                }
            } else {
                PawnObject *pawnObj = locatePawn(TempBoardPosition{this->gameGrids[gameI]->position.col, this->gameGrids[gameI]->position.row});
                if(pawnObj->pawnType == RED_PAWN) {
                    if(!pawnObj->isKing){
                        this->gameState[gameI] = 2; // red pawn
                    } else {
                        this->gameState[gameI] = 4; // red king
                    }
                } else {
                    if(!pawnObj->isKing){
                        this->gameState[gameI] = 3; // blue pawn
                    } else {
                        this->gameState[gameI] = 6; // blue king
                    }
                }
            }
            gameI++;
        }
    }
}

void GameRoom::printGameState(){
    std::cout << "\n---------------";
    for(int i = 0; i < 64; i++){
        if(i % 8 == 0){
            std::cout << '\n';
        }
        std::cout << gameState[i] << ' ';
    }
    std::cout << "\n---------------\n";
}

void GameRoom::draw() {
    SDL_RenderClear( this->renderer );
    
//    if(!this->gameOver) {
        this->checkGameOver();
        std::vector<BoardPosition> eatable = this->eatingPossibility();
        if(eatable.size() > 0){
            eatTextDebug = "EAT: " + std::to_string(((eatable[0].row * 8 + eatable[0].col)/2) + 1);
            textTexture->loadFromRenderedText(eatTextDebug, debugFont, {255, 0, 0, 0});
            textTexture->render(460, 40);
        }
        
        // debug
        if(redTurn) {
            textTexture->loadFromRenderedText("Turn: RED", debugFont, {0, 0, 0, 0});
        } else {
            textTexture->loadFromRenderedText("Turn: BLUE", debugFont, {0, 0, 0, 0});
        }
        textTexture->render(460, 20);
//    }
    
    this->drawBoard();
    this->drawPawns();
    this->updateGameState();
    this->setActionableGrid();
    this->resetButton->draw();
    this->gameStateButton->draw();
    
    if(forceEating != nullptr) {
        this->holdingAPawn = true;
        this->pawnInHand = locatePawn(*forceEating);
    }
    
    PawnCount pc = checker->getPawnLeft();
    
    textTexture->loadFromRenderedText("Red Pawn Left: " + std::to_string(pc.redPawn), debugFont, {0, 0, 0, 0});
    textTexture->render(460, 60);
    
    textTexture->loadFromRenderedText("Blue Pawn Left: " + std::to_string(pc.bluePawn), debugFont, {0, 0, 0, 0});
    textTexture->render(460, 80);
    
    SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderPresent( this->renderer );
}

GameRoom::~GameRoom() {
    darkGrid->free();
    checkerPawns->free();
    textTexture->free();
    
    TTF_CloseFont(boardPositionFont);
    delete pawnInHand;
    
    SDL_DestroyRenderer( this->renderer );
}

