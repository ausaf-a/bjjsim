#!/usr/bin/env python3
"""
Test script to verify the BJJ RL environment is working correctly.
"""

import sys
import os

# Add the project root to the Python path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

def test_imports():
    """Test that all required modules can be imported."""
    print("Testing imports...")
    
    try:
        import numpy as np
        print("✓ numpy imported successfully")
    except ImportError as e:
        print(f"✗ Failed to import numpy: {e}")
        return False
    
    try:
        import networkx as nx
        print("✓ networkx imported successfully")
    except ImportError as e:
        print(f"✗ Failed to import networkx: {e}")
        return False
    
    try:
        import gymnasium as gym
        print("✓ gymnasium imported successfully")
    except ImportError as e:
        print(f"✗ Failed to import gymnasium: {e}")
        return False
    
    try:
        from Graph.graph_constructor import construct_graph
        print("✓ graph_constructor imported successfully")
    except ImportError as e:
        print(f"✗ Failed to import graph_constructor: {e}")
        return False
    
    return True

def test_graph_construction():
    """Test that the graph can be constructed."""
    print("\nTesting graph construction...")
    
    try:
        from Graph.graph_constructor import construct_graph
        G = construct_graph()
        print(f"✓ Graph constructed successfully with {G.number_of_nodes()} nodes and {G.number_of_edges()} edges")
        return True
    except Exception as e:
        print(f"✗ Failed to construct graph: {e}")
        return False

def test_game_creation():
    """Test that a game can be created."""
    print("\nTesting game creation...")
    
    try:
        from Game.play_game import Game
        game = Game("Test Game")
        game.initialize_game("Player1", "Player2")
        print("✓ Game created and initialized successfully")
        return True
    except Exception as e:
        print(f"✗ Failed to create game: {e}")
        return False

def test_gym_env():
    """Test that the Gym environment can be created."""
    print("\nTesting Gym environment...")
    
    try:
        from Game.gym_env import BJJEnv
        env = BJJEnv()
        obs, info = env.reset()
        print("✓ BJJ Gym environment created and reset successfully")
        print(f"  - Observation space: {env.observation_space}")
        print(f"  - Action space: {env.action_space}")
        return True
    except Exception as e:
        print(f"✗ Failed to create BJJ environment: {e}")
        return False

def main():
    """Run all tests."""
    print("🥋 Testing BJJ RL Environment Setup 🥋\n")
    
    tests = [
        test_imports,
        test_graph_construction,
        test_game_creation,
        test_gym_env
    ]
    
    results = []
    for test in tests:
        results.append(test())
    
    print(f"\n{'='*50}")
    print(f"Test Results: {sum(results)}/{len(results)} passed")
    
    if all(results):
        print("🎉 All tests passed! Your BJJ RL environment is ready to use.")
        return True
    else:
        print("❌ Some tests failed. Check the error messages above.")
        return False

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
