#!/usr/bin/env python3
"""
Quick test to verify the BJJ RL setup is working.
"""

import os
import sys

def quick_test():
    print("🥋 Quick BJJ RL Test")
    print("===================")
    
    # Test 1: Check if we're in the right directory
    required_files = ['README.md', 'Graph/files/nodes.json', 'Graph/files/transitions.json']
    for file in required_files:
        if os.path.exists(file):
            print(f"✓ Found {file}")
        else:
            print(f"❌ Missing {file}")
            return False
    
    # Test 2: Try importing the main modules
    try:
        print("\nTesting imports...")
        import numpy as np
        import networkx as nx
        import gymnasium as gym
        print("✓ Core libraries imported successfully")
        
        from Graph.graph_constructor import construct_graph
        print("✓ Graph constructor imported")
        
        from Game.play_game import Game
        print("✓ Game module imported")
        
        from Game.gym_env import BJJEnv
        print("✓ BJJ Environment imported")
        
    except ImportError as e:
        print(f"❌ Import error: {e}")
        return False
    
    # Test 3: Try creating the graph
    try:
        print("\nTesting graph construction...")
        G = construct_graph()
        print(f"✓ Graph created with {G.number_of_nodes()} nodes and {G.number_of_edges()} edges")
    except Exception as e:
        print(f"❌ Graph construction failed: {e}")
        return False
    
    # Test 4: Try creating the environment
    try:
        print("\nTesting environment...")
        env = BJJEnv()
        obs, info = env.reset()
        print("✓ BJJ Environment created and reset successfully")
    except Exception as e:
        print(f"❌ Environment creation failed: {e}")
        return False
    
    print("\n🎉 All tests passed! The BJJ RL environment is ready to use.")
    return True

if __name__ == "__main__":
    success = quick_test()
    if not success:
        print("\n💡 If you see errors, try:")
        print("1. Make sure you're in the project root directory")
        print("2. Activate your virtual environment: source venv/bin/activate")
        print("3. Install dependencies: pip install -r requirements.txt")
        sys.exit(1)
    else:
        print("\n🚀 Try running a quick training session:")
        print("python -c \"from Game.gym_env import BJJEnv, q_learning; env = BJJEnv(); print('Training 5 episodes...'); q_table = q_learning(env, num_episodes=5); print('Done!')\"")
