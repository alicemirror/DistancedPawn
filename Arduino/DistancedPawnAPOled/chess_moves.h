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

#define ERR_OUT_OF_BOUND        "Coordinates out of bound"
#define ERR_SAME_COLOR_PIECE    "Invalid move: cannot land on your own ChessPiece"
#define ERR_SOURCE_EMPTY        "No player piece on the origin move coordinates"
#define ERR_GENERIC_ERROR       "Invalid move"
#define ERR_PAWN                "Wrong pawn move"
#define ERR_ROOK                "Wrong rook move"
#define ERR_KNIGHT              "Wrong knight move"
#define ERR_BISHOP              "Wrong bishop move"
#define ERR_QUEEN               "Wrong queen move"
#define ERR_KING                "Wrong king move"

#define MOVE_OK                 0   //!< Correct move
#define MOVE_OUT_OF_BOUND       1   //!< Move out of bound
#define MOVE_SAME_COLOR_PIECE   2   //!< A piece of the same color still present on the dest coordinates
#define MOVE_SOURCE_EMPTY       3   //!< The from coordinates have no pieces on it
#define MOVE_GENERIC_ERROR      4   //!< The move is invalid
#define MOVE_PAWN_INVALID       5   //!< Invalid pawn move
#define MOVE_ROOK_INVALID       6   //!< Invalid rook move
#define MOVE_KNIGHT_INVALID     7   //!< Invalid knight move
#define MOVE_BISHOP_INVALID     8   //!< Invalid bishop move
#define MOVE_QUEEN_INVALID      9   //!< Invalid queen move
#define MOVE_KING_INVALID      10   //!< Invalid king move

//! Definition of the pieces, including the empty square
enum ChessPiece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
//! Definition of the player color. Color type NONE is for an empy square
enum ChessColor { PLAY_WHITE, PLAY_BLACK, PLAY_NONE };

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
  
  //! Retrieve the current piece color
  ChessColor getPieceColor();
  
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
  ChessColor turn = PLAY_WHITE;

  /**
   * Check for the Kingueen rule and makes the move
   * 
   * @param thisKing Pointer to the Square object with the King
   * @param thatSpace Pinter to the Square object where move the Piece
   * 
   * @return If the move is valid according to the piece rules returns MOVE_OK
   * else return the wrong rule error
   */
  int moveKing(Square* thisKing, Square* thatSpace);
  
  /**
   * Check for the Queen rule and makes the move
   * 
   * @param thisQueen Pointer to the Square object with the Queen
   * @param thatSpace Pinter to the Square object where move the Piece
   * 
   * @return If the move is valid according to the piece rules returns MOVE_OK
   * else return the wrong rule error
   */
  int moveQueen(Square* thisQueen, Square* thatSpace);
  
  /**
   * Check for the Bishop rule and makes the move
   * 
   * @param thisBishop Pointer to the Square object with the Bishop
   * @param thatSpace Pinter to the Square object where move the Piece
   * 
   * @return If the move is valid according to the piece rules returns MOVE_OK
   * else return the wrong rule error
   */
  int moveBishop(Square* thisBishop, Square* thatSpace);
  
  /**
   * Check for the Knight rule and makes the move
   * 
   * @param thisRook Pointer to the Square object with the Knight
   * @param thatSpace Pinter to the Square object where move the Piece
   * 
   * @return If the move is valid according to the piece rules returns MOVE_OK
   * else return the wrong rule error
   */
  int moveKnight(Square* thisKnight, Square* thatSpace);
  
  /**
   * Check for the Rook rule and makes the move
   * 
   * @param thisRook Pointer to the Square object with the Rook
   * @param thatSpace Pinter to the Square object where move the Piece
   * 
   * @return If the move is valid according to the piece rules returns MOVE_OK
   * else return the wrong rule error
   */
  int moveRook(Square* thisRook, Square* thatSpace);
  
  /**
   * Check for the Pawn rule and makes the move
   * 
   * @param thisPawn Pointer to the Square object with the Pawn
   * @param thatSpace Pinter to the Square object where move the Piece
   * 
   * @return If the move is valid according to the piece rules returns MOVE_OK
   * else return the wrong rule error
   */
  int movePawn(Square* thisPawn, Square* thatSpace);
  
  /** 
   * Executes the move on the board checking for vaidity.
   * 
   * It is expected that this method is called after checking for turns
   * 
   * @params x1, y1 Start coordinates of the move
   * @params x2, y2 Destination coordinates of the move
   * 
   * @return One of the move statuses
   */
  int makeMove(int x1, int y1, int x2, int y2);
  
  /** 
   * Draw the board on the display of the controller. 
   * This method manages all the game conditions and accordingly to the
   * application settings draw the board in the right place (serial console
   * of web page)
   */
  void drawDisplayBoard();
  
  /**
   * Draw the board to the https client.
   * 
   * The board drawing is replaced by the move sent via GET call when two devices
   * are connected via WiFi to play distanced.
   * 
   * If only the Arduino MKR1010 configured as access point manages the game this
   * method draw the updated board on the browser after a move has been done.
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
  Square* getSquare(int x, int y) { return &square[x][y]; }

  /**
   * Set the square at the desired position
   * 
   * @param s Pointer to the Square class
   * @param x, y The square coordinates on the board
   */
  void setSquare(Square * s, int x, int y) { square[x][y]=*s; }

  //! Check for a valid move and executes it
  bool doMove();
  
  //! Initializes the board bi-dimensional array to the start of game
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
