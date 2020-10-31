#include <QtWidgets/QApplication>
#include "mainWindow.h"
#include "blockWidget.h"
#include "mineField.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	mainWindow b;
	b.show();

	return a.exec();
}
