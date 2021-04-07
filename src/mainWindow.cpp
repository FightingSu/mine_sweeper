#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QWidget(parent),
	menuBar(new QMenuBar(this)),
	statusBar(new QStatusBar(this)), 
	mainLayout(new QVBoxLayout(this)),
	contentLayout(new QVBoxLayout), field (new mineField(this))
{
	setLayout(mainLayout);
	mainLayout->addWidget(menuBar);
	mainLayout->addLayout(contentLayout);
	contentLayout->addWidget(field);
	contentLayout->setContentsMargins(4, 4, 4, 4);
	mainLayout->addWidget(statusBar);
	preField = 0;

	menuBar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	statusBar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

	QMenu* settings = menuBar->addMenu(QString(u8"设置"));
	QMenu* difficulty = settings->addMenu(QString(u8"难度设置"));
	userDefine = difficulty->addAction(QString(u8"用户自定义"));
	// 9 * 9 10
	easy = difficulty->addAction(QString(u8"简单 9 x 9"));
	// 16 * 16 40
	middle = difficulty->addAction(QString(u8"中等 16 * 16"));
	// 30 * 16 99
	hard = difficulty->addAction(QString(u8"难 30 * 16"));

	//timer.setInterval(1000);
	//timer.start();
	//connect(&timer, SIGNAL(timeout()), this, SLOT(newGames()));
	

	connect(userDefine, SIGNAL(triggered(bool)), this, SLOT(defaultDifficultySetting(bool)));
	connect(easy, SIGNAL(triggered(bool)), this, SLOT(defaultDifficultySetting(bool)));
	connect(middle, SIGNAL(triggered(bool)), this, SLOT(defaultDifficultySetting(bool)));
	connect(hard, SIGNAL(triggered(bool)), this, SLOT(defaultDifficultySetting(bool)));
	connect(field, SIGNAL(gameOver(bool)), this, SLOT(getGameOver(bool)));

	//connect(this, SIGNAL(createNewGame(int, int, int)), this, SLOT(resetMineField(int, int, int)));
}

mainWindow::~mainWindow()
{
}

QSize mainWindow::sizeHint() const {
	return field->sizeHint();
}

void mainWindow::resetMineField(int xNum, int yNum, int MineNum)
{
	if (MineNum > xNum * yNum) {
		QMessageBox::warning(this, QString(u8"提示"), QString(u8"别逗了，雷不能比块还多"));
	}
	else {
		disconnect(field, SIGNAL(gameOver(bool)), this, SLOT(getGameOver(bool)));
		
		contentLayout->removeWidget(field);
		field->setParent(Q_NULLPTR);

		if (preField) {
			// qDebug() << "release!";
			delete preField;
			preField = field;
		}
		else {
			preField = field;
		}

		field = new mineField(this, xNum, yNum, MineNum);
		contentLayout->addWidget(field);
		adjustSize();

		connect(field, SIGNAL(gameOver(bool)), this, SLOT(getGameOver(bool)));
	}
}

void mainWindow::defaultDifficultySetting(bool checked)
{
	QAction* act = qobject_cast<QAction*>(QObject::sender());
	if (act == easy) {
		resetMineField(9, 9, 10);
		//qDebug() << "easy";
	}
	else if (act == middle) {
		resetMineField(16, 16, 40);
		//qDebug() << "middle";
	}
	else if (act == hard) {
		resetMineField(30, 16, 99);
		//qDebug() << "hard";
	}
	else if (act == userDefine) {
		//qDebug() << "userDefine";
		QDialog dialog;
		QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
		connect(btn, SIGNAL(accepted()), &dialog, SLOT(accept()));
		connect(btn, SIGNAL(rejected()), &dialog, SLOT(reject()));
		dialog.setWindowTitle(QString(u8"雷区设置"));

		QVBoxLayout* mainDialogLayout = new QVBoxLayout(&dialog);

		QHBoxLayout* layout_xNum = new QHBoxLayout;
		QHBoxLayout* layout_yNum = new QHBoxLayout;
		QHBoxLayout* layout_mineNum = new QHBoxLayout;

		mainDialogLayout->addLayout(layout_xNum);
		mainDialogLayout->addLayout(layout_yNum);
		mainDialogLayout->addLayout(layout_mineNum);
		mainDialogLayout->addWidget(btn);

		QLabel* label_xNum = new QLabel(QString(u8"长"), &dialog);
		QLineEdit* edit_xNum = new QLineEdit(&dialog);
		edit_xNum->setFixedWidth(80);
		layout_xNum->addWidget(label_xNum);
		layout_xNum->addWidget(edit_xNum);

		QLabel* label_yNum = new QLabel(QString(u8"宽"), &dialog);
		QLineEdit* edit_yNum = new QLineEdit(&dialog);
		edit_yNum->setFixedWidth(80);
		layout_yNum->addWidget(label_yNum);
		layout_yNum->addWidget(edit_yNum);

		QLabel* label_mineNum = new QLabel(QString(u8"雷数"), &dialog);
		QLineEdit* edit_mineNum = new QLineEdit(&dialog);
		edit_mineNum->setFixedWidth(80);
		layout_mineNum->addWidget(label_mineNum);
		layout_mineNum->addWidget(edit_mineNum);

		dialog.exec();

		if (dialog.result() == QDialog::Rejected) {
			dialog.close();
		}
		else if (dialog.result() == QDialog::Accepted) {
			resetMineField(edit_xNum->text().toInt(), edit_yNum->text().toInt(), edit_mineNum->text().toInt());
		}
	}

}

void mainWindow::getGameOver(bool _lifeStatus)
{
	QMessageBox msg;
	QPushButton* restore = msg.addButton(QString(u8"重来"), QMessageBox::AcceptRole);
	QPushButton* showMineField = msg.addButton(QString(u8"展示雷区"), QMessageBox::AcceptRole);
	QPushButton* newGame = msg.addButton(QString(u8"新游戏"), QMessageBox::AcceptRole);

	if (_lifeStatus) {
        msg.setWindowTitle(QString(u8"阔以"));
		msg.setText(QString(u8"你hin强👍"));
		msg.exec();
	}
	else {
        msg.setWindowTitle(QString(u8"哦豁"));
		msg.setText(QString(u8"你娃头洗白了👍"));
		msg.exec();

	}

	if (msg.clickedButton() == restore) {
		field->resotre();
	}
	else if (msg.clickedButton() == showMineField) {
		field->showMines();
	}
	else if (msg.clickedButton() == newGame) {
		resetMineField(field->getXNum(), field->getYNum(), field->getMineNum());
		//emit createNewGame(field->getXNum(), field->getYNum(), field->getMineNum());
	}
}
