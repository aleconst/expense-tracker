#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Expense Tracker");
    window.resize(800, 500);

    QVBoxLayout layout (&window);
    QLabel title ("Expense Tracker");
    layout.addWidget (&title);

    QLabel subtitle ("Track your daily expenses");
    layout.addWidget (&subtitle);
    
    QLineEdit description_input;
    description_input.setPlaceholderText ("Expense description");
    layout.addWidget (&description_input);

    QPushButton add_button ("Add expense");
    layout.addWidget (&add_button);

    QObject::connect (&add_button, &QPushButton::clicked, &subtitle, [&subtitle, &description_input] () {
        if (description_input.text().trimmed().isEmpty() == true)
            subtitle.setText ("No description entered.");
        else
            subtitle.setText (description_input.text().trimmed());
    });

    window.show();

    return app.exec();
}