#ifndef _CHESS_MOVES
#define _CHESS_MOVES

#include <cmath>
#include <string>
#include <Streaming.h>

//! If TERMINAL_MODE is defined the game is emulated on the serial
//! terminal instead of the display
#define TERMINAL_MODE

//! Definition of the pieces, including the empty square
enum chessPiece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
//! Definition of the player color. Color type NONE is for an empy square
enum chessColor { WHITE, BLACK, NONE };

/**
 * Square is the class that manages the single square with the piece
 * on it, if any.
 */
class Square {
  //! Pieces enumerator type
  chessPiece piece;
  //! Player color anumerator type
  chessColor color;
  //! Chessboard square coordinates
  int x, y;

public:
  void setSpace(Square*);
  void setEmpty();
  void setPieceAndColor(Piece, Color);

  chessPiece getPiece();
  chessColor getColor();
  void setX(int ex) { x = ex; }
  void setY(int why) { y = why; }
  int getX() { return x; }
  int getY() { return y; }
  Square();
};

class Board {
  Square square[8][8];
  Color turn=WHITE;
  bool moveKing(Square* thisKing, Square* thatSpace);
  bool moveQueen(Square* thisQueen, Square* thatSpace);
  bool moveBishop(Square* thisBishop, Square* thatSpace);
  bool moveKnight(Square* thisKnight, Square* thatSpace);
  bool moveRook(Square* thisRook, Square* thatSpace);
  bool movePawn(Square* thisPawn, Square* thatSpace);
  bool makeMove(int x1, int y1, int x2, int y2);
  void printBoard();
public:
  Square* getSquare(int x, int y) {
    return &square[x][y];
  }
  void setSquare(Square * s, int x, int y){
    square[x][y]=*s;
  }
  bool doMove();
  
  void setBoard();
  bool playGame();
};

#endif
