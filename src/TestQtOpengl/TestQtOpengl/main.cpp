#include "TestQtOpengl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestQtOpengl w;
	w.show();
	return a.exec();
}
