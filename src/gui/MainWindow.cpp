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
#include <QDir>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneHoverEvent>
#include <cmath>

// DraggableNode implementation
DraggableNode::DraggableNode(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x, y, width, height, parent),
      isDragging(false),
      isInteractive(true)
{
    // Enable mouse tracking and hover events
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    
    // Store original brush for hover effects
    originalBrush = brush();
    
    // Default cursor to indicate draggable
    setCursor(Qt::OpenHandCursor);
}

void DraggableNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isInteractive) {
        // Start dragging
        isDragging = true;
        dragStartPosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
        
        // Add a temporary highlight effect
        setBrush(QBrush(QColor(255, 165, 0))); // Orange highlight
        
        // Call base implementation for selection behavior
        QGraphicsEllipseItem::mousePressEvent(event);
        
        // Bring to front while dragging
        setZValue(20);
    }
}

void DraggableNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isDragging && isInteractive) {
        // Inform QGraphicsItem of the move for standard dragging
        QGraphicsEllipseItem::mouseMoveEvent(event);
        
        // Signal that the node has moved to update connected edges
        emit dynamic_cast<QObject*>(scene())->property("nodeMoving").toBool();
    }
}

void DraggableNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging) {
        // Stop dragging
        isDragging = false;
        setCursor(Qt::OpenHandCursor);
        
        // Restore standard appearance but keep selected state
        if (isSelected()) {
            setBrush(QBrush(QColor(100, 200, 255))); // Light blue for selected
        } else {
            setBrush(originalBrush);
        }
        
        // Reset the Z value
        setZValue(10);
        
        // Call base implementation
        QGraphicsEllipseItem::mouseReleaseEvent(event);
        
        // Notify the scene that dragging has finished
        QGraphicsScene *graphScene = scene();
        if (graphScene) {
            // This will be caught by MainWindow
            QMetaObject::invokeMethod(graphScene->parent(), "handleNodeDragFinished");
        }
    }
}

void DraggableNode::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (isInteractive) {
        // Store the original brush if not already stored
        if (brush() != QBrush(QColor(255, 165, 0)) && brush() != QBrush(QColor(100, 200, 255))) {
            originalBrush = brush();
        }
        
        // Apply subtle hover effect if not selected or dragging
        if (!isSelected() && !isDragging) {
            QColor hoverColor = originalBrush.color().lighter(115);
            setBrush(QBrush(hoverColor));
        }
    }
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void DraggableNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (isInteractive) {
        // Restore original appearance if not selected or dragging
        if (!isSelected() && !isDragging) {
            setBrush(originalBrush);
        }
    }
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

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
        // Check if the file has a .txt extension, if not, add it
        if (!fileName.endsWith(".txt", Qt::CaseInsensitive)) {
            fileName += ".txt";
        }
        
        // Ensure the data directory exists
        QDir dataDir("data");
        if (!dataDir.exists()) {
            QDir().mkdir("data");
            logMessage("Created data directory");
        }
        
        // Try to save the graph
        try {
            ioManager.saveGraph(fileName.toStdString(), graph);
            logMessage("Graph saved to " + fileName);
        } catch (const std::exception& e) {
            logMessage("Error saving graph: " + QString(e.what()));
            QMessageBox::critical(this, "Save Error", "Failed to save graph: " + QString(e.what()));
        }
    }
}

