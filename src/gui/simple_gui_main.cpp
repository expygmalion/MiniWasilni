#include "SimpleMainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    SimpleMainWindow window;
    window.show();
    
    return app.exec();
} 