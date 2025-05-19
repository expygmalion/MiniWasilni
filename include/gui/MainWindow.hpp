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
#include <QGraphicsEllipseItem>
#include <QSlider>
#include <QGraphicsSceneMouseEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QApplication>
#include <QRandomGenerator>

#include "../Graph.hpp"
#include "../IOManager.hpp"
#include "../Traversal.hpp"
#include "../Dijkstra.hpp"
#include "../AStar.hpp"

class QGraphicsScene;
class QGraphicsView;

// Custom class for interactive draggable nodes
class DraggableNode : public QGraphicsEllipseItem {
public:
    DraggableNode(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    
    // Get and set node data
    QString getCityName() const { return cityName; }
    void setCityName(const QString &name) { cityName = name; }
    
    // Enable interactive features
    void setInteractive(bool interactive) { isInteractive = interactive; }
    
protected:
    // Override mouse event handlers for dragging
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    // Hover events for visual feedback
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    
private:
    QString cityName;
    bool isDragging;
    bool isInteractive;
    QPointF dragStartPosition;
    QBrush originalBrush;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Override event handlers
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

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
    void visualizeSpanningTree(const vector<string>& path, const vector<pair<string, string>>& treeEdges, bool isDFS);
    
    // Pathfinding operations
    void findDijkstraPath();
    void findAStarPath();
    
    // Node interaction handling
    void handleNodeDragFinished();
    
    // Layout operations
    void performAutoLayout();
    
    // Theme switching
    void applyDarkMode(bool dark);
    
private:
    // Core components
    Graph graph;
    IOManager ioManager;
    
    // Theme tracking
    bool isDarkMode;
    
    // UI Components - Main layout
    QSplitter *mainSplitter;
    QWidget *displayPanel;
    QWidget *optionsPanel;
    QTextEdit *logPanel;
    
    // Graphics components for graph visualization
    QGraphicsScene *graphScene;
    QGraphicsView *graphView;
    
    // View control components
    QPushButton *autoLayoutButton;
    QPushButton *resetViewButton;
    
    // Node tracking
    QMap<QString, DraggableNode*> nodeItems;
    
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
    void updateEdgesForNode(const QString &nodeName);
    
    void logMessage(const QString &message);
    
    // Graph visualization helpers
    void calculateForceDirectedLayout(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                                    QMap<QString, QPointF>& nodePositions);
                                    
    // New layout methods
    void placeInCircle(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                      QMap<QString, QPointF>& nodePositions, double radius);
                      
    void placeInClusteredLayout(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                              QMap<QString, QPointF>& nodePositions,
                              const QMap<QString, int>& nodeDegrees, 
                              const QMap<QString, vector<QString>>& connectedNodes);
                              
    void placeWithForceDirected(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                              QMap<QString, QPointF>& nodePositions,
                              const QMap<QString, int>& nodeDegrees,
                              int minWeight, int maxWeight);
                                  
    void placeWithHierarchicalLayout(const unordered_map<string, vector<pair<string, int>>>& adjList,
                                   QMap<QString, QPointF>& nodePositions,
                                   const QMap<QString, int>& nodeDegrees);
                                   
    void resolveCollisions(QMap<QString, QPointF>& nodePositions, qreal minDistance);
                                   
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
    
    // New spanning tree visualization methods
    void drawSpanningTree(const unordered_map<string, vector<pair<string, int>>>& adjList,
                        const vector<pair<string, string>>& treeEdges,
                        bool isDFS);
};

#endif // MAINWINDOW_HPP 