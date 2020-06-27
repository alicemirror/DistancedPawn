/**
 * @file chess_moves.h
 * @brief header of the classes that manages the moves and the board
 * 
 * The game can be played with two interactive boards in a server-client configuration
 * with the fixed IP address of the access point Arduino MKR1010 is automatically
 * connected by the remote client Arduino MKR1010.\n
 * On the server side, indeed, it is possible to play a single game where the remote user
 * plays with a browser while the local user plays using the serial terminal.\n
 * The kind of game to be played depends on the header definition of TERMINAL_MODE
 */

#ifndef _CHESS_MOVES
#define _CHESS_MOVES

#include <Streaming.h>

//! If TERMINAL_MODE is defined the game is emulated on the serial
//! terminal and html remote web instead of the display
#define TERMINAL_MODE
//! Board output type (where the board should be drawn)
#define BOARD_SERIAL 1
#define BOARD_HTML 2
#define BOARD_DISPLAY 3

//! Definition of the pieces, including the empty square
enum ChessPiece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
//! Definition of the player color. Color type NONE is for an empy square
enum ChessColor { WHITE, BLACK, NONE };

/**
 * Square is the class that manages the single square with the piece
 * on it, if any. This class is controlled by the class Board
 */
class Square {
  //! Pieces enumerator type
  ChessPiece piece;
  //! Player color anumerator type
  ChessColor color;
  //! Chessboard square coordinates
  int x, y;

public:
  //! Represent a square of the board with the right color
  void setSpace(Square*);
  //! Set an empty square
  void setEmpty();
  /**
   * Set a piece of the correct color on a square
   * 
   * @param ChessPiece, ChessColor Piece and color enumerators
   */
  void setPieceAndColor(ChessPiece, ChessColor);
  //! Retrieve the current piece
  ChessPiece getPiece();
  //! Retrieve the current square color
  ChessColor getSquareColor();
  //! Set the desired x coordinate
  void setX(int ex) { x = ex; }
  //! Set the desired y coordinate
  void setY(int why) { y = why; }
  //! Get the desired x coordinate
  int getX() { return x; }
  //! Get the desired y coordinate
  int getY() { return y; }
  //! The Square class constructor initializes the square to empy with no color
  Square();
};

/**
 * The Board class defines the board in a 8x8 two-dimensions array and manage
 * the pieces moves
 */
class Board {
  //! Definition of the two-dimensions array representing the board
  Square square[8][8];
  //! Current player turn. Start with white always
  ChessColor turn = WHITE;
  //! Manage the move of the King
  bool moveKing(Square* thisKing, Square* thatSpace);
  //! Manage the move of the Queen
  bool moveQueen(Square* thisQueen, Square* thatSpace);
  //! Manage the move of the Bishop
  bool moveBishop(Square* thisBishop, Square* thatSpace);
  //! Manage the move of the Knight
  bool moveKnight(Square* thisKnight, Square* thatSpace);
  //! Manage the move of the Rook
  bool moveRook(Square* thisRook, Square* thatSpace);
  //! Manage the move of the Pawn
  bool movePawn(Square* thisPawn, Square* thatSpace);
  /** 
   * Executes the move on the board 
   * 
   * @params x1, y1 Start coordinates of the move
   * @params x2, y2 Destination coordinates of the move
   */
  bool makeMove(int x1, int y1, int x2, int y2);
  /** 
   * Draw the board to the display of the controller. 
   * This method is called when the game is played in normal conditions 
   * where two remote boards are connected via the WiFi.
   */
  void drawDisplayBoard();
  /**
   * Draw the board to the https client.
   * 
   * The board drawing is replaced by the move sent via GET call when two devices
   * are connected via WiFi to play distanced.
   */
  void drawHtmlBoard();
  /**
   * Draw the board on the serial console.
   * 
   * When the game is played by only one side, the local player can play with the serial
   * terminal while the remote will play from the browser.
   */
  void drawSerialBoard();

public:
  /** 
   * Pointer to the Square class at the specific board coordinates 
   * 
   * @params x, y coordinates of the square
   */
  Square* getSquare(int x, int y) {
    return &square[x][y];
  }
  /**
   * Set the square at the desired position
   * 
   * @param s Pointer to the Square class
   * @param x, y The square coordinates on the board
   */
  void setSquare(Square * s, int x, int y){
    square[x][y]=*s;
  }
  //! Check for a valid move and executes it
  bool doMove();
  //! Initializes the board to the start of game
  void setBoard();
  //! Play a game
  bool playGame();
  /** 
   * This method updates the board with the last move, accordingly to the current 
   * output type (serial, web page, display)
   * 
   * @param int t The kind of desired output
   */
  void drawBoard(int t);
};

#endif
