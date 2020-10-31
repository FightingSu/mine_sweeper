#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "mineField.h"

#include <QMenuBar>
#include <QLineEdit>
#include <QLabel>
#include <QAction>
#include <QStatusBar>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QResizeEvent>

#include <QDebug>
#include <QTimer>

class mainWindow: public QWidget
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = Q_NULLPTR);
	~mainWindow();

	// reimplement function
	QSize sizeHint() const;

private Q_SLOTS:
	void resetMineField(int xNum, int yNum, int MineNum);
	void defaultDifficultySetting(bool checked);
	void getGameOver(bool _lifeStatus);

private:
	QTimer timer;
	QMenuBar* menuBar;
	QStatusBar* statusBar;
	QVBoxLayout* mainLayout;
	QVBoxLayout* contentLayout;

	// 当前雷区
	mineField* field;

	// 上一个雷区

	// 为什么要有上一个雷区？
	// 因为中雷的时候会发送 gameOver() 信号
	// 在与其相连的槽中若析构这个雷区
	// 槽函数调用完成时，返回继续执行发送信号的函数将会“找不到”
	// 因为已经被析构了
	// 所以需要一个指针保有上一个雷区，在下一次创建新雷区的时候再析构
	mineField* preField;

	QAction* userDefine;
	// 9 * 9 10
	QAction* easy;
	// 16 * 16 40
	QAction* middle;
	// 30 * 16 99
	QAction* hard;
};

#endif // _MAIN_WINDOW_H_
