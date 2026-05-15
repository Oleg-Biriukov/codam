# Fly-In

*This project has been created as part of the 42 curriculum by obirukov.*

## Description

**Fly-In** is a sophisticated pathfinding and drone management system that solves the problem of efficiently routing multiple autonomous drones between interconnected hubs in a dynamic network environment. The project demonstrates advanced algorithmic problem-solving through the implementation of pathfinding combined with real-time visualization.

### Goal

The primary objective of Fly-In is to:
- Calculate optimal routes for multiple drones from a start hub to an end hub through intermediate hubs
- Manage concurrent drone movements while respecting hub capacity constraints and network link limitations
- Provide real-time visual feedback showing the drone navigation process
- Handle various zone types and priority levels for intelligent route optimization

### Overview

Fly-In consists of several key components:
- **Configuration System**: Parses and validates complex network configurations defined in text files
- **Pathfinding Engine**: Implements the Dijkstra's algorithm with multi-objective optimization
- **Drone Management**: Tracks and manages the state of multiple autonomous drones
- **Visual Representation**: Real-time pygame-based 3D-like visualization with interactive controls
- **Zone System**: Supports different zone types (normal, blocked, restricted, priority) that affect routing decisions

## Instructions

### Installation

1. **Clone or setup the project directory**:
   ```bash
   cd fly_in
   ```