void MainWindow::loadGraph() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Graph", "data/", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        try {
            ioManager.loadGraph(fileName.toStdString(), graph);
            updateGraphDisplay();
            logMessage("Graph loaded from " + fileName);
        } catch (const std::exception& e) {
            logMessage("Error loading graph: " + QString(e.what()));
            QMessageBox::critical(this, "Load Error", "Failed to load graph: " + QString(e.what()));
        }
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
    
    // Clear the scene, but keep track of node positions
    QMap<QString, QPointF> nodePositions;
    
    // First, collect node positions before clearing
    for (auto it = nodeItems.begin(); it != nodeItems.end(); ++it) {
        DraggableNode* node = it.value();
        nodePositions[it.key()] = node->pos();
    }
    
    // Now clear the scene
    graphScene->clear();
    nodeItems.clear();
    
    // Get graph data
    const auto& adjList = graph.getAdjList();
    
    // If the graph is empty, just show a message
    if (adjList.empty()) {
        QGraphicsTextItem *textItem = graphScene->addText("Graph is empty. Please add some cities and edges.");
        textItem->setPos(0, 0);
        logMessage("DEBUG: Graph is empty, no rendering needed.");
        return;
    }
    
    // For first-time display, calculate initial positions using force-directed layout
    if (nodePositions.isEmpty()) {
        calculateForceDirectedLayout(adjList, nodePositions);
    }
    
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
    
    // Find all node items - we now use our nodeItems map
    QMap<QString, QPointF> nodePositions;
    
    for (auto it = nodeItems.begin(); it != nodeItems.end(); ++it) {
        nodePositions[it.key()] = it.value()->pos();
    }
    
    // Highlight the path edges
    for (int i = 0; i < pathCities.size() - 1; i++) {
        QString fromCity = pathCities[i];
        QString toCity = pathCities[i+1];
        
        if (nodePositions.contains(fromCity) && nodePositions.contains(toCity)) {
            QPointF fromPos = nodePositions[fromCity];
            QPointF toPos = nodePositions[toCity];
            
            // Draw highlighted arrow
            drawCurvedArrow(graphScene, fromPos, toPos, "", QColor(255, 102, 0), 3.0, true, false);
            
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
    qreal radius = qMin(300.0, 50.0 + numNodes * 15.0); // Dynamic radius based on node count
    int i = 0;
    
    // Map to store nodes for quick lookup
    QMap<QString, bool> nodeMap;
    
    // Map to count connection degree of nodes and store max weight
    QMap<QString, int> nodeDegrees;
    QMap<QString, int> nodeMaxWeight;
    int maxDegree = 0;
    int maxWeight = 1;
    int minWeight = INT_MAX;
    
    // First position nodes in a circle and compute node degrees
    for (const auto& [city, neighbors] : adjList) {
        QString cityStr = QString::fromStdString(city);
        
        // Count degree and find maximum edge weight for this node
        int degree = neighbors.size();
        int maxNodeWeight = 0;
        
        for (const auto& [_, weight] : neighbors) {
            maxNodeWeight = qMax(maxNodeWeight, weight);
            minWeight = qMin(minWeight, weight);
            maxWeight = qMax(maxWeight, weight);
        }
        
        nodeDegrees[cityStr] = degree;
        nodeMaxWeight[cityStr] = maxNodeWeight;
        maxDegree = qMax(maxDegree, degree);
        
        // Place node on the circle
        qreal angle = 2 * M_PI * i / numNodes;
        qreal x = radius * cos(angle);
        qreal y = radius * sin(angle);
        
        nodePositions[cityStr] = QPointF(x, y);
        nodeMap[cityStr] = true;
        i++;
    }
    
    // If there's only one weight value, avoid division by zero
    if (minWeight == maxWeight) {
        minWeight = maxWeight - 1;
    }
    
    // Force-directed layout iterations
    const int ITERATIONS = 70; // Increased iterations for better convergence
    // Optimal distance - dynamic based on node count
    const qreal k = qMin(150.0, 30.0 + numNodes * 3.0);
    const qreal REPULSION_BASE = 10000.0;
    const qreal ATTRACTION_BASE = 0.05;
    const qreal MAX_DELTA = 15.0; // Increased maximum delta for faster convergence
    
    // Weight scaling function: Map weights from [minWeight, maxWeight] to [0.5, 2.0]
    auto scaleWeight = [minWeight, maxWeight](int weight) -> qreal {
        // Invert the weight - smaller weights mean stronger attraction (shorter distance)
        // For very large weights, use a logarithmic scale
        if (maxWeight > 100) {
            return 0.5 + 1.5 * (1.0 - log(weight) / log(maxWeight));
        } else {
            return 0.5 + 1.5 * (1.0 - (weight - minWeight) / static_cast<qreal>(maxWeight - minWeight));
        }
    };
    
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
            
            // Repulsion factor scales with node degree
            qreal degreeFactor = 0.5 + 0.5 * (nodeDegrees[c1] / static_cast<qreal>(qMax(1, maxDegree)));
            qreal nodeRepulsion = REPULSION_BASE * degreeFactor;
            
            for (const auto& [city2, _] : adjList) {
                if (city1 == city2) continue;
                
                QString c2 = QString::fromStdString(city2);
                QPointF pos2 = nodePositions[c2];
                
                QPointF delta = pos1 - pos2;
                qreal distance = qMax(0.1, QLineF(pos1, pos2).length());
                
                // Repulsive force is inversely proportional to distance
                qreal repulsiveForce = nodeRepulsion / (distance * distance);
                
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
            
            for (const auto& [neighbor, weight] : neighbors) {
                QString c2 = QString::fromStdString(neighbor);
                QPointF pos2 = nodePositions[c2];
                
                QPointF delta = pos1 - pos2;
                qreal distance = QLineF(pos1, pos2).length();
                
                // Attractive force is proportional to distance and inversely proportional to weight
                // Higher weights = longer distances = weaker attraction
                qreal weightFactor = scaleWeight(weight);
                qreal attractiveForce = ATTRACTION_BASE * distance * weightFactor;
                
                // Normalize the delta
                if (distance > 0) {
                    delta = delta * attractiveForce / distance;
                }
                
                forces[c1] -= delta;
            }
        }
        
        // Apply forces with damping - more aggressive at the start, gentler at the end
        qreal damping = 1.0 - (iter / static_cast<qreal>(ITERATIONS));
        // Cubic damping curve - stays stronger longer then falls off quickly
        damping = damping * damping * (3 - 2 * damping);
        
        for (const auto& [city, _] : adjList) {
            QString c = QString::fromStdString(city);
            QPointF force = forces[c];
            
            // Apply forces with iteration-dependent limits for convergence
            qreal iterationMaxDelta = MAX_DELTA * damping + 2.0;
            
            // Limit maximum movement per step
            qreal magnitude = QLineF(QPointF(0, 0), force).length();
            if (magnitude > iterationMaxDelta) {
                force = force * iterationMaxDelta / magnitude;
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
    
    // Scale the graph to fit in the view
    qreal maxDistance = 0;
    for (auto it = nodePositions.begin(); it != nodePositions.end(); ++it) {
        maxDistance = qMax(maxDistance, QLineF(QPointF(0, 0), it.value()).length());
    }
    
    // Apply final scaling if needed to prevent graph from being too large
    qreal desiredRadius = qMin(400.0, 100.0 + numNodes * 15.0);
    if (maxDistance > desiredRadius) {
        qreal scaleFactor = desiredRadius / maxDistance;
        for (auto it = nodePositions.begin(); it != nodePositions.end(); ++it) {
            it.value() *= scaleFactor;
        }
    }
}

void MainWindow::drawGraphEdges(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                               const QMap<QString, QPointF>& nodePositions) {
    // Track edges to avoid duplicate visualization
    QSet<QPair<QString, QString>> drawnEdges;
    
    // Find maximum and minimum weight for edge thickness scaling
    int minWeight = INT_MAX;
    int maxWeight = 0;
    
    for (const auto& [_, neighbors] : adjList) {
        for (const auto& [__, weight] : neighbors) {
            minWeight = qMin(minWeight, weight);
            maxWeight = qMax(maxWeight, weight);
        }
    }
    
    // Handle case where all weights are the same
    if (minWeight == maxWeight) {
        minWeight = maxWeight - 1;
    }
    
    // Draw all edges
    for (const auto& [city, neighbors] : adjList) {
        QString fromCity = QString::fromStdString(city);
        
        // Skip if source city doesn't exist in our node positions
        if (!nodePositions.contains(fromCity)) {
            continue;
        }
        
        QPointF fromPos = nodePositions[fromCity];
        
        for (const auto& [neighbor, weight] : neighbors) {
            QString toCity = QString::fromStdString(neighbor);
            
            // Skip if target city doesn't exist in our node positions
            if (!nodePositions.contains(toCity)) {
                continue;
            }
            
            QPointF toPos = nodePositions[toCity];
            
            // Create a unique edge identifier
            QPair<QString, QString> edge(fromCity, toCity);
            
            // Check if we've already drawn this edge
            if (drawnEdges.contains(edge)) {
                continue;
            }
            
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
            
            // Calculate edge thickness based on weight
            // Thinner lines for higher weights (counterintuitive but visually clearer)
            // Scale between 0.5 and 3.0
            qreal normalizedWeight = (weight - minWeight) / qMax(1.0, static_cast<qreal>(maxWeight - minWeight));
            qreal edgeThickness = 3.0 - normalizedWeight * 2.5; // 3.0 for min weight, 0.5 for max weight
            
            // Choose edge color based on weight
            // Blend from green (low weight = good) to red (high weight = bad)
            QColor edgeColor;
            if (maxWeight > 10 * minWeight) {
                // Logarithmic scale for widely varying weights
                qreal logMin = log(minWeight);
                qreal logMax = log(maxWeight);
                qreal logWeight = log(weight);
                qreal normalizedLogWeight = (logWeight - logMin) / (logMax - logMin);
                
                int red = qMin(255, int(normalizedLogWeight * 255));
                int green = qMin(255, int((1.0 - normalizedLogWeight) * 200) + 55);
                int blue = 55;
                edgeColor = QColor(red, green, blue);
            } else {
                // Linear scale for weights in similar range
                int red = qMin(255, int(normalizedWeight * 255));
                int green = qMin(255, int((1.0 - normalizedWeight) * 200) + 55);
                int blue = 55;
                edgeColor = QColor(red, green, blue);
            }
            
            // Draw the arrow
            drawCurvedArrow(graphScene, fromPos, toPos, QString::number(weight), 
                           edgeColor, edgeThickness, false, hasBidirectional);
            
            // Apply metadata to newly created items that don't have data yet
            for (QGraphicsItem *item : graphScene->items()) {
                if (!item->data(0).isValid()) {
                    // Based on item type, set appropriate metadata
                    if (item->type() == QGraphicsPathItem::Type) {
                        // Determine if it's an edge path or arrowhead (arrowheads have a brush)
                        QGraphicsPathItem* pathItem = qgraphicsitem_cast<QGraphicsPathItem*>(item);
                        if (pathItem->brush() == Qt::NoBrush) {
                            item->setData(0, "edge");
                        } else {
                            item->setData(0, "arrowhead");
                        }
                    } else if (item->type() == QGraphicsTextItem::Type) {
                        item->setData(0, "edgeLabel");
                    } else if (item->type() == QGraphicsRectItem::Type) {
                        item->setData(0, "edgePanel");
                    }
                    
                    // Set connection data
                    item->setData(1, fromCity);
                    item->setData(2, toCity);
                    if (item->data(0).toString() == "edge") {
                        item->setData(3, weight);
                    }
                }
            }
            
            // Mark this edge as drawn
            drawnEdges.insert(edge);
        }
    }
}

void MainWindow::drawGraphNodes(const unordered_map<string, vector<pair<string, int>>>& adjList, 
                               const QMap<QString, QPointF>& nodePositions) {
    // Clear existing node map but keep references for deletion
    QMap<QString, DraggableNode*> oldNodeItems = nodeItems;
    nodeItems.clear();
    
    // Count incoming and outgoing connections for each node
    QMap<QString, int> incomingEdges;
    QMap<QString, int> outgoingEdges;
    QMap<QString, int> totalWeight;
    
    // Initialize counters
    for (const auto& [city, _] : adjList) {
        QString cityStr = QString::fromStdString(city);
        incomingEdges[cityStr] = 0;
        outgoingEdges[cityStr] = 0;
        totalWeight[cityStr] = 0;
    }
    
    // Count connections
    for (const auto& [city, neighbors] : adjList) {
        QString cityStr = QString::fromStdString(city);
        
        // Count outgoing edges
        outgoingEdges[cityStr] = neighbors.size();
        
        // Add to total weight
        for (const auto& [_, weight] : neighbors) {
            totalWeight[cityStr] += weight;
        }
        
        // Count incoming edges
        for (const auto& [neighbor, weight] : neighbors) {
            QString neighborStr = QString::fromStdString(neighbor);
            incomingEdges[neighborStr]++;
            totalWeight[neighborStr] += weight;
        }
    }
    
    // Find max connection count and weight for scaling
    int maxConnections = 0;
    int maxNodeWeight = 1;
    
    for (const auto& [city, _] : adjList) {
        QString cityStr = QString::fromStdString(city);
        int connections = incomingEdges[cityStr] + outgoingEdges[cityStr];
        maxConnections = qMax(maxConnections, connections);
        maxNodeWeight = qMax(maxNodeWeight, totalWeight[cityStr]);
    }
    
    // Draw nodes after edges so they appear on top
    for (const auto& [city, _] : adjList) {
        QString cityStr = QString::fromStdString(city);
        QPointF pos = nodePositions[cityStr];
        
        // Calculate node size based on connection count (importance)
        // Size between 15 and 40 radius
        int connections = incomingEdges[cityStr] + outgoingEdges[cityStr];
        qreal importanceFactor = 0.5 + 0.5 * sqrt(connections / qMax(1.0, static_cast<qreal>(maxConnections)));
        qreal nodeSize = 15.0 + importanceFactor * 25.0; // Radius between 15 and 40
        
        // Choose node color based on incoming/outgoing ratio
        QColor nodeColor;
        if (connections > 0) {
            // Calculate color based on in/out ratio
            qreal inRatio = incomingEdges[cityStr] / qMax(1.0, static_cast<qreal>(connections));
            
            // More incoming = more blue, more outgoing = more green
            int red = 100;
            int green = 100 + int((1.0 - inRatio) * 155); // More green if more outgoing
            int blue = 100 + int(inRatio * 155); // More blue if more incoming
            
            nodeColor = QColor(red, green, blue);
        } else {
            // Default color for isolated nodes
            nodeColor = QColor(150, 150, 150);
        }
        
        // Create draggable node instead of regular ellipse
        DraggableNode *node = new DraggableNode(-nodeSize, -nodeSize, nodeSize*2, nodeSize*2);
        node->setPos(pos);
        node->setBrush(QBrush(nodeColor));
        node->setPen(QPen(Qt::black));
        node->setZValue(10); // Ensure nodes appear above edges
        node->setCityName(cityStr);
        graphScene->addItem(node);
        
        // Store in map for later access
        nodeItems[cityStr] = node;
        
        // Add label
        QGraphicsTextItem *label = graphScene->addText(cityStr);
        label->setDefaultTextColor(Qt::white);
        
        // Make text size relative to node size
        QFont font = label->font();
        font.setPointSizeF(qMax(7.0, 8.0 + importanceFactor * 4.0)); // Font size between 8 and 12
        font.setBold(true);
        label->setFont(font);
        
        // Center the label in the node
        label->setPos(pos.x() - label->boundingRect().width()/2, 
                    pos.y() - label->boundingRect().height()/2);
        label->setZValue(11); // Ensure labels appear above nodes
        
        // Add drop shadow for better visibility
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
        effect->setOffset(1.0);
        effect->setBlurRadius(3.0);
        effect->setColor(Qt::black);
        label->setGraphicsEffect(effect);
        
        // Associate label with node for moving together
        label->setData(0, "label");
        label->setData(1, cityStr);
        
        // Set node metadata
        node->setData(0, "node");
        node->setData(1, cityStr);
    }
    
    // Delete old nodes that are no longer in use
    // Note: QGraphicsScene will handle actual deletion
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
    // Use a dynamic curve factor based on length
    qreal baseCurveFactor = hasBidirectional ? 0.3 : 0.15;
    qreal curveFactor = baseCurveFactor * qMin(1.0, length / 200.0); // Scale down for short edges
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
    
    // Calculate arrowhead points with size proportional to edge thickness
    qreal arrowSize = highlighted ? 15.0 : 8.0 + penWidth * 2.0;
    QPointF arrowP1 = tipBase - tipDirection * arrowSize + tipPerp * arrowSize * 0.5;
    QPointF arrowP2 = tipBase - tipDirection * arrowSize - tipPerp * arrowSize * 0.5;
    
    // Draw the path with improved styling
    QPen pen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    
    // Use a fancier style for highlighted paths
    if (highlighted) {
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(penWidth + 1);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
    }
    
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
    
    // Add distance label if provided with improved styling
    if (!label.isEmpty() && !highlighted) {
        QGraphicsTextItem *textItem = scene->addText(label);
        
        // Style the label
        QFont font = textItem->font();
        font.setPointSizeF(8.0);
        font.setBold(true);
        textItem->setFont(font);
        
        // Set color - use contrasting color against background
        QColor textColor;
        int colorLightness = (color.red() + color.green() + color.blue()) / 3;
        if (colorLightness > 128) {
            // Dark text for light backgrounds
            textColor = QColor(10, 10, 10);
        } else {
            // Light text for dark backgrounds
            textColor = QColor(245, 245, 245);
        }
        textItem->setDefaultTextColor(textColor);
        
        // Add a background panel for better visibility
        QRectF textRect = textItem->boundingRect();
        QGraphicsRectItem *panel = scene->addRect(textRect, Qt::NoPen, QBrush(QColor(color.red(), color.green(), color.blue(), 100)));
        
        // Position label near the midpoint but offset from the line
        QPointF midPoint = (from + to) / 2.0;
        
        // Manually normalize the perpendicular vector
        qreal perpLength = sqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());
        QPointF normalizedPerp = (perpLength > 0) ? 
                                QPointF(perpendicular.x() / perpLength, perpendicular.y() / perpLength) : 
                                QPointF(0, 0);
        
        QPointF textOffset = normalizedPerp * 15.0; // Offset from the line
        QPointF textPos = midPoint + textOffset;
        
        // Adjust position to center the label
        textPos.rx() -= textItem->boundingRect().width() / 2;
        textPos.ry() -= textItem->boundingRect().height() / 2;
        
        // Position both the panel and text
        panel->setPos(textPos);
        textItem->setPos(textPos);
        
        // Add a drop shadow effect for the text
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
        shadowEffect->setOffset(1.0);
        shadowEffect->setBlurRadius(2.0);
        shadowEffect->setColor(QColor(0, 0, 0, 160));
        textItem->setGraphicsEffect(shadowEffect);
        
        // Ensure text is above the panel
        panel->setZValue(2);
        textItem->setZValue(3);
        
        // Tag each created item with its type (used for cleanup)
        panel->setData(0, "edgePanel");
        textItem->setData(0, "edgeLabel");
    }
    
    // Tag edge items (useful for cleanup)
    pathItem->setData(0, "edge");
    arrowItem->setData(0, "arrowhead");
}

// Add new methods to handle node dragging and edge updates
void MainWindow::handleNodeDragFinished() {
    // Find which node was dragged
    for (auto it = nodeItems.begin(); it != nodeItems.end(); ++it) {
        DraggableNode* node = it.value();
        if (node->isSelected()) {
            QString cityName = node->getCityName();
            
            // Update all edges connected to this node
            updateEdgesForNode(cityName);
            
            // Also update any associated labels
            for (QGraphicsItem *item : graphScene->items()) {
                QGraphicsTextItem *textItem = dynamic_cast<QGraphicsTextItem*>(item);
                if (textItem && textItem->data(0).toString() == "label" && 
                    textItem->data(1).toString() == cityName) {
                    
                    // Reposition label to stay centered on the node
                    QPointF nodePos = node->pos();
                    textItem->setPos(nodePos.x() - textItem->boundingRect().width()/2, 
                                    nodePos.y() - textItem->boundingRect().height()/2);
                    break;
                }
            }
            
            // Log the node move
            logMessage("Moved node: " + cityName);
            break;
        }
    }
}

void MainWindow::updateEdgesForNode(const QString &nodeName) {
    // Clear ALL edges from the scene and redraw completely to avoid any duplication issues
    // This is more thorough but might be less efficient for large graphs
    
    // We'll keep track of all items to remove
    QList<QGraphicsItem*> itemsToRemove;
    
    // Find all edge-related items (any items with data attributes)
    for (QGraphicsItem *item : graphScene->items()) {
        // Check if the item has any data - if so, it's something we created
        if (item->data(0).isValid()) {
            QString dataType = item->data(0).toString();
            // If it's an edge-related item, remove it
            if (dataType == "edge" || dataType == "arrowhead" || 
                dataType == "edgeLabel" || dataType == "edgePanel") {
                itemsToRemove.append(item);
            }
        }
    }
    
    // Remove all identified items
    for (QGraphicsItem *item : itemsToRemove) {
        graphScene->removeItem(item);
        delete item;
    }
    
    // Get current positions of all nodes
    QMap<QString, QPointF> currentPositions;
    for (auto it = nodeItems.begin(); it != nodeItems.end(); ++it) {
        currentPositions[it.key()] = it.value()->pos();
    }
    
    // Redraw all edges
    drawGraphEdges(graph.getAdjList(), currentPositions);
    
    // Log the action
    logMessage("Updated edges after moving node: " + nodeName);
} 