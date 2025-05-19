#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QWidget window;
    window.setWindowTitle("Simple Qt Test");
    window.resize(300, 200);
    
    QVBoxLayout *layout = new QVBoxLayout(&window);
    
    QLabel *label = new QLabel("Hello from Qt!");
    layout->addWidget(label);
    
    QPushButton *button = new QPushButton("Click Me");
    QObject::connect(button, &QPushButton::clicked, [label]() {
        label->setText("Button clicked!");
    });
    layout->addWidget(button);
    
    window.show();
    
    return app.exec();
} 