/**
 * @file chess_moves.h
 * @brief header of the classes that manages the moves and the board
 */

#include "chess_moves.h"

// --------------------------------------------------------------------- Square class
Square::Square() {
  piece = EMPTY;
  color = NONE;
}

void Square::setSpace(Square* space) {
  color = space->getSquareColor();
  piece = space->getPiece();
}

void Square::setEmpty() {
  color = NONE;
  piece = EMPTY;
}

ChessPiece Square::getPiece() {
  return piece;
}

ChessColor Square::getSquareColor() {
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
  // Loop by row and columns
  for (int i = 0; i < 8; i++) {
    Serial << " " << i << "   ";
    for (int j = 0; j < 8; j++) {
      // Set the ChessPieces in the starting position and
      // draw the squares
      ChessPiece p = square[i][j].getPiece();
      ChessColor c = square[i][j].getSquareColor();

      switch (p)
      {
      case KING: (c == WHITE) ? Serial << " K " : Serial << " k ";
        break;
      case QUEEN: (c == WHITE) ? Serial << " Q " : Serial << " q ";
        break;
      case BISHOP:(c == WHITE) ? Serial << " B " : Serial << " b ";
        break;
      case KNIGHT:(c == WHITE) ? Serial << " H " : Serial << " h ";
        break;
      case ROOK: (c == WHITE) ? Serial << " R " : Serial << " r ";
        break;
      case PAWN: (c == WHITE) ? Serial << " P " : Serial << " p ";
        break;
      case EMPTY: Serial << " " << "\21" << " ";
        break;
      default: Serial << "XXX";
        break;
      }

    }
    Serial << endl;
  }
}

void Board::drawHtmlBoard() {
//  cout << "   y: 0  1  2  3  4  5  6  7 " << endl << "x:" << endl;
//  for (int i = 0; i < 8; i++)
//  {
//    cout << " " << i << "   ";
//    for (int j = 0; j < 8; j++)
//    {
//      ChessPiece p = square[i][j].getPiece();
//      ChessColor c = square[i][j].getSquareColor();
//
//      switch (p)
//      {
//      case KING: (c == WHITE) ? cout << " K " : cout << " k ";
//        break;
//      case QUEEN: (c == WHITE) ? cout << " Q " : cout << " q ";
//        break;
//      case BISHOP:(c == WHITE) ? cout << " B " : cout << " b ";
//        break;
//      case KNIGHT:(c == WHITE) ? cout << " H " : cout << " h ";
//        break;
//      case ROOK: (c == WHITE) ? cout << " R " : cout << " r ";
//        break;
//      case PAWN: (c == WHITE) ? cout << " P " : cout << " p ";
//        break;
//      case EMPTY: cout << " " << "\21" << " ";
//        break;
//      default: cout << "XXX";
//        break;
//      }
//
//    }
//    cout << endl;
//  }
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
//    (turn == WHITE) ? cout << "White's turn" << endl : cout << "Black's turn" << endl;
//    cout << "Type in your move as a single four character string. Use x-coordinates first in each pair." << endl;
//    cin >> move;
//    x1 = move[0] - 48;
//    y1 = move[1] - 48;
//    x2 = move[2] - 48;
//    y2 = move[3] - 48;
//    if (getSquare(x1, y1)->getSquareColor() == turn)
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
//    if (getSquare(x1, y1)->getSquareColor() == WHITE)
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
//    turn = WHITE;
//  else
//    turn = BLACK;

  return true;

}

