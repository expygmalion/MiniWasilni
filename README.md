# MiniWasilni

MiniWasilni is a comprehensive graph visualization and manipulation application for directed graphs, with both CLI and GUI interfaces. It's designed to efficiently create, visualize, and analyze city networks through various graph algorithms.

## Features

- Interactive graph visualization with a dynamic force-directed layout
- Add and delete cities and route connections
- Real-time graph updates with curved edge visualization
- Graph traversal algorithms (DFS and BFS)
- Path finding algorithms (Dijkstra and A*)
- Path highlighting with visual differentiation
- Save and load graph data
- Export graphs as SVG files
- Interactive zoom and pan controls
- Detailed logging of operations

## Versions

### Command Line Interface (CLI)

The original CLI version provides text-based interaction with the graph data structure, ideal for script-based operations and server environments.

### Graphical User Interface (GUI)

The new GUI version offers an intuitive interface with advanced visual representation of the graph:
- Dynamic force-directed layout for optimal graph visualization
- Curved edges with directional arrows
- Highlighted traversal and pathfinding paths
- Interactive zoom and navigation controls
- Real-time graph updates

## Dependencies

- C++17 compatible compiler
- CMake 3.10 or higher
- Qt5 (Core, Widgets, and SVG modules) for GUI version
- GraphViz (optional, for SVG export)

## Building

### Prerequisites

#### On Arch Linux/Manjaro
```bash
sudo pacman -S qt5-base qt5-svg cmake
```

#### On Ubuntu/Debian
```bash
sudo apt-get install qt5-default libqt5svg5-dev cmake
```

#### On Fedora
```bash
sudo dnf install qt5-qtbase-devel qt5-qtsvg-devel cmake
```

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
```

## Running

### CLI Version
```bash
./MiniWasilni
```

### GUI Version
```bash
./MiniWasilniGUI
```

## GUI Usage Guide

### Graph Visualization

The left panel displays an interactive graph visualization:
- Blue nodes represent cities
- Directed edges show connections between cities
- Edge labels display the distance
- Orange highlighting shows traversal or pathfinding results

### City Management

Add or delete cities using the top section of the right panel:
1. Enter a city name in the text box
2. Click "Add City" to create a new city
3. To delete a city, enter its name and click "Delete City"

### Edge Management

Create or remove connections between cities:
1. Enter the starting city in the "From" field
2. Enter the destination city in the "To" field
3. Specify the distance in the "Distance" field
4. Click "Add Edge" to create the connection
5. To remove a connection, specify the "From" and "To" cities and click "Delete Edge"

### Graph Traversal

Explore the graph using traversal algorithms:
1. Enter a starting city name
2. Click "DFS Traverse" for Depth-First Search traversal
3. Click "BFS Traverse" for Breadth-First Search traversal
4. The traversal path will be highlighted in orange

### Pathfinding

Find optimal routes between cities:
1. Enter the starting city in the "From" field
2. Enter the destination city in the "To" field
3. Click "Dijkstra" for shortest path using Dijkstra's algorithm
4. Click "A* Pathfinding" for shortest path using A* algorithm
5. The optimal path will be highlighted in orange with the total distance displayed in the log

### File Operations

Access file operations from the top menu:
- "Save Graph": Save the current graph to a text file
- "Load Graph": Load a graph from a text file
- "Export to SVG": Export the graph visualization as an SVG file

### Navigation Controls

- Use the zoom slider at the bottom of the display panel to adjust the zoom level
- Click and drag to pan around the graph
- The graph automatically rescales when nodes are added or removed

## Examples

The application comes with sample graph files in the `data/` directory:
- `cairo.txt`: A directed graph of Cairo's neighborhoods
- `khartoum.txt`: A directed graph of Khartoum's neighborhoods

## Development

MiniWasilni is built with a modular architecture:
- Graph class for core data structure
- Traversal algorithms for graph exploration
- Pathfinding algorithms for optimal routing
- IOManager for file operations
- Qt-based visualization layer for the GUI

## License

MiniWasilni is released under the MIT License. See the LICENSE file for details.