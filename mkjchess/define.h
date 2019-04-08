#pragma once
#ifndef define_h_
#define define_h_

#define NOCHESS 0    //没有棋子

#define B_KING       1  //黑帅
#define B_CAR        2  //黑车
#define B_HORSE      3  //黑马
#define B_CANON      4  //黑炮
#define B_BISHOP     5  //黑士
#define B_ELEPHANT   6  //黑象
#define B_PAWN       7  //黑卒
#define B_BEGIN      B_KING
#define B_END        B_PAWN

#define R_KING       8   //红帅
#define R_CAR        9   //红车
#define R_HORSE      10  //红马
#define R_CANON      11  //红炮
#define R_BISHOP     12  //红士
#define R_ELEPHANT   13  //红象
#define R_PAWN       14  //红兵
#define R_BEGIN      R_KING
#define R_END        R_PAWN

#define IsBlack(x)  (x>=B_BEGIN&&x<=B_END) //判断棋子(黑色)
#define IsRed(x)  (x>=R_BEGIN&&x<=R_END)   //判断棋子(红色)
#define IsSameSide(x,y)  ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))  //是否自己的棋子
#define WM_FIRSTSTEP_MSG WM_USER + 100
typedef struct _chessmanposition
{
	BYTE		x;
	BYTE		y;
}CHESSMANPOS;

typedef struct _chessmove
{
	short	  	    ChessID;	//什么棋子
	CHESSMANPOS	From;         //起始位置
	CHESSMANPOS	To;			//落子位置
	int			    Score;		// 值
}CHESSMOVE;

#endif //define_h_