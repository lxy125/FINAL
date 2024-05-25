#include "mainwindow.h"
#include "globals.h"
#include "functions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    grammarInputTextEdit = new QTextEdit(this);
    initGrammarTextEdit = new QTextEdit(this);
    readVnAndVtTextEdit = new QTextEdit(this);
    convergeTextEdit = new QTextEdit(this);
    constructStatusSetTextEdit = new QTextEdit(this);

    setGrammarButton = new QPushButton("Set Grammar", this);
    initGrammarButton = new QPushButton("Init Grammar", this);
    readVnAndVtButton = new QPushButton("Read VN and VT", this);
    convergeButton = new QPushButton("Converge", this);
    constructStatusSetButton = new QPushButton("Construct Status Set", this);

    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(setGrammarButton);
    buttonLayout->addWidget(initGrammarButton);
    buttonLayout->addWidget(readVnAndVtButton);
    buttonLayout->addWidget(convergeButton);
    buttonLayout->addWidget(constructStatusSetButton);

    QVBoxLayout* textEditLayout = new QVBoxLayout;
    textEditLayout->addWidget(grammarInputTextEdit);
    textEditLayout->addWidget(initGrammarTextEdit);
    textEditLayout->addWidget(readVnAndVtTextEdit);
    textEditLayout->addWidget(convergeTextEdit);
    textEditLayout->addWidget(constructStatusSetTextEdit);

    layout->addLayout(buttonLayout);
    layout->addLayout(textEditLayout);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(setGrammarButton, &QPushButton::clicked, this, &MainWindow::onSetGrammar);
    connect(initGrammarButton, &QPushButton::clicked, this, &MainWindow::onInitGrammar);
    connect(readVnAndVtButton, &QPushButton::clicked, this, &MainWindow::onReadVnAndVt);
    connect(convergeButton, &QPushButton::clicked, this, &MainWindow::onConverge);
    connect(constructStatusSetButton, &QPushButton::clicked, this, &MainWindow::onConstructStatusSet);
}

// Slot implementations
void MainWindow::onSetGrammar() {
    grammarInput = grammarInputTextEdit->toPlainText();
    initGrammarTextEdit->clear();
    readVnAndVtTextEdit->clear();
    convergeTextEdit->clear();
    constructStatusSetTextEdit->clear();
    initGrammarTextEdit->append(grammarInput);
}

void MainWindow::onInitGrammar() {
    initGrammar(initGrammarTextEdit);
}

void MainWindow::onReadVnAndVt() {
    readVnAndVt(readVnAndVtTextEdit);
}

void MainWindow::onConverge() {
    converge(convergeTextEdit);
}

void MainWindow::onConstructStatusSet() {
    constructStatusSet(constructStatusSetTextEdit);
}

MainWindow::~MainWindow()
{
}
