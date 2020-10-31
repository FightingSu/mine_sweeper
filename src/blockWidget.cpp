#include "blockWidget.h"

QVector<QPixmap> blockWidget::pixmaps;


blockWidget::blockWidget(QWidget* parent):
	QWidget(parent), xPos(0), yPos(0),
	pimpl(new blockPrivate)
{
	initPixmaps();
	pen.setWidthF(3);
	font.setFamily("Microsoft YaHei UI");
	font.setPointSize(14);

	setFixedSize(pixmaps[0].size());
}

void blockWidget::initPixmaps() {
	pixmaps.resize(5);
	blockWidget::pixmaps[blockPrivate::mineNormal].load(":/mine_normal.png");
	blockWidget::pixmaps[blockPrivate::mineHover].load(":/mine_hover.png");
	blockWidget::pixmaps[blockPrivate::mineClicked].load(":/mine_clicked.png");
	blockWidget::pixmaps[blockPrivate::mineFlaged].load(":/mine_flaged.png");
	blockWidget::pixmaps[blockPrivate::mineExplode].load(":/mine_explode.png");
}


blockWidget::~blockWidget()
{
	delete pimpl;
}

blockPrivate* blockWidget::getBlockPrivatePtr()
{
	return pimpl;
}

void blockWidget::setXPos(int _xPos)
{
	xPos = _xPos;
}
void blockWidget::setYPos(int _yPos) 
{
	yPos = _yPos;
}
int blockWidget::getXPos()
{
	return xPos;
}
int blockWidget::getYPos()
{
	return yPos;
}

void blockWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	pen.setColor(QColor(colorGroup[pimpl->getNearBy()]));
	painter.setPen(pen);
	painter.setFont(font);

	painter.drawPixmap(rect(), pixmaps[pimpl->getStatus()]);

	//painter.drawPixmap(rect(),pixmaps[pimpl->getStatus()]);
	if (pimpl->getSweeped() && pimpl->getNearBy() && !pimpl->getMine())
	{
		painter.drawText(QRect(15, 8, 20, 20), QString("%1").arg(pimpl->getNearBy()));
	}
}

void blockWidget::enterEvent(QEvent* event)
{
	if (!pimpl->getSweeped() && !pimpl->getFlag())
	{
		pimpl->setStatus(blockPrivate::mineHover);
	}
	update();
}
void blockWidget::leaveEvent(QEvent* event)
{
	if (!pimpl->getSweeped() && !pimpl->getFlag())
	{
		pimpl->setStatus(blockPrivate::mineNormal);
	}
	update();
}

void blockWidget::mousePressEvent(QMouseEvent* event) 
{
	if (!pimpl->getSweeped() && !pimpl->getFlag())
	{
		pimpl->setStatus(blockPrivate::mineClicked);
	}
	QWidget::mousePressEvent(event);
	update();
}

void blockWidget::mouseReleaseEvent(QMouseEvent* event) 
{
	// 鼠标释放点在这个块上
	if (rect().contains(event->pos()))
	{
		// 左键，且没有Flag
		if (event->button() == Qt::LeftButton && !pimpl->getFlag()) {
			pimpl->setSweep(true);
			if (pimpl->getMine()) {
				pimpl->setStatus(blockPrivate::mineExplode);
				emit explode();
			}
			else {
				pimpl->setStatus(blockPrivate::mineClicked);
				if (!pimpl->getNearBy())
				{
					emit emptyBlock(xPos, yPos);
				}
			}
		}
		// 右键，设置Flag
		else if (event->button() == Qt::RightButton && !pimpl->getSweeped()) {
			pimpl->setFlag(!pimpl->getFlag());
			if (pimpl->getFlag()) {
				pimpl->setStatus(blockPrivate::mineFlaged);
			}
			else {
				pimpl->setStatus(blockPrivate::mineNormal);
			}
			emit flag(xPos, yPos, pimpl->getFlag());
		}
	}
	else
	{
		pimpl->setStatus(blockPrivate::mineNormal);
	}
	QWidget::mouseReleaseEvent(event);
	update();
}