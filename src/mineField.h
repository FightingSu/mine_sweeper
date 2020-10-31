#ifndef _MINE_FIELD_H_
#define _MINE_FIELD_H_

#include "blockWidget.h"
#include <cstdlib>
#include <ctime>
#include <QWidget>
#include <QVector>
#include <QMessageBox>
#include <QMouseEvent>

#include <QDebug>

class mineField :
	public QWidget
{
	Q_OBJECT

public:
	mineField(QWidget* parent = nullptr, 
		int _width = 9, int _height = 9, int _mineNum = 10);

	void getSettingSquare(int _square[3][3], int center, int _xNum);
	int  getXNum();
	int  getYNum();
	int  getMineNum();
	void mouseReleaseEvent(QMouseEvent* event);
	void resotre();
	void showMines();

	// reimplement function
	QSize sizeHint() const;

public Q_SLOTS:
	void getExplode();
	void getFlag(int _xPos, int _yPos, bool _flag);
	void getEmptyBlock(int _xPos, int _yPos);

Q_SIGNALS:
	// true alive
	// false dead
	void gameOver(bool _lifeStatus);

private:
	// 为什么使用 QVector<blockWidget*> 而不是 blockWidget*？
	// 因为 这里需要申请多个 blockWidget， 每个 blockWidget 的父对象都是 mineField
	// 当释放第一个 blockWidget 的时候， mineField 会将 blockWidget 当作 blockWidget* 释放
	// 但是实际上它是一个 blockWidget* []
	// 相当于 delete [] 用成了 delete 
	QVector<blockWidget*> ptrVector;
	QVector<int> randomRes;
	QVector<int> flagBlocks;
	int xNum;
	int yNum;
	int mineNum;

	QVector<int> randomMines(int mineNumber, int _xNum, int _yNum);
};

#endif // _MINE_FIELD_H_