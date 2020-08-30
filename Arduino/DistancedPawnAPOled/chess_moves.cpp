/**
 * @file chess_moves.cpp
 * @brief Class that manages the moves and the board
 */

#include "chess_moves.h"

// ----------------------------------------------------- Square class
Square::Square() {
  piece = EMPTY;
  color = PLAY_NONE;
}

void Square::setSpace(Square* space) {
  color = space->getPieceColor();
  piece = space->getPiece();
}

void Square::setEmpty() {
  color = PLAY_NONE;
  piece = EMPTY;
}

ChessPiece Square::getPiece() {
  return piece;
}

ChessColor Square::getPieceColor() {
  return color;
}

void Square::setPieceAndColor(ChessPiece p, ChessColor c) {
  piece = p;
  color = c;

}

// --------------------------------------------------------------------- Borad class
void Board::drawBoard(int t) {
  switch(t) {
    // Board output to serial console
    case BOARD_SERIAL:
      drawSerialBoard();
    break;
    
    // Board output to remote http client
    case BOARD_HTML:
    
    break;

    // Board output to the controller display
    case BOARD_DISPLAY:
    
    break;
  }
}

void Board::drawSerialBoard() {
  // Add an empty line
  Serial1 << endl << "      = Game Status =" << endl << endl;
  // Loop by row and columns
  for (int i = 7; i >= 0; i--) {
    Serial1 << " " << i + 1 << "| ";
    for (int j = 0; j < 8; j++) {
      // Set the ChessPieces in the starting position and
      // draw the squares
      ChessPiece p = square[j][i].getPiece();
      ChessColor c = square[j][i].getPieceColor();
      switch (p)
      {
      case KING: (c == PLAY_WHITE) ? Serial1 << "[K]" : Serial1 << "[k]";
        break;
      case QUEEN: (c == PLAY_WHITE) ? Serial1 << "]Q]" : Serial1 << "[q]";
        break;
      case BISHOP:(c == PLAY_WHITE) ? Serial1 << "[B]" : Serial1 << "[b]";
        break;
      case KNIGHT:(c == PLAY_WHITE) ? Serial1 << "[H]" : Serial1 << "[h]";
        break;
      case ROOK: (c == PLAY_WHITE) ? Serial1 << "[R]" : Serial1 << "[r]";
        break;
      case PAWN: (c == PLAY_WHITE) ? Serial1 << "[P]" : Serial1 << "[p]";
        break;
      case EMPTY: Serial1 << "[ ]";
        break;
      default: Serial1 << "???"; // Should never happen
        break;
      } // Case piece
    } // print row
    Serial1 << endl;
  } // print col
  // Last row
  Serial1 << "    ________________________" << endl;
  Serial1 << "     A  B  C  D  E  F  G  H" << endl;
}

void Board::drawHtmlBoard() {

  // Not yet implemented

}

void Board::drawDisplayBoard() {

  // Not yet available.

}

bool Board::doMove() {
//  string move;
//  int x1, x2, y1, y2;
//  bool stop = false;
//  while (!stop)
//  {
//    (turn == PLAY_WHITE) ? cout << "White's turn" << endl : cout << "Black's turn" << endl;
//    cout << "Type in your move as a single four character string. Use x-coordinates first in each pair." << endl;
//    cin >> move;
//    x1 = move[0] - 48;
//    y1 = move[1] - 48;
//    x2 = move[2] - 48;
//    y2 = move[3] - 48;
//    if (getSquare(x1, y1)->getPieceColor() == turn)
//    {
//
//
//      if (makeMove(x1, y1, x2, y2) == false)
//      {
//        cout << "Invalid move, try again." << endl;
//      }
//      else
//        stop = true;
//    }
//    else
//      cout << "That's not your ChessPiece. Try again." << endl;
//  }
//  if (getSquare(x2, y2)->getPiece() == KING)
//    if (getSquare(x1, y1)->getPieceColor() == PLAY_WHITE)
//    {
////      cout << "WHITE WINS" << endl;
//      return false;
//    }
//    else
//
//    {
////      cout << "BLACK WINS" << endl;
//      return false;
//    }
//
//
//  if (turn == BLACK)
//    turn = PLAY_WHITE;
//  else
//    turn = PLAY_BLACK;

  return true;

}

