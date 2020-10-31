#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "blockPrivate.h"
#include <QTimer>
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QPainter>

#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>

class blockWidget :
	public QWidget
{
	Q_OBJECT

public:

	enum nearByColor {
		zero = 0xffffff,
		one = 0x2fa7e4,
		two = 0x28b27d,
		three = 0xd51d1d,
		four = 0x0f54af,
		five = 0x147d1e,
		six = 0x450404,
		seven = 0x0e2f40,
		eight = 0x0a5129
	} colorGroup[9] = {
		zero, one, two, 
		three, four, five, 
		six, seven, eight
	};

	blockWidget(QWidget* parent = nullptr);
	~blockWidget();

	// functions
	blockPrivate* getBlockPrivatePtr();

	// events
	void paintEvent(QPaintEvent* event);
	void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void setXPos(int _xPos);
	void setYPos(int _yPos);
	int getXPos();
	int getYPos();

Q_SIGNALS:
	void explode();
	void flag(int, int, bool);
	void emptyBlock(int, int);

private:
	// 静态成员，共享QPixmap节约内存
	// 然而并没有什么卵用，QPixmap是隐式共享的
	static QVector<QPixmap> pixmaps;
	static void initPixmaps();
	QPen pen;
	QFont font;

	// 为什么放在 blockWidget 不放在 blockPrivate
	// 因为和mineFields交互的是blockWidget
	// 若是其他交互方式，显然和控件绑定更好
	int xPos;
	int yPos;
	
	// pimpl 降低编译耦合度
	// 将模块信息封装起来方便复用
	blockPrivate* pimpl;
};

#endif // _BLOCK_H_
