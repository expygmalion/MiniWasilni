#include "../../include/gui/MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <QSplitter>
#include <QTemporaryDir>
#include <QProcess>
#include <QTextStream>
#include <QFile>
#include <QScrollArea>
#include <QRegularExpression>
#include <QSet>
#include <QSizePolicy>
#include <QPainter>
#include <QSvgRenderer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QMap>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MiniWasilni - Graph Visualization");
    resize(1200, 800);
    
    setupUI();
    createMenuBar();
    createDisplayPanel();
    createOptionsPanel();
    createLogPanel();
    
    // Initial update
    updateGraphDisplay();
    
    logMessage("Welcome to MiniWasilni Graph Visualization GUI");
}

MainWindow::~MainWindow() {
    // Any cleanup code goes here
}

void MainWindow::setupUI() {
    mainSplitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(mainSplitter);
    
    // Create the display panel (left side)
    displayPanel = new QWidget();
    
    // Create the options panel (right side)
    optionsPanel = new QWidget();
    optionsPanel->setMinimumWidth(300); // Ensure minimum width for controls
    
    // Add widgets to the splitter
    mainSplitter->addWidget(displayPanel);
    mainSplitter->addWidget(optionsPanel);
    
    // Set the initial sizes of the splitter to 70/30 ratio
    QList<int> sizes;
    int totalWidth = 1000; // Example total width
    sizes << int(totalWidth * 0.7) << int(totalWidth * 0.3);  // 70% left, 30% right
    mainSplitter->setSizes(sizes);
}

void MainWindow::createMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu("File");
    
    saveAction = new QAction("Save Graph", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveGraph);
    fileMenu->addAction(saveAction);
    
    loadAction = new QAction("Load Graph", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadGraph);
    fileMenu->addAction(loadAction);
    
    fileMenu->addSeparator();
    
    exportAction = new QAction("Export to SVG", this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportToSVG);
    fileMenu->addAction(exportAction);
}

void MainWindow::createDisplayPanel() {
    QVBoxLayout *displayLayout = new QVBoxLayout(displayPanel);
    
    // Create graphics scene and view for graph display
    graphScene = new QGraphicsScene(this);
    graphView = new QGraphicsView(graphScene);
    graphView->setRenderHint(QPainter::Antialiasing);
    graphView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphView->setMinimumSize(600, 400);
    
    // Add zoom controls
    QSlider *zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setMinimum(1);
    zoomSlider->setMaximum(100);
    zoomSlider->setValue(50);
    connect(zoomSlider, &QSlider::valueChanged, [this](int value) {
        qreal scale = value / 50.0;
        QTransform transform;
        transform.scale(scale, scale);
        graphView->setTransform(transform);
    });
    
    displayLayout->addWidget(graphView);
    displayLayout->addWidget(new QLabel("Zoom:"));
    displayLayout->addWidget(zoomSlider);
    
    displayPanel->setLayout(displayLayout);
}