2. **Create and activate a Python virtual environment**:
   ```bash
   python3 -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

3. **Install dependencies**:
   ```bash
   make install
   ```
   
   Or manually:
   ```bash
   pip install -r requirements.txt
   ```

### Compilation & Execution

**Running the application**:
```bash
make run config.txt
```

Or with a specific configuration file:
```bash
make run <config_filename>
```

**Direct execution with Python**:
```bash
python3 __main__.py config.txt
```

### Configuration File Format

Configuration files define the network topology and drone parameters. Example structure:

```
nb_drones: <number>
start_hub: <name> <x> <y> [zone=<type> color=<color> max_drones=<num>]
end_hub: <name> <x> <y> [zone=<type> color=<color> max_drones=<num>]
hub: <name> <x> <y> [zone=<type> color=<color> max_drones=<num>]
connection: <hub1>-<hub2> [max_link_capacity=<num>]
```

**Zone Types**:
- `normal`: Standard hub with no special properties
- `blocked`: Inaccessible hub (drones cannot pass through)
- `restricted`: Requires 2 turns to traverse
- `priority`: Preferred routing target (lower cost in Dijkstra's)

**Parameters**:
- `zone`: Hub type classification
- `color`: CSS3 color name for visualization
- `max_drones`: Maximum concurrent drones in hub
- `max_link_capacity`: Maximum concurrent drones using a connection

### Development Tools

**Run linting and type checking**:
```bash
make lint
```

**Clean cache files**:
```bash
make clean
```

### Interactive Controls

Once the visualization window is open:
- **WASD**: Pan camera (W=up, A=left, S=down, D=right)
- **Mouse Wheel / R3**: Zoom in/out
- **SPACE**: Start the simulation
- **+/-**: Adjust drone movement speed
- **ESC**: Close error message for unsolvable configuration

## Algorithm Choices and Implementation Strategy

### Dijkstra's Pathfinding Algorithm

**Why Dijkstra?**
- Optimal: Guaranteed to find the shortest path if one exists
- Reliable: Well-established algorithm with predictable behavior
- Flexible: Easily incorporates multiple constraints and zone types
- No heuristic needed: Works well in complex constraint environments

**Implementation Details**:

1. **Cost Function**: 
   - Base cost: 1 for normal zones
   - Restricted zone cost: 2 (double traversal penalty)
   - Priority zones: Cost 1 with priority boost in heap

2. **Priority Queue Strategy**:
   - Priority 0 for priority zones (preferred routing)
   - Priority 1 for normal zones
   - Ensures priority zones are explored with preference

3. **Constraint Handling**:
   - Blocked zones are excluded during expansion
   - Hub capacity is validated before adding to open list
   - Link capacity is checked dynamically against drone routes
   - Collision avoidance: Routes of other drones are considered to prevent capacity violations

4. **Data Structure**:
   - Priority queue (heapq) for efficient node selection
   - Closed list for explored nodes
   - Parent pointers for route reconstruction
   - Per-node _g value tracking minimum cost to reach each node

### Multi-Drone Coordination

The system handles multiple drones through:
- **Independent route calculation**: Each drone gets a personalized route
- **Capacity validation**: Before committing a route, the system verifies no hub/link capacity violations occur
- **Conflict detection**: Routes of concurrent drones are analyzed to prevent bottlenecks
- **Dynamic re-routing**: If no valid path exists, attempts are repeated with increased costs

### Configuration Parsing Strategy

The ConfigCompiler validates and processes network definitions with:
- **Line-by-line parsing**: Robust error reporting with line numbers
- **Metadata validation**: CSS3 colors, integer constraints, zone types
- **Connection verification**: Ensures referenced hubs exist
- **Ordering enforcement**: Drone count must be defined first, start_hub before other hubs

### Performance Optimizations

1. **Lazy reset**: Hub state (_g values, parents) reset between routes, not before entire computation
2. **Early termination**: Dijkstra's stops immediately upon reaching end_hub
3. **Constraint filtering**: Invalid neighbors pruned before heap insertion
4. **Type validation**: Pydantic models prevent invalid state

## Visual Representation Features

### 3D Isometric-like Perspective

The visualization uses a camera system with translation and zoom to create depth:
- **World coordinates**: Large virtual world (±5000 units)
- **Screen scaling**: Dynamic scaling based on zoom level (0.2x to 2x)
- **Layering**: Connections drawn first, then hubs, then drones (proper depth sorting)

### Real-Time Elements

1. **Hub Representation**:
   - Color-coded by metadata (CSS3 colors)
   - Start/end hubs use distinct sprite sheets
   - Size scales with zoom level
   - Display drone count when multiple drones occupy same hub

2. **Connections**:
   - Orange lines representing network links
   - Line thickness scales with zoom
   - Visual capacity indicators (decreases as drones traverse)

3. **Drone Visualization**:
   - Smooth animation between hubs using normalized velocity
   - Sprite-based rendering with proper scaling
   - Position updates based on frame delta time (DT) for frame-rate independence

4. **Information Display**:
   - Turn counter (top-right)
   - Control guide (top-left)
   - Error messages centered on screen
   - Drone state logged to console (e.g., "D0-hub_a D1-hub_b")

### Camera System

- **Pan**: WASD keys for smooth scrolling
- **Zoom**: Mouse wheel or R3 button (maintains 0.2-2x range)
- **Adaptive rendering**: Scale factors change based on zoom level for visual clarity

### Performance Enhancements for Visualization

- **Frame-rate independent movement**: Uses delta time (dt) to ensure consistent drone speeds
- **Sprite caching**: Pre-colored hub/dron sprites stored after first render
- **Background cycling**: Animated background with 60 frames
- **Overlay rendering**: Semi-transparent overlay for visual clarity without performance impact

## Resources

### Documentation and References

1. **Dijkstra's Algorithm**:
   - [Dijkstra's Algorithm - Wikipedia](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
   - [Dijkstra's Shortest Path Algorithm](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/)

2. **Game Development & Visualization**:
   - [Pygame Documentation](https://www.pygame.org/docs/)
   - [Camera Systems in Games](https://gameprogrammingpatterns.com/camera.html)

3. **Graph Algorithms**:
   - [Network Flow and Capacity Constraints](https://en.wikipedia.org/wiki/Flow_network)
   - [Multi-agent Pathfinding Overview](https://en.wikipedia.org/wiki/Path_planning)

4. **Python Development**:
   - [Pydantic Validation](https://docs.pydantic.dev/)
   - [Type Hints in Python](https://docs.python.org/3/library/typing.html)

### How AI Was Used

**AI was utilized for specific technical and design tasks**:

1. **Algorithm Optimization**: AI provided guidance on Dijkstra's implementation patterns with priority queues for the multi-constraint pathfinding problem

2. **Validation and Type Safety**: AI recommended Pydantic models for configuration validation and type hints for static analysis

3. **Visualization Pipeline**: AI provided insights on sprite rendering optimization, camera mathematics, and pygame event handling patterns

4. **Error Handling**: AI contributed to comprehensive error messages with line numbers for configuration file validation

**Specific project parts**:
- Dijkstra's algorithm implementation and constraint checking
- Pygame rendering pipeline and camera system
- Pydantic model definitions for type safety
- Error message formatting and line-number tracking

## Project Structure

```
fly_in/
├── __main__.py                 # Entry point
├── ConfigCompiler/
│   └── ConfigCompiler.py       # Configuration file parser
├── Engine/
│   ├── engine.py               # Main game engine and visualization
│   └── strategies.py           # pathfinding strategy
├── hubs/
│   └── hub.py                  # Hub and Dron models
├── DataPrompts.py              # Enums and error classes
├── assets/                     # Sprites and fonts
├── requirements.txt            # Python dependencies
├── Makefile                    # Build and execution commands
└── config.txt                  # Example configuration file
```

## Technical Stack

- **Language**: Python 3.10+
- **Pathfinding**: Dijkstra's algorithm with priority queue
- **Visualization**: Pygame 2.6.1+
- **Validation**: Pydantic 2.0+
- **Code Quality**: mypy (type checking), flake8 (linting)

---

**Author**: obirukov  
**42 School Project**
