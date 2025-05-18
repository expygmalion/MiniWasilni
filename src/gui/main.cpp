#include "../../include/gui/MainWindow.hpp"
#include <QApplication>
#include <QMessageBox>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Check for GraphViz installation
    QProcess process;
    process.start("dot", QStringList() << "-V");
    if (!process.waitForStarted() || !process.waitForFinished()) {
        QMessageBox::critical(nullptr, "Missing Dependency", 
                             "GraphViz 'dot' command not found in the system PATH.\n"
                             "Please install GraphViz to use the graph visualization features.");
        std::cerr << "GraphViz 'dot' command not found. Please install GraphViz." << std::endl;
        return 1;
    }
    
    MainWindow window;
    window.show();
    
    return app.exec();
} 