void MainWindow::createOptionsPanel() {
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsPanel);
    
    // 1. City Management Group
    QGroupBox *cityGroup = new QGroupBox("City Management");
    QVBoxLayout *cityLayout = new QVBoxLayout();
    
    cityNameInput = new QLineEdit();
    cityNameInput->setPlaceholderText("Enter city name");
    
    QHBoxLayout *cityButtonLayout = new QHBoxLayout();
    addCityButton = new QPushButton("Add City");
    deleteCityButton = new QPushButton("Delete City");
    cityButtonLayout->addWidget(addCityButton);
    cityButtonLayout->addWidget(deleteCityButton);
    
    cityLayout->addWidget(cityNameInput);
    cityLayout->addLayout(cityButtonLayout);
    cityGroup->setLayout(cityLayout);
    
    // 2. Edge Management Group
    QGroupBox *edgeGroup = new QGroupBox("Edge Management");
    QVBoxLayout *edgeLayout = new QVBoxLayout();
    
    QHBoxLayout *edgeCityLayout = new QHBoxLayout();
    QVBoxLayout *fromLayout = new QVBoxLayout();
    QVBoxLayout *toLayout = new QVBoxLayout();
    
    QLabel *fromLabel = new QLabel("From:");
    fromCityInput = new QLineEdit();
    fromLayout->addWidget(fromLabel);
    fromLayout->addWidget(fromCityInput);
    
    QLabel *toLabel = new QLabel("To:");
    toCityInput = new QLineEdit();
    toLayout->addWidget(toLabel);
    toLayout->addWidget(toCityInput);
    
    edgeCityLayout->addLayout(fromLayout);
    edgeCityLayout->addLayout(toLayout);
    
    QLabel *distanceLabel = new QLabel("Distance:");
    distanceInput = new QLineEdit();
    distanceInput->setPlaceholderText("Enter distance (numeric)");
    
    QHBoxLayout *edgeButtonLayout = new QHBoxLayout();
    addEdgeButton = new QPushButton("Add Edge");
    deleteEdgeButton = new QPushButton("Delete Edge");
    edgeButtonLayout->addWidget(addEdgeButton);
    edgeButtonLayout->addWidget(deleteEdgeButton);
    
    edgeLayout->addLayout(edgeCityLayout);
    edgeLayout->addWidget(distanceLabel);
    edgeLayout->addWidget(distanceInput);
    edgeLayout->addLayout(edgeButtonLayout);
    edgeGroup->setLayout(edgeLayout);
    
    // 3. Traversal Group
    QGroupBox *traversalGroup = new QGroupBox("Graph Traversal");
    QVBoxLayout *traversalLayout = new QVBoxLayout();
    
    QLabel *startLabel = new QLabel("Starting City:");
    startCityInput = new QLineEdit();
    
    QHBoxLayout *traversalButtonLayout = new QHBoxLayout();
    dfsButton = new QPushButton("DFS Traverse");
    bfsButton = new QPushButton("BFS Traverse");
    traversalButtonLayout->addWidget(dfsButton);
    traversalButtonLayout->addWidget(bfsButton);
    
    traversalLayout->addWidget(startLabel);
    traversalLayout->addWidget(startCityInput);
    traversalLayout->addLayout(traversalButtonLayout);
    traversalGroup->setLayout(traversalLayout);
    
    // 4. Pathfinding Group
    QGroupBox *pathfindingGroup = new QGroupBox("Pathfinding");
    QVBoxLayout *pathfindingLayout = new QVBoxLayout();
    
    QHBoxLayout *pathCityLayout = new QHBoxLayout();
    QVBoxLayout *pathFromLayout = new QVBoxLayout();
    QVBoxLayout *pathToLayout = new QVBoxLayout();
    
    QLabel *pathFromLabel = new QLabel("From:");
    pathFromInput = new QLineEdit();
    pathFromLayout->addWidget(pathFromLabel);
    pathFromLayout->addWidget(pathFromInput);
    
    QLabel *pathToLabel = new QLabel("To:");
    pathToInput = new QLineEdit();
    pathToLayout->addWidget(pathToLabel);
    pathToLayout->addWidget(pathToInput);
    
    pathCityLayout->addLayout(pathFromLayout);
    pathCityLayout->addLayout(pathToLayout);
    
    QHBoxLayout *pathfindingButtonLayout = new QHBoxLayout();
    dijkstraButton = new QPushButton("Dijkstra");
    aStarButton = new QPushButton("A* Pathfinding");
    pathfindingButtonLayout->addWidget(dijkstraButton);
    pathfindingButtonLayout->addWidget(aStarButton);
    
    pathfindingLayout->addLayout(pathCityLayout);
    pathfindingLayout->addLayout(pathfindingButtonLayout);
    pathfindingGroup->setLayout(pathfindingLayout);
    
    // Add all groups to the options panel
    optionsLayout->addWidget(cityGroup);
    optionsLayout->addWidget(edgeGroup);
    optionsLayout->addWidget(traversalGroup);
    optionsLayout->addWidget(pathfindingGroup);
    optionsLayout->addStretch();
    
    // Connect signals to slots
    connect(addCityButton, &QPushButton::clicked, this, &MainWindow::addCity);
    connect(deleteCityButton, &QPushButton::clicked, this, &MainWindow::deleteCity);
    connect(addEdgeButton, &QPushButton::clicked, this, &MainWindow::addEdge);
    connect(deleteEdgeButton, &QPushButton::clicked, this, &MainWindow::deleteEdge);
    connect(dfsButton, &QPushButton::clicked, this, &MainWindow::performDFS);
    connect(bfsButton, &QPushButton::clicked, this, &MainWindow::performBFS);
    connect(dijkstraButton, &QPushButton::clicked, this, &MainWindow::findDijkstraPath);
    connect(aStarButton, &QPushButton::clicked, this, &MainWindow::findAStarPath);
}

void MainWindow::createLogPanel() {
    // Create a horizontal splitter for the main area and log panel
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical);
    verticalSplitter->addWidget(mainSplitter);
    
    // Create log panel
    logPanel = new QTextEdit();
    logPanel->setReadOnly(true);
    logPanel->setMaximumHeight(150);
    verticalSplitter->addWidget(logPanel);
    
    // Set the splitter as the central widget
    setCentralWidget(verticalSplitter);
    
    // Set the initial sizes
    QList<int> sizes;
    sizes << 650 << 150;  // Main area larger than log panel
    verticalSplitter->setSizes(sizes);
}

