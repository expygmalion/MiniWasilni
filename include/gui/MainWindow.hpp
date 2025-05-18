#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QProcess>
#include <QTemporaryFile>
#include <QTimer>
#include <QString>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSlider>

#include "../Graph.hpp"
#include "../IOManager.hpp"
#include "../Traversal.hpp"
#include "../Dijkstra.hpp"
#include "../AStar.hpp"

class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void saveGraph();
    void loadGraph();
    void exportToSVG();
    
    // City operations
    void addCity();
    void deleteCity();
    
    // Edge operations
    void addEdge();
    void deleteEdge();
    
    // Traversal operations
    void performDFS();
    void performBFS();
    
    // Pathfinding operations
    void findDijkstraPath();
    void findAStarPath();
    
private:
    // Core components
    Graph graph;
    IOManager ioManager;
    
    // UI Components - Main layout
    QSplitter *mainSplitter;
    QWidget *displayPanel;
    QWidget *optionsPanel;
    QTextEdit *logPanel;
    
    // Graphics components for graph visualization
    QGraphicsScene *graphScene;
    QGraphicsView *graphView;
    
    // City management group
    QLineEdit *cityNameInput;
    QPushButton *addCityButton;
    QPushButton *deleteCityButton;
    
    // Edge management group
    QLineEdit *fromCityInput;
    QLineEdit *toCityInput;
    QLineEdit *distanceInput;
    QPushButton *addEdgeButton;
    QPushButton *deleteEdgeButton;
    
    // Traversal group
    QLineEdit *startCityInput;
    QPushButton *dfsButton;
    QPushButton *bfsButton;
    
    // Pathfinding group
    QLineEdit *pathFromInput;
    QLineEdit *pathToInput;
    QPushButton *dijkstraButton;
    QPushButton *aStarButton;
    
    // Menu components
    QAction *saveAction;
    QAction *loadAction;
    QAction *exportAction;
    
    // Methods
    void setupUI();
    void createMenuBar();
    void createDisplayPanel();
    void createOptionsPanel();
    void createLogPanel();
    
    void updateGraphDisplay();
    void updateGraphDisplay(const QString &highlightPath);
    
    void logMessage(const QString &message);
    
    // Graph visualization helpers
    void calculateForceDirectedLayout(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                                    QMap<QString, QPointF>& nodePositions);
    void drawGraphEdges(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                       const QMap<QString, QPointF>& nodePositions);
    void drawGraphNodes(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                       const QMap<QString, QPointF>& nodePositions);
    void drawCurvedArrow(QGraphicsScene *scene, QPointF from, QPointF to, 
                        const QString &label, QColor color = Qt::black, qreal penWidth = 1.0,
                        bool highlighted = false, bool hasBidirectional = false);
    
    // Generate DOT file for export
    QString generateDotFile();
    QString generateDotFileWithHighlight(const QStringList &highlightPath);
    bool renderDotToSvg(const QString &dotFilePath, const QString &svgFilePath);
};

#endif // MAINWINDOW_HPP 