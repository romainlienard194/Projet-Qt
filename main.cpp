#include <QtCore/QCoreApplication>

#include "test.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	ChatServer server(1234);

	return a.exec();
}