void Board::setBoard() {
  // Place the first row pieces in their position for white side
  square[0][0].setPieceAndColor(ROOK, PLAY_WHITE);
  square[1][0].setPieceAndColor(KNIGHT, PLAY_WHITE);
  square[2][0].setPieceAndColor(BISHOP, PLAY_WHITE);
  square[3][0].setPieceAndColor(QUEEN, PLAY_WHITE);
  square[4][0].setPieceAndColor(KING, PLAY_WHITE);
  square[5][0].setPieceAndColor(BISHOP, PLAY_WHITE);
  square[6][0].setPieceAndColor(KNIGHT, PLAY_WHITE);
  square[7][0].setPieceAndColor(ROOK, PLAY_WHITE);

  // Place the first row pieces in their position for black side
  square[0][7].setPieceAndColor(ROOK, PLAY_BLACK);
  square[1][7].setPieceAndColor(KNIGHT, PLAY_BLACK);
  square[2][7].setPieceAndColor(BISHOP, PLAY_BLACK);
  square[3][7].setPieceAndColor(QUEEN, PLAY_BLACK);
  square[4][7].setPieceAndColor(KING, PLAY_BLACK);
  square[5][7].setPieceAndColor(BISHOP, PLAY_BLACK);
  square[6][7].setPieceAndColor(KNIGHT, PLAY_BLACK);
  square[7][7].setPieceAndColor(ROOK, PLAY_BLACK);

  // Place the Pawn rows
  for (int i = 0; i < 8; i++) {
    square[i][1].setPieceAndColor(PAWN, PLAY_WHITE);
    square[i][6].setPieceAndColor(PAWN, PLAY_BLACK);

  }

  // Create the empty squares
  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      square[j][i].setPieceAndColor(EMPTY, PLAY_NONE);
    }
  }

  
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      square[i][j].setX(i);
      square[i][j].setY(j);
    }

}

bool Board::playGame()
{
//  system("cls");
//  printBoard();
//  return doMove();
}


int Board::moveKing(Square* thisKing, Square* thatSpace) {
  
  if (abs(thatSpace->getX() - thisKing->getX()) == 1) {
    if (abs(thatSpace->getY() - thisKing->getY()) == 1) {
      thatSpace->setSpace(thisKing);
      thisKing->setEmpty();
      return true;
    } // Correct move
    else {
      return false;
    } // Wrong move
  } // Check if dest is valid
  else {
    return false;
  } // Wrong move
}

int Board::moveQueen(Square* thisQueen, Square* thatSpace) { 

  int queenX = thisQueen->getX();
  int queenY = thisQueen->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();
  int yIncrement;
  int xIncrement;

  //! Moves validation flag
  bool invalid = false;

  if (queenX != thatX || queenY != thatY) {
    if (queenX == thatX) {
      yIncrement = (thatY - queenY) / (abs(thatY - queenY));
      
      for (int i = queenY + yIncrement; i != thatY; i += yIncrement) {
        if (square[thatX][i].getPieceColor() != PLAY_NONE) {
          return MOVE_QUEEN_INVALID;
        }
      }
    }
    else {
      if (queenY == thatY) {
        xIncrement = (thatX - queenX) / (abs(thatX - queenX));
        for (int i = queenX + xIncrement; i != thatX; i += xIncrement) {
          if (square[i][thatY].getPieceColor() != PLAY_NONE) {
            return MOVE_QUEEN_INVALID;
          }
        }
      }
      else {
        if (abs(queenX - thatX) == abs(queenY - thatY)) {
          xIncrement = (thatX - queenX) / (abs(thatX - queenX));
          yIncrement = (thatY - queenY) / (abs(thatY - queenY));

          for (int i = 1; i < abs(queenX - thatX); i++) {
            if (square[queenX + xIncrement*i][queenY + yIncrement*i].getPieceColor() != PLAY_NONE) {
              return MOVE_QUEEN_INVALID;
            } // Wrong move
          } // Loop on destination position
        } // Check the absolute range of moves (poisitive and negative directions)
        else {
          return MOVE_QUEEN_INVALID;
        } // Wrong move
      }
    }
  } // Check if destination square is acceptable

  if (invalid == false) {
    thatSpace->setSpace(thisQueen);
    thisQueen->setEmpty();
    return MOVE_OK;
  } // Correct move
  else {
    return MOVE_QUEEN_INVALID;
  } // Wrong move
}

int Board::moveBishop(Square* thisBishop, Square* thatSpace) { //there might be problems with number of brackets
  int bishopX = thisBishop->getX();
  int bishopY = thisBishop->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();

  //! Moves validation flag
  bool invalid = false;
  
  Square *s;
  if (abs(bishopX - thatX) == abs(bishopY - thatY)) {
    int xIncrement = (thatX - bishopX) / (abs(thatX - bishopX));
    int yIncrement = (thatY - bishopY) / (abs(thatY - bishopY));

    for (int i = 1; i < abs(bishopX - thatX); i++) {
      if (square[bishopX + xIncrement*i][bishopY + yIncrement*i].getPieceColor() != PLAY_NONE) {
        return MOVE_BISHOP_INVALID;
      } // Wrong move
    }
  } 
  else {
    return MOVE_BISHOP_INVALID;
  } // Wrong move

  if (invalid == false) {
    thatSpace->setSpace(thisBishop);
    thisBishop->setEmpty();
    return MOVE_OK;
  } // Correct move (the only positive exit point)
  else {
    return MOVE_BISHOP_INVALID;
  }
}