void MainWindow::logMessage(const QString &message) {
    logPanel->append(message);
}

// File Operations
void MainWindow::saveGraph() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Graph", "data/", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        ioManager.saveGraph(fileName.toStdString(), graph);
        logMessage("Graph saved to " + fileName);
    }
}

void MainWindow::loadGraph() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Graph", "data/", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        ioManager.loadGraph(fileName.toStdString(), graph);
        updateGraphDisplay();
        logMessage("Graph loaded from " + fileName);
    }
}

void MainWindow::exportToSVG() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export to SVG", "", "SVG Files (*.svg)");
    if (!fileName.isEmpty()) {
        QTemporaryDir tempDir;
        if (tempDir.isValid()) {
            QString dotFilePath = tempDir.path() + "/graph.dot";
            QFile dotFile(dotFilePath);
            if (dotFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&dotFile);
                out << generateDotFile();
                dotFile.close();
                
                if (renderDotToSvg(dotFilePath, fileName)) {
                    logMessage("Graph exported to " + fileName);
                } else {
                    logMessage("Error: Failed to export graph to SVG");
                }
            }
        }
    }
}

// City Operations
void MainWindow::addCity() {
    QString cityName = cityNameInput->text().trimmed();
    if (cityName.isEmpty()) {
        logMessage("Error: City name cannot be empty");
        return;
    }
    
    graph.addCity(cityName.toStdString());
    updateGraphDisplay();
    cityNameInput->clear();
    
    // Add debug message
    logMessage("DEBUG: Graph now has " + QString::number(graph.getAdjList().size()) + " cities.");
}

void MainWindow::deleteCity() {
    QString cityName = cityNameInput->text().trimmed();
    if (cityName.isEmpty()) {
        logMessage("Error: City name cannot be empty");
        return;
    }
    
    if (!graph.cityExists(cityName.toStdString())) {
        logMessage("Error: City " + cityName + " does not exist");
        return;
    }
    
    graph.deleteCity(cityName.toStdString());
    updateGraphDisplay();
    cityNameInput->clear();
}

// Edge Operations
void MainWindow::addEdge() {
    QString fromCity = fromCityInput->text().trimmed();
    QString toCity = toCityInput->text().trimmed();
    QString distanceText = distanceInput->text().trimmed();
    
    if (fromCity.isEmpty() || toCity.isEmpty()) {
        logMessage("Error: City names cannot be empty");
        return;
    }
    
    bool ok;
    int distance = distanceText.toInt(&ok);
    if (!ok || distance < 0) {
        logMessage("Error: Distance must be a non-negative number");
        return;
    }
    
    graph.addEdge(fromCity.toStdString(), toCity.toStdString(), distance);
    updateGraphDisplay();
    
    // Clear inputs
    fromCityInput->clear();
    toCityInput->clear();
    distanceInput->clear();
}

void MainWindow::deleteEdge() {
    QString fromCity = fromCityInput->text().trimmed();
    QString toCity = toCityInput->text().trimmed();
    
    if (fromCity.isEmpty() || toCity.isEmpty()) {
        logMessage("Error: City names cannot be empty");
        return;
    }
    
    graph.deleteEdge(fromCity.toStdString(), toCity.toStdString());
    updateGraphDisplay();
    
    // Clear inputs
    fromCityInput->clear();
    toCityInput->clear();
}

// Traversal Operations
void MainWindow::performDFS() {
    QString startCity = startCityInput->text().trimmed();
    if (startCity.isEmpty()) {
        logMessage("Error: Starting city name cannot be empty");
        return;
    }
    
    if (!graph.cityExists(startCity.toStdString())) {
        logMessage("Error: City " + startCity + " does not exist");
        return;
    }
    
    // Capture the path from DFS traversal
    vector<string> path;
    Traversal::dfsWithPath(graph, startCity.toStdString(), path);
    
    // Convert path to QStringList for highlighting
    QStringList pathStringList;
    for (const auto& city : path) {
        pathStringList << QString::fromStdString(city);
    }
    
    // Update display with highlighted path
    updateGraphDisplay(pathStringList.join(","));
    logMessage("DFS traversal from " + startCity + ": " + pathStringList.join(" -> "));
}

void MainWindow::performBFS() {
    QString startCity = startCityInput->text().trimmed();
    if (startCity.isEmpty()) {
        logMessage("Error: Starting city name cannot be empty");
        return;
    }
    
    if (!graph.cityExists(startCity.toStdString())) {
        logMessage("Error: City " + startCity + " does not exist");
        return;
    }
    
    // Capture the path from BFS traversal
    vector<string> path;
    Traversal::bfsWithPath(graph, startCity.toStdString(), path);
    
    // Convert path to QStringList for highlighting
    QStringList pathStringList;
    for (const auto& city : path) {
        pathStringList << QString::fromStdString(city);
    }
    
    // Update display with highlighted path
    updateGraphDisplay(pathStringList.join(","));
    logMessage("BFS traversal from " + startCity + ": " + pathStringList.join(" -> "));
}

