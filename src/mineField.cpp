#include "mineField.h"

mineField::mineField(QWidget* parent, 
	int _width, int _height, int _mineNum):
	QWidget(parent), xNum(_width), yNum(_height), mineNum(_mineNum),
	// 随机布雷
	randomRes(randomMines(_mineNum, _width, _height))
{
	//为随机雷排序方便满足结束条件
	qSort(randomRes);

	// resize ptrVector
	ptrVector.resize(xNum * yNum);
	
	// 初始化雷块 
	for (int i = 0; i < xNum * yNum; ++i)
	{
		ptrVector[i] = new blockWidget(this);
		ptrVector[i]->setXPos(i % xNum);
		ptrVector[i]->setYPos(i / xNum);
		connect(ptrVector[i], SIGNAL(explode()), this, SLOT(getExplode()));
		connect(ptrVector[i], SIGNAL(emptyBlock(int, int)), this, SLOT(getEmptyBlock(int, int)));
		connect(ptrVector[i], SIGNAL(flag(int, int, bool)), this, SLOT(getFlag(int, int, bool)));
	}

	setFixedSize(QSize(
		xNum * ptrVector[0]->width() + (xNum - 1) * 2,
		yNum * ptrVector[0]->width() + (yNum - 1) * 2));

	int w_count = 0;
	int h_count = 0;
	int len = ptrVector[0]->width();

	// 初始化雷块布局
	for (int i = 0; i < yNum; ++i)
	{
		for (int j = 0; j < xNum; ++j)
		{
			ptrVector[i * xNum + j]->setGeometry(j + w_count + 2, i + h_count + 2, len, len);
			w_count += len;
		}
		w_count = 0;
		h_count += len;
	}
	
    // 初始化nearby和isMine
	for (QVector<int>::iterator iter = randomRes.begin();
		iter != randomRes.end(); ++iter)
	{
        ptrVector[*iter]->getBlockPrivatePtr()->setMine(true);

		// 初始化nearby
		int settingSquare[3][3];
		getSettingSquare(settingSquare, *iter, xNum);

		// 遍历settingSquare
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				// 不计算不合法位置
				if (settingSquare[i][j] != -1) {
					ptrVector[settingSquare[i][j]]->getBlockPrivatePtr()->setNearBy(
						ptrVector[settingSquare[i][j]]->getBlockPrivatePtr()->getNearBy() + 1);
				}
			}
		}
	}
}

void mineField::getSettingSquare(int _square[3][3], int center, int _xNum)
{
	int x = center % _xNum;
	int y = center / _xNum;

	_square[0][0] = center - _xNum - 1;
	_square[0][1] = center - _xNum;
	_square[0][2] = center - _xNum + 1;
	_square[1][0] = center - 1;
	_square[1][1] = center;
	_square[1][2] = center + 1;
	_square[2][0] = center + _xNum - 1;
	_square[2][1] = center + _xNum;
	_square[2][2] = center + _xNum + 1;
	/*
		{ center - _xNum - 1, center - _xNum, center - _xNum + 1 },
		{ center - 1,         center,         center + 1         },
		{ center + _xNum - 1, center + _xNum, center + _xNum + 1 }
	*/

	if (x == 0) {
		_square[0][0] = -1;
		_square[1][0] = -1;
		_square[2][0] = -1;
	}
	else if (x == xNum - 1) {
		_square[0][2] = -1;
		_square[1][2] = -1;
		_square[2][2] = -1;
	}

	if (y == 0) {
		_square[0][0] = -1;
		_square[0][1] = -1;
		_square[0][2] = -1;
	}
	else if (y == yNum - 1) {
		_square[2][0] = -1;
		_square[2][1] = -1;
		_square[2][2] = -1;
	}
}

int mineField::getXNum()
{
	return xNum;
}

int mineField::getYNum()
{
	return yNum;
}

int mineField::getMineNum()
{
	return mineNum;
}

void mineField::mouseReleaseEvent(QMouseEvent* event)
{
	if (randomRes == flagBlocks) {
		emit gameOver(true);
	}
}

void mineField::resotre()
{
	for (auto iter = ptrVector.begin(); iter != ptrVector.end(); ++iter)
	{
		(*iter)->getBlockPrivatePtr()->setFlag(false);
		(*iter)->getBlockPrivatePtr()->setSweep(false);
		(*iter)->getBlockPrivatePtr()->setStatus(blockPrivate::mineNormal);
	}
	flagBlocks.clear();
	update();
}

void mineField::showMines()
{
	for (auto iter = randomRes.begin(); iter != randomRes.end(); ++iter)
	{
		ptrVector[*iter]->getBlockPrivatePtr()->setSweep(true);
		ptrVector[*iter]->getBlockPrivatePtr()->setStatus(blockPrivate::mineExplode);
	}
	update();
}

QSize mineField::sizeHint() const {
	if (!ptrVector.empty()) {
		return QSize(xNum * ptrVector[0]->width(), yNum * ptrVector[0]->width());
	}
	else {
		return QSize(200, 100);
	}
}

void mineField::getExplode()
{
	emit gameOver(false);
}

void mineField::getFlag(int _xPos, int _yPos, bool _flag)
{
	if (_flag) {
		flagBlocks.push_back(_xPos + _yPos * xNum);
	}
	else {
		auto iter = qFind(flagBlocks.begin(),flagBlocks.end(), _xPos + _yPos * xNum);
		flagBlocks.erase(iter);
	}
	qSort(flagBlocks);
	qDebug() << flagBlocks;
	qDebug() << randomRes;
}

void mineField::getEmptyBlock(int _xPos, int _yPos)
{
	int settingSquare[3][3];
	getSettingSquare(settingSquare, _yPos * xNum + _xPos, xNum);
	// 不包括自己
	blockPrivate* ptr;
	ptr = ptrVector[settingSquare[1][1]]->getBlockPrivatePtr();
	ptr->setSweep(true);
	ptr->setStatus(blockPrivate::mineClicked);
	settingSquare[1][1] = -1;
	update();

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (settingSquare[i][j] != -1) {
				ptr = ptrVector[settingSquare[i][j]]->getBlockPrivatePtr();
				if (ptr->getNearBy() && !ptr->getMine())
				{
					ptr->setSweep(true);
					ptr->setStatus(blockPrivate::mineClicked);
				}
				else {
					if (!ptr->getSweeped() && !ptr->getMine()) {
						getEmptyBlock(ptrVector[settingSquare[i][j]]->getXPos(),
							ptrVector[settingSquare[i][j]]->getYPos());
					}
				}
			}
		}
	}
}

QVector<int> mineField::randomMines(int mineNumber, int _xNum, int _yNum)
{
	QVector<int> randomResult(mineNumber);
	srand(time(NULL));
	int total = _xNum * _yNum;

	for (int i = 0; i < mineNumber; ++i)
	{
		int num = rand() % total;
		if (randomResult.end() != qFind(randomResult, num)) {
			--i;
		}
		else {
			randomResult[i] = num;
		}
	}

	return randomResult;
}
