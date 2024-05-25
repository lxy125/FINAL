#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onSetGrammar();
    void onInitGrammar();
    void onReadVnAndVt();
    void onConverge();
    void onConstructStatusSet();

private:
    QTextEdit* grammarInputTextEdit;
    QTextEdit* initGrammarTextEdit;
    QTextEdit* readVnAndVtTextEdit;
    QTextEdit* convergeTextEdit;
    QTextEdit* constructStatusSetTextEdit;
    QPushButton* setGrammarButton;
    QPushButton* initGrammarButton;
    QPushButton* readVnAndVtButton;
    QPushButton* convergeButton;
    QPushButton* constructStatusSetButton;
};

#endif // MAINWINDOW_H
