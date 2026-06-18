# Minecraft Village Builder

A C++ program that procedurally generates a village layout — terraforming land, subdividing it into plots, placing rooms and furniture, and connecting everything with pathways — inspired by Minecraft-style world generation.

## What it does

- **Terraforms** a 3D grid-based world, clearing and flattening land ready for construction.
- **Subdivides** plots of land into valid building sites and places rooms, doors, and furniture within them according to size and placement constraints.
- **Generates pathways** connecting waypoints across the village, including handling for elevation changes such as mountains.
- Validates each stage of generation against a structured suite of component and edge-case tests.

## Skills demonstrated

- Object-oriented design and modular architecture in C++
- Procedural generation algorithms
- Spatial/grid-based data structures
- Save/load logic with crash-resilience and edge-case handling
- Component and edge-case test design

## Tech stack

C++

## Building it

```bash
make
./gen-village
```

## Project context

Originally developed as a university systems programming assignment; uploaded here as a personal portfolio copy.