// Pathfinding Operations
void MainWindow::findDijkstraPath() {
    QString fromCity = pathFromInput->text().trimmed();
    QString toCity = pathToInput->text().trimmed();
    
    if (fromCity.isEmpty() || toCity.isEmpty()) {
        logMessage("Error: City names cannot be empty");
        return;
    }
    
    if (!graph.cityExists(fromCity.toStdString())) {
        logMessage("Error: City " + fromCity + " does not exist");
        return;
    }
    
    if (!graph.cityExists(toCity.toStdString())) {
        logMessage("Error: City " + toCity + " does not exist");
        return;
    }
    
    // Implement our own path finding based on Dijkstra algorithm
    const auto& adjList = graph.getAdjList();
    
    // Standardize city names
    string standardFrom = Graph::standardizeCity(fromCity.toStdString());
    string standardTo = Graph::standardizeCity(toCity.toStdString());
    
    // Initialize data structures
    unordered_map<string, int> distances;
    unordered_map<string, string> previous;
    priority_queue<pair<int, string>, 
                  vector<pair<int, string>>, 
                  greater<pair<int, string>>> pq;
    
    // Set initial distances
    for (const auto& [city, _] : adjList) {
        distances[city] = numeric_limits<int>::max();
    }
    distances[standardFrom] = 0;
    pq.push({0, standardFrom});
    
    while (!pq.empty()) {
        string current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();
        
        // If we've found a better path to current, skip
        if (currentDist > distances[current]) continue;
        
        // If we've reached the end city, we're done
        if (current == standardTo) break;
        
        // Check all neighbors
        if (adjList.find(current) != adjList.end()) {
            for (const auto& [neighbor, dist] : adjList.at(current)) {
                int newDist = distances[current] + dist;
                
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    previous[neighbor] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }
    
    // If we couldn't reach the end city
    if (distances[standardTo] == numeric_limits<int>::max()) {
        logMessage("Error: No path found from " + fromCity + " to " + toCity);
        return;
    }
    
    // Reconstruct path
    vector<string> path;
    string current = standardTo;
    
    while (current != standardFrom) {
        path.push_back(current);
        current = previous.at(current);
    }
    path.push_back(standardFrom);
    
    // Reverse path to get from->to order
    reverse(path.begin(), path.end());
    
    // Convert path to QStringList for highlighting
    QStringList pathStringList;
    for (const auto& city : path) {
        pathStringList << QString::fromStdString(city);
    }
    
    // Update display with highlighted path
    updateGraphDisplay(pathStringList.join(","));
    logMessage("Dijkstra shortest path from " + fromCity + " to " + toCity + 
               " (distance: " + QString::number(distances[standardTo]) + "): " + pathStringList.join(" -> "));
}

void MainWindow::findAStarPath() {
    QString fromCity = pathFromInput->text().trimmed();
    QString toCity = pathToInput->text().trimmed();
    
    if (fromCity.isEmpty() || toCity.isEmpty()) {
        logMessage("Error: City names cannot be empty");
        return;
    }
    
    if (!graph.cityExists(fromCity.toStdString())) {
        logMessage("Error: City " + fromCity + " does not exist");
        return;
    }
    
    if (!graph.cityExists(toCity.toStdString())) {
        logMessage("Error: City " + toCity + " does not exist");
        return;
    }
    
    // Using Dijkstra for now as A* requires heuristics 
    logMessage("A* currently using Dijkstra's algorithm as a fallback...");
    
    // Call the Dijkstra method (reusing the same code from above)
    const auto& adjList = graph.getAdjList();
    
    // Standardize city names
    string standardFrom = Graph::standardizeCity(fromCity.toStdString());
    string standardTo = Graph::standardizeCity(toCity.toStdString());
    
    // Initialize data structures
    unordered_map<string, int> distances;
    unordered_map<string, string> previous;
    priority_queue<pair<int, string>, 
                  vector<pair<int, string>>, 
                  greater<pair<int, string>>> pq;
    
    // Set initial distances
    for (const auto& [city, _] : adjList) {
        distances[city] = numeric_limits<int>::max();
    }
    distances[standardFrom] = 0;
    pq.push({0, standardFrom});
    
    while (!pq.empty()) {
        string current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();
        
        // If we've found a better path to current, skip
        if (currentDist > distances[current]) continue;
        
        // If we've reached the end city, we're done
        if (current == standardTo) break;
        
        // Check all neighbors
        if (adjList.find(current) != adjList.end()) {
            for (const auto& [neighbor, dist] : adjList.at(current)) {
                int newDist = distances[current] + dist;
                
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    previous[neighbor] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }
    
    // If we couldn't reach the end city
    if (distances[standardTo] == numeric_limits<int>::max()) {
        logMessage("Error: No path found from " + fromCity + " to " + toCity);
        return;
    }
    
    // Reconstruct path
    vector<string> path;
    string current = standardTo;
    
    while (current != standardFrom) {
        path.push_back(current);
        current = previous.at(current);
    }
    path.push_back(standardFrom);
    
    // Reverse path to get from->to order
    reverse(path.begin(), path.end());
    
    // Convert path to QStringList for highlighting
    QStringList pathStringList;
    for (const auto& city : path) {
        pathStringList << QString::fromStdString(city);
    }
    
    // Update display with highlighted path
    updateGraphDisplay(pathStringList.join(","));
    logMessage("A* shortest path from " + fromCity + " to " + toCity + 
               " (distance: " + QString::number(distances[standardTo]) + "): " + pathStringList.join(" -> "));
}

// Graph Visualization Methods
QString MainWindow::generateDotFile() {
    QString dotContent = "digraph G {\n";
    dotContent += "    node [shape=ellipse, style=filled, color=\"#4286f4\", fontcolor=white];\n";
    dotContent += "    edge [color=\"#555555\"];\n";
    
    const auto& adjList = graph.getAdjList();
    
    // Add all nodes
    for (const auto& [city, neighbors] : adjList) {
        dotContent += "    \"" + QString::fromStdString(city) + "\";\n";
    }
    
    // Add all edges
    for (const auto& [city, neighbors] : adjList) {
        for (const auto& [neighbor, distance] : neighbors) {
            dotContent += "    \"" + QString::fromStdString(city) + "\" -> \"" + 
                         QString::fromStdString(neighbor) + "\" [label=\"" + 
                         QString::number(distance) + "\"];\n";
        }
    }
    
    dotContent += "}\n";
    return dotContent;
}

QString MainWindow::generateDotFileWithHighlight(const QStringList &highlightPath) {
    QString dotContent = "digraph G {\n";
    dotContent += "    node [shape=ellipse, style=filled, color=\"#aaaaaa\", fontcolor=white];\n";
    dotContent += "    edge [color=\"#aaaaaa\", penwidth=1];\n";
    
    const auto& adjList = graph.getAdjList();
    QList<QString> pathCities = highlightPath.first().split(",");
    
    // Add all nodes
    for (const auto& [city, neighbors] : adjList) {
        QString cityStr = QString::fromStdString(city);
        if (pathCities.contains(cityStr)) {
            dotContent += "    \"" + cityStr + "\" [color=\"#ff6600\"];\n";
        } else {
            dotContent += "    \"" + cityStr + "\";\n";
        }
    }
    
    // Create a set of path edges
    QSet<QString> pathEdges;
    for (int i = 0; i < pathCities.size() - 1; i++) {
        pathEdges.insert(pathCities[i] + "->" + pathCities[i+1]);
    }
    
    // Add all edges
    for (const auto& [city, neighbors] : adjList) {
        QString cityStr = QString::fromStdString(city);
        for (const auto& [neighbor, distance] : neighbors) {
            QString neighborStr = QString::fromStdString(neighbor);
            QString edgeKey = cityStr + "->" + neighborStr;
            
            if (pathEdges.contains(edgeKey)) {
                dotContent += "    \"" + cityStr + "\" -> \"" + 
                            neighborStr + "\" [label=\"" + 
                            QString::number(distance) + "\", color=\"#ff6600\", penwidth=2];\n";
            } else {
                dotContent += "    \"" + cityStr + "\" -> \"" + 
                            neighborStr + "\" [label=\"" + 
                            QString::number(distance) + "\"];\n";
            }
        }
    }
    
    dotContent += "}\n";
    return dotContent;
}

bool MainWindow::renderDotToSvg(const QString &dotFilePath, const QString &svgFilePath) {
    // Debug: Log the dot file content
    QFile dotFileDebug(dotFilePath);
    if (dotFileDebug.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString dotContent = QString::fromUtf8(dotFileDebug.readAll());
        logMessage("DEBUG: DOT file content:\n" + dotContent);
        dotFileDebug.close();
    }

    // Run the GraphViz process with more detailed logging
    QProcess process;
    logMessage("DEBUG: Starting GraphViz dot process...");
    process.start("dot", QStringList() << "-Tsvg" << dotFilePath << "-o" << svgFilePath);
    
    if (!process.waitForStarted(3000)) { // 3 second timeout
        logMessage("Error: Failed to start GraphViz dot process. Is GraphViz installed?");
        return false;
    }
    
    if (!process.waitForFinished(5000)) { // 5 second timeout
        logMessage("Error: GraphViz process failed to complete within timeout");
        process.kill();
        return false;
    }
    
    QString stdErr = QString::fromUtf8(process.readAllStandardError());
    if (!stdErr.isEmpty()) {
        logMessage("DEBUG: GraphViz stderr: " + stdErr);
    }
    
    bool success = (process.exitCode() == 0);
    logMessage("DEBUG: GraphViz process " + QString(success ? "succeeded" : "failed") + 
               " with exit code " + QString::number(process.exitCode()));
    
    // Verify SVG file was created
    QFileInfo svgInfo(svgFilePath);
    if (success && svgInfo.exists() && svgInfo.size() > 0) {
        logMessage("DEBUG: SVG file created successfully: " + svgFilePath + 
                   " (Size: " + QString::number(svgInfo.size()) + " bytes)");
    } else {
        logMessage("Error: SVG file was not created properly.");
        success = false;
    }
    
    return success;
}

void MainWindow::updateGraphDisplay() {
    logMessage("DEBUG: Updating graph display...");
    
    // Clear the scene
    graphScene->clear();
    
    // Get graph data
    const auto& adjList = graph.getAdjList();
    
    // If the graph is empty, just show a message
    if (adjList.empty()) {
        QGraphicsTextItem *textItem = graphScene->addText("Graph is empty. Please add some cities and edges.");
        textItem->setPos(0, 0);
        logMessage("DEBUG: Graph is empty, no rendering needed.");
        return;
    }
    
    // Calculate node positions using force-directed layout for better visualization
    QMap<QString, QPointF> nodePositions;
    calculateForceDirectedLayout(adjList, nodePositions);
    
    // First draw edges
    drawGraphEdges(adjList, nodePositions);
    
    // Then draw nodes (so they appear on top)
    drawGraphNodes(adjList, nodePositions);
    
    // Center the view and ensure all items are visible
    graphScene->setSceneRect(graphScene->itemsBoundingRect().adjusted(-50, -50, 50, 50));
    graphView->fitInView(graphScene->sceneRect(), Qt::KeepAspectRatio);
    graphView->centerOn(0, 0);
    
    logMessage("Graph display updated successfully with " + 
              QString::number(adjList.size()) + " nodes.");
}

void MainWindow::updateGraphDisplay(const QString &highlightPath) {
    // First update the standard display
    updateGraphDisplay();
    
    // If no highlight path, we're done
    if (highlightPath.isEmpty()) {
        return;
    }
    
    logMessage("DEBUG: Highlighting path: " + highlightPath);
    
    // Get the path cities
    QStringList pathCities = highlightPath.split(",");
    if (pathCities.size() <= 1) {
        return;  // Need at least two cities for a path
    }
    
    // Find all node items
    QMap<QString, QGraphicsEllipseItem*> nodeItems;
    QMap<QString, QPointF> nodePositions;
    
    for (QGraphicsItem *item : graphScene->items()) {
        QGraphicsEllipseItem *ellipse = dynamic_cast<QGraphicsEllipseItem*>(item);
        if (ellipse && ellipse->data(0).toString() == "node") {
            QString city = ellipse->data(1).toString();
            nodeItems[city] = ellipse;
            nodePositions[city] = ellipse->pos();
        }
    }
    
    // Highlight the path edges
    for (int i = 0; i < pathCities.size() - 1; i++) {
        QString fromCity = pathCities[i];
        QString toCity = pathCities[i+1];
        
        if (nodePositions.contains(fromCity) && nodePositions.contains(toCity)) {
            QPointF fromPos = nodePositions[fromCity];
            QPointF toPos = nodePositions[toCity];
            
            // Draw highlighted arrow
            drawCurvedArrow(graphScene, fromPos, toPos, "", QColor(255, 102, 0), 3.0, true);
            
            // Highlight the nodes
            if (nodeItems.contains(fromCity)) {
                // Change the node color to orange
                nodeItems[fromCity]->setBrush(QBrush(QColor(255, 102, 0)));
            }
        }
    }
    
    // Always highlight the last node in the path
    if (!pathCities.isEmpty() && nodeItems.contains(pathCities.last())) {
        nodeItems[pathCities.last()]->setBrush(QBrush(QColor(255, 102, 0)));
    }
}

void MainWindow::calculateForceDirectedLayout(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                                           QMap<QString, QPointF>& nodePositions) {
    // Initialize node positions in a circle
    int numNodes = adjList.size();
    qreal radius = 200;
    int i = 0;
    
    // Map to store nodes for quick lookup
    QMap<QString, bool> nodeMap;
    
    // First position nodes in a circle
    for (const auto& [city, _] : adjList) {
        qreal angle = 2 * M_PI * i / numNodes;
        qreal x = radius * cos(angle);
        qreal y = radius * sin(angle);
        
        QString cityStr = QString::fromStdString(city);
        nodePositions[cityStr] = QPointF(x, y);
        nodeMap[cityStr] = true;
        i++;
    }
    
    // Force-directed layout iterations
    const int ITERATIONS = 50;
    const qreal k = 50.0;  // Optimal distance
    const qreal REPULSION = 9000.0;
    const qreal ATTRACTION = 0.06;
    const qreal MAX_DELTA = 10.0;
    
    for (int iter = 0; iter < ITERATIONS; iter++) {
        // Calculate repulsive forces between all pairs of nodes
        QMap<QString, QPointF> forces;
        
        // Initialize forces to zero
        for (const auto& [city, _] : adjList) {
            forces[QString::fromStdString(city)] = QPointF(0, 0);
        }
        
        // Calculate repulsive forces
        for (const auto& [city1, _] : adjList) {
            QString c1 = QString::fromStdString(city1);
            QPointF pos1 = nodePositions[c1];
            
            for (const auto& [city2, _] : adjList) {
                if (city1 == city2) continue;
                
                QString c2 = QString::fromStdString(city2);
                QPointF pos2 = nodePositions[c2];
                
                QPointF delta = pos1 - pos2;
                qreal distance = qMax(0.1, QLineF(pos1, pos2).length());
                
                // Repulsive force is inversely proportional to distance
                qreal repulsiveForce = REPULSION / (distance * distance);
                
                // Normalize the delta
                if (distance > 0) {
                    delta = delta * repulsiveForce / distance;
                }
                
                forces[c1] += delta;
            }
        }
        
        // Calculate attractive forces along edges
        for (const auto& [city, neighbors] : adjList) {
            QString c1 = QString::fromStdString(city);
            QPointF pos1 = nodePositions[c1];
            
            for (const auto& [neighbor, _] : neighbors) {
                QString c2 = QString::fromStdString(neighbor);
                QPointF pos2 = nodePositions[c2];
                
                QPointF delta = pos1 - pos2;
                qreal distance = QLineF(pos1, pos2).length();
                
                // Attractive force is proportional to distance
                qreal attractiveForce = ATTRACTION * distance;
                
                // Normalize the delta
                if (distance > 0) {
                    delta = delta * attractiveForce / distance;
                }
                
                forces[c1] -= delta;
            }
        }
        
        // Apply forces with damping
        qreal damping = 1.0 - (iter / static_cast<qreal>(ITERATIONS));
        for (const auto& [city, _] : adjList) {
            QString c = QString::fromStdString(city);
            QPointF force = forces[c];
            
            // Limit maximum movement per step
            qreal magnitude = QLineF(QPointF(0, 0), force).length();
            if (magnitude > MAX_DELTA) {
                force = force * MAX_DELTA / magnitude;
            }
            
            nodePositions[c] += force * damping;
        }
    }
    
    // Normalize positions to be centered around origin
    QPointF center(0, 0);
    for (const auto& pos : nodePositions) {
        center += pos;
    }
    center /= nodePositions.size();
    
    for (auto it = nodePositions.begin(); it != nodePositions.end(); ++it) {
        it.value() -= center;
    }
}

void MainWindow::drawGraphEdges(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                               const QMap<QString, QPointF>& nodePositions) {
    // Track edges to avoid duplicate visualization
    QSet<QPair<QString, QString>> drawnEdges;
    
    // Draw all edges
    for (const auto& [city, neighbors] : adjList) {
        QString fromCity = QString::fromStdString(city);
        QPointF fromPos = nodePositions[fromCity];
        
        for (const auto& [neighbor, distance] : neighbors) {
            QString toCity = QString::fromStdString(neighbor);
            QPointF toPos = nodePositions[toCity];
            
            // Create a unique edge identifier
            QPair<QString, QString> edge(fromCity, toCity);
            
            // Check if we've already drawn this edge
            if (!drawnEdges.contains(edge)) {
                // Check if there's a reverse edge to create proper curves
                bool hasBidirectional = false;
                for (const auto& [c, n] : adjList) {
                    if (c == neighbor) {
                        for (const auto& [neigh, _] : n) {
                            if (neigh == city) {
                                hasBidirectional = true;
                                break;
                            }
                        }
                    }
                    if (hasBidirectional) break;
                }
                
                // Draw the arrow
                drawCurvedArrow(graphScene, fromPos, toPos, QString::number(distance), 
                               Qt::black, 1.0, false, hasBidirectional);
                
                // Mark this edge as drawn
                drawnEdges.insert(edge);
            }
        }
    }
}

void MainWindow::drawGraphNodes(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                               const QMap<QString, QPointF>& nodePositions) {
    // Draw nodes after edges so they appear on top
    for (const auto& [city, _] : adjList) {
        QString cityStr = QString::fromStdString(city);
        QPointF pos = nodePositions[cityStr];
        
        // Draw node
        QGraphicsEllipseItem *node = graphScene->addEllipse(-20, -20, 40, 40);
        node->setPos(pos);
        node->setBrush(QBrush(QColor(66, 134, 244)));
        node->setPen(QPen(Qt::black));
        node->setZValue(10); // Ensure nodes appear above edges
        
        // Store the city name as item data
        node->setData(0, "node");
        node->setData(1, cityStr);
        
        // Add label
        QGraphicsTextItem *label = graphScene->addText(cityStr);
        label->setDefaultTextColor(Qt::white);
        label->setPos(pos.x() - label->boundingRect().width()/2, 
                     pos.y() - label->boundingRect().height()/2);
        label->setZValue(11); // Ensure labels appear above nodes
    }
}

void MainWindow::drawCurvedArrow(QGraphicsScene *scene, QPointF from, QPointF to, 
                               const QString &label, QColor color, qreal penWidth,
                               bool highlighted, bool hasBidirectional) {
    // Calculate arrow properties
    QLineF line(from, to);
    qreal length = line.length();
    
    // Adjust end points to stop at the node boundaries
    qreal nodeRadius = 20;
    qreal dx = to.x() - from.x();
    qreal dy = to.y() - from.y();
    qreal scale = 1.0;
    
    if (length > 0) {
        scale = (length - nodeRadius) / length;
    }
    
    QPointF direction(dx, dy);
    
    // Calculate curve control point
    qreal curveFactor = hasBidirectional ? 0.3 : 0.15;
    QPointF perpendicular(-dy, dx);
    QPointF controlPoint = (from + to) / 2.0 + perpendicular * curveFactor;
    
    // Create path for the curved arrow
    QPainterPath path;
    path.moveTo(from);
    
    // Use quadratic curve
    path.quadTo(controlPoint, to);
    
    // Calculate point near the tip for arrowhead
    qreal t = 1.0 - nodeRadius / length;
    t = qMax(0.0, qMin(1.0, t)); // Clamp between 0 and 1
    
    // Calculate point on curve near the tip
    QPointF tipBase = from * (1-t)*(1-t) + controlPoint * 2*t*(1-t) + to * t*t;
    
    // Calculate direction vector at tip
    QPointF tipDirection = (2*(1-t)*controlPoint + 2*t*to) - (2*(1-t)*from + 2*t*controlPoint);
    qreal tipLength = QLineF(QPointF(0,0), tipDirection).length();
    if (tipLength > 0) {
        tipDirection = tipDirection / tipLength;
    }
    
    // Calculate perpendicular vector for arrowhead
    QPointF tipPerp(-tipDirection.y(), tipDirection.x());
    
    // Calculate arrowhead points
    qreal arrowSize = highlighted ? 15.0 : 10.0;
    QPointF arrowP1 = tipBase - tipDirection * arrowSize + tipPerp * arrowSize * 0.5;
    QPointF arrowP2 = tipBase - tipDirection * arrowSize - tipPerp * arrowSize * 0.5;
    
    // Draw the path
    QPen pen(color, penWidth);
    QGraphicsPathItem *pathItem = scene->addPath(path, pen);
    pathItem->setZValue(1); // Ensure below nodes
    
    // Draw arrowhead
    QPainterPath arrowPath;
    arrowPath.moveTo(tipBase);
    arrowPath.lineTo(arrowP1);
    arrowPath.lineTo(arrowP2);
    arrowPath.closeSubpath();
    
    QGraphicsPathItem *arrowItem = scene->addPath(arrowPath, pen, QBrush(color));
    arrowItem->setZValue(2);
    
    // Add distance label if provided
    if (!label.isEmpty() && !highlighted) {
        QGraphicsTextItem *textItem = scene->addText(label);
        textItem->setDefaultTextColor(color);
        
        // Position label near the control point
        QPointF textPos = controlPoint;
        textPos.rx() -= textItem->boundingRect().width() / 2;
        textPos.ry() -= textItem->boundingRect().height() / 2;
        textItem->setPos(textPos);
        textItem->setZValue(3);
    }
} 