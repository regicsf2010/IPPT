#include <QApplication>
#include "src/QTPro/ImageViewer.h"

#include <QDesktopWidget> // permite calcular o tamanho da janela

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	ImageViewer *mainWindow = new ImageViewer;
	QRect screenGeometry = QApplication::desktop()->screenGeometry();

	mainWindow->move( (screenGeometry.width() - mainWindow->width()) / 2, (screenGeometry.height() - mainWindow->height()) / 2);
	mainWindow->show(); // o default é invisible, por isso o show()

	// deixa a aplicação em estado "event looping", ou seja, a aplicação
	// continua rodando até que seja fechada
	return a.exec();
}