int Board::moveKnight(Square* thisKnight, Square* thatSpace) {
  int knightX = thisKnight->getX();
  int knightY = thisKnight->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();

  // Check for Knight valid moves
  if ((abs(knightX - thatX) == 2 && abs(knightY - thatY) == 1) || (abs(knightX - thatX) == 1 && abs(knightY - thatY) == 2)) {
    thatSpace->setSpace(thisKnight);
    thisKnight->setEmpty();
    return MOVE_OK;
  } // Move valid
  else {
    return MOVE_KNIGHT_INVALID;
  } // Wrong move
}

int Board::moveRook(Square* thisRook, Square* thatSpace) {
  int rookX = thisRook->getX();
  int rookY = thisRook->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();
  //! Move validation flag
  bool invalid = false;

  // Check for the rook move rules
  if (rookX != thatX || rookY != thatY) {
    if (rookX == thatX) {
      int yIncrement = (thatY - rookY) / (abs(thatY - rookY));
      for (int i = rookY + yIncrement; i != thatY; i += yIncrement) {
        if (square[thatX][i].getPieceColor() != PLAY_NONE) {
          return MOVE_ROOK_INVALID;
        } // Wrong move
      } // Loop on the possible landing squares
    } // Check on moves x-based
    else {
      if (rookY == thatY) {
        int xIncrement = (thatX - rookX) / (abs(thatX - rookX));
        for (int i = rookX + xIncrement; i != thatX; i += xIncrement) {
          if (square[i][thatY].getPieceColor() != PLAY_NONE) {
            return MOVE_ROOK_INVALID;
          } // Wrong move
        } // Loop on pthe possible landing squares
      } // Check on moves y-based
      else {
        return MOVE_ROOK_INVALID;
      } // Wrong move
    }
  }
  if (invalid == false) {
    thatSpace->setSpace(thisRook);
    thisRook->setEmpty();
    return MOVE_OK;
  } // All move controls passed
  else {
    return MOVE_ROOK_INVALID;
  } // Wrong move for Rook
}

int Board::movePawn(Square* thisPawn, Square* thatSpace) {
  int pawnX = thisPawn->getX();
  int pawnY = thisPawn->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();

  if (thisPawn->getPieceColor() == PLAY_WHITE) {
    if (pawnX == thatX && thatY == pawnY + 1 && thatSpace->getPieceColor() == PLAY_NONE) {
      thatSpace->setSpace(thisPawn);
      thisPawn->setEmpty();
      return MOVE_OK;
    } // Correct move
    else {
      if ((pawnX + 1 == thatX || pawnX - 1 == thatX) && pawnY + 1 == thatY  && thatSpace->getPieceColor() == PLAY_BLACK) {
        thatSpace->setSpace(thisPawn);
        thisPawn->setEmpty();
        return MOVE_OK;
      } // Eat piece is valid
      else {
        return MOVE_PAWN_INVALID;
      } // Invalid move
    } // Pawn eats 
  } // Case white player
  else {
    if (thisPawn->getPieceColor() == PLAY_BLACK) {
      if (pawnX == thatX && thatY == pawnY - 1 && thatSpace->getPieceColor() == PLAY_NONE) {
        thatSpace->setSpace(thisPawn);
        thisPawn->setEmpty();
      return MOVE_OK;
      } // Correct move
      else {
        if ((pawnX + 1 == thatX || pawnX - 1 == thatX) && pawnY - 1 == thatY  && thatSpace->getPieceColor() == PLAY_WHITE) {
          thatSpace->setSpace(thisPawn);
          thisPawn->setEmpty();
          return MOVE_OK;
        }
        else {
          return MOVE_PAWN_INVALID;
        }
      }
    } // Case black player
    else {
      return MOVE_PAWN_INVALID;
    } // Invalid move
  } // Case black player
}

int Board::makeMove(int x1, int y1, int x2, int y2) {
  // Validate from...to coordinates
  if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7 || x2 < 0 || x2 > 7 || y2 < 0 || y2 > 8) {
    return MOVE_OUT_OF_BOUND;
  }

  //! Square object pointer to the from coordinates
  Square* src = getSquare(x1, y1);
  //! Square object point to the dest coordinates
  Square* dest = getSquare(x2, y2);

  // Check if there is a piece of the same color on the destination coordinates
  if (src->getPieceColor() == dest->getPieceColor() && dest->getPieceColor() != PLAY_NONE) {
    return MOVE_SAME_COLOR_PIECE;
  }

  // Check for valid move accordingly to the game rules of the moved piece
  // found on the source coordinates.
  switch (src->getPiece()) {
    case KING: 
      return moveKing(src, dest);
      break;
    case QUEEN: 
      return moveQueen(src, dest);
      break;
    case BISHOP: 
      return moveBishop(src, dest);
      break;
    case KNIGHT: 
      return moveKnight(src, dest);
      break;
    case ROOK: 
      return moveRook(src, dest);
      break;
    case PAWN: 
      return movePawn(src, dest);
      break;
    case EMPTY: 
      return MOVE_SOURCE_EMPTY;
      break;
    default:
      return MOVE_GENERIC_ERROR;
      break;
  }
  // None of the validation cases has passed, the move is not valid
  return MOVE_GENERIC_ERROR;
}
