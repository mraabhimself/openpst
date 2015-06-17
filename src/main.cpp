/**
* LICENSE PLACEHOLDER
*/

#include <QApplication>
#include "gui/main_window.h"
#ifdef WIN32
	#include <Windows.h>
#endif

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
	#ifdef WIN32
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	#endif

	QApplication application(argc, argv);

    MainWindow mainWindow;

    mainWindow.show();

    return application.exec();
}