void Board::setBoard()
{
  square[0][0].setPieceAndColor(ROOK, WHITE);
  square[1][0].setPieceAndColor(KNIGHT, WHITE);
  square[2][0].setPieceAndColor(BISHOP, WHITE);
  square[3][0].setPieceAndColor(QUEEN, WHITE);
  square[4][0].setPieceAndColor(KING, WHITE);
  square[5][0].setPieceAndColor(BISHOP, WHITE);
  square[6][0].setPieceAndColor(KNIGHT, WHITE);
  square[7][0].setPieceAndColor(ROOK, WHITE);

  square[0][7].setPieceAndColor(ROOK, BLACK);
  square[1][7].setPieceAndColor(KNIGHT, BLACK);
  square[2][7].setPieceAndColor(BISHOP, BLACK);
  square[3][7].setPieceAndColor(QUEEN, BLACK);
  square[4][7].setPieceAndColor(KING, BLACK);
  square[5][7].setPieceAndColor(BISHOP, BLACK);
  square[6][7].setPieceAndColor(KNIGHT, BLACK);
  square[7][7].setPieceAndColor(ROOK, BLACK);

  for (int i = 0; i < 8; i++)
  {
    square[i][1].setPieceAndColor(PAWN, WHITE);
    square[i][6].setPieceAndColor(PAWN, BLACK);

  }
  for (int i = 2; i < 6; i++)
  {
    for (int j = 0; j < 8; j++)
      square[j][i].setPieceAndColor(EMPTY, NONE);

  }
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
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

bool Board::moveKing(Square* thisKing, Square* thatSpace) {
  //off board inputs should be handled elsewhere (before this)
  //squares with same ChessColor should be handled elsewhere (before this)
  if (abs(thatSpace->getX() - thisKing->getX()) == 1)
    if (abs(thatSpace->getY() - thisKing->getY()) == 1)
    {
      thatSpace->setSpace(thisKing);
      thisKing->setEmpty();
      return true;
    }
    else return false;
  else return false;
}

bool Board::moveQueen(Square* thisQueen, Square* thatSpace) { //there might be problems with numbers of brackets
                             //off board inputs should be handled elsewhere (before this)
                             //squares with same ChessColor should be handled elsewhere (before this)

  int queenX = thisQueen->getX();
  int queenY = thisQueen->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();
//  std::cout << "this";
  int yIncrement;
  int xIncrement;

  bool invalid = false;
  if (queenX != thatX || queenY != thatY)
  {

    if (queenX == thatX)
    {
      yIncrement = (thatY - queenY) / (abs(thatY - queenY));
      for (int i = queenY + yIncrement; i != thatY; i += yIncrement)
      {

        if (square[thatX][i].getSquareColor() != NONE)
          return false;

      }
    }
    else
      if (queenY == thatY)
      {

        xIncrement = (thatX - queenX) / (abs(thatX - queenX));
        for (int i = queenX + xIncrement; i != thatX; i += xIncrement)
        {
          if (square[i][thatY].getSquareColor() != NONE)
            return false;
        }
      }
      else
        if (abs(queenX - thatX) == abs(queenY - thatY))
        {
          xIncrement = (thatX - queenX) / (abs(thatX - queenX));
          yIncrement = (thatY - queenY) / (abs(thatY - queenY));

          for (int i = 1; i < abs(queenX - thatX); i++)
          {
//            std::cout << "It got here somehow";
            if (square[queenX + xIncrement*i][queenY + yIncrement*i].getSquareColor() != NONE)
              return false;

          }
        }
        else
          return false;
    //if()
  }



  if (invalid == false)
  {
    thatSpace->setSpace(thisQueen);
    thisQueen->setEmpty();
    return true;
  }
  else
  {
    return false;
  }
}

bool Board::moveBishop(Square* thisBishop, Square* thatSpace) { //there might be problems with number of brackets
  int bishopX = thisBishop->getX();
  int bishopY = thisBishop->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();
  bool invalid = false;
  Square *s;
  if (abs(bishopX - thatX) == abs(bishopY - thatY))
  {
    int xIncrement = (thatX - bishopX) / (abs(thatX - bishopX));
    int yIncrement = (thatY - bishopY) / (abs(thatY - bishopY));

    for (int i = 1; i < abs(bishopX - thatX); i++)
    {
//      std::cout << "It got here somehow";
      if (square[bishopX + xIncrement*i][bishopY + yIncrement*i].getSquareColor() != NONE)
        return false;

    }
  }
  else
    return false;

  if (invalid == false)
  {
    thatSpace->setSpace(thisBishop);
    thisBishop->setEmpty();
    return true;
  }
  else
  {
    return false;
  }
}
bool Board::moveKnight(Square* thisKnight, Square* thatSpace)
{
  //off board inputs should be handled elsewhere (before this)
  //squares with same ChessColor should be handled elsewhere (before this)
  int knightX = thisKnight->getX();
  int knightY = thisKnight->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();

  if ((abs(knightX - thatX) == 2 && abs(knightY - thatY) == 1) || (abs(knightX - thatX) == 1 && abs(knightY - thatY) == 2))
  {
    thatSpace->setSpace(thisKnight);
    thisKnight->setEmpty();
    return true;
  }
  else
  {
    return false;
  }
}

bool Board::moveRook(Square* thisRook, Square* thatSpace)
{
  //off board inputs should be handled elsewhere (before this)
  //squares with same ChessColor should be handled elsewhere (before this)
  int rookX = thisRook->getX();
  int rookY = thisRook->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();
  bool invalid = false;
  if (rookX != thatX || rookY != thatY)
  {

    if (rookX == thatX)
    {
      int yIncrement = (thatY - rookY) / (abs(thatY - rookY));
      for (int i = rookY + yIncrement; i != thatY; i += yIncrement)
      {

        if (square[thatX][i].getSquareColor() != NONE)
          return false;

      }
    }
    else
      if (rookY == thatY)
      {

        int xIncrement = (thatX - rookX) / (abs(thatX - rookX));
        for (int i = rookX + xIncrement; i != thatX; i += xIncrement)
        {
          if (square[i][thatY].getSquareColor() != NONE)
            return false;
        }
      }
      else
        return false;
  }
  if (invalid == false)
  {
    thatSpace->setSpace(thisRook);
    thisRook->setEmpty();
    return true;
  }
  else
  {//Return some erorr or something. Probably return false;
//    std::cout << "That is an invalid move for rook";
    return false;
  }
}

bool Board::movePawn(Square* thisPawn, Square* thatSpace) {
  //off board inputs should be handled elsewhere (before this)
  //squares with same ChessColor should be handled elsewhere (before this)
  bool invalid = false;
  int pawnX = thisPawn->getX();
  int pawnY = thisPawn->getY();
  int thatX = thatSpace->getX();
  int thatY = thatSpace->getY();


  if (thisPawn->getSquareColor() == WHITE)
  {

    if (pawnX == thatX && thatY == pawnY + 1 && thatSpace->getSquareColor() == NONE)
    {
      thatSpace->setSpace(thisPawn);
      thisPawn->setEmpty();
      return true;
    }
    else
      if ((pawnX + 1 == thatX || pawnX - 1 == thatX) && pawnY + 1 == thatY  && thatSpace->getSquareColor() == BLACK)
      {
        thatSpace->setSpace(thisPawn);
        thisPawn->setEmpty();
        return true;
      }
      else
        return false;
  }
  else
    if (thisPawn->getSquareColor() == BLACK)
    {
      if (pawnX == thatX && thatY == pawnY - 1 && thatSpace->getSquareColor() == NONE)
      {
        thatSpace->setSpace(thisPawn);
        thisPawn->setEmpty();
        return true;
      }
      else
        if ((pawnX + 1 == thatX || pawnX - 1 == thatX) && pawnY - 1 == thatY  && thatSpace->getSquareColor() == WHITE)
        {
          thatSpace->setSpace(thisPawn);
          thisPawn->setEmpty();
          return true;
        }
        else
          return false;
    }
    else
      return false;
}
bool Board::makeMove(int x1, int y1, int x2, int y2) {
  //Checking for turns will be done previous to this
  if (x1 < 0 || x1>7 || y1 < 0 || y1>7 || x2 < 0 || x2>7 || y2 < 0 || y2>8)
  {
//    std::cout << "One of your inputs was our of bounds" << std::endl;
    return false;
  }
  Square* src = getSquare(x1, y1);
  Square* dest = getSquare(x2, y2);

  if (src->getSquareColor() == dest->getSquareColor() && dest->getSquareColor() != NONE)
  {
//    std::cout << "Invalid move: cannot land on your own ChessPiece" << std::endl;
    return false;
  }

  switch (src->getPiece())
  {
  case KING: return moveKing(src, dest);
    break;
  case QUEEN: return moveQueen(src, dest);
    break;
  case BISHOP: return moveBishop(src, dest);
    break;
  case KNIGHT: return moveKnight(src, dest);
    break;
  case ROOK: return moveRook(src, dest);
    break;
  case PAWN: return movePawn(src, dest);
    break;
  case EMPTY: // std::cout << "You do not have a ChessPiece there" << std::endl;  return false;
    break;
  default: // std::cerr << "Something went wrong in the switch statement in makeMove()" << std::endl;
    break;
  }
  return false;
}
