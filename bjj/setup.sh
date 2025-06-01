#!/bin/bash

echo "🥋 BJJ RL Setup Script 🥋"
echo "========================="

# Check if we're in the right directory
if [ ! -f "README.md" ] || [ ! -d "Graph" ] || [ ! -d "Game" ]; then
    echo "❌ Error: Please run this script from the BJJ RL project root directory"
    echo "   Expected files: README.md, Graph/, Game/"
    exit 1
fi

echo "✓ Found project files"

# Create virtual environment if it doesn't exist
if [ ! -d "venv" ]; then
    echo "📦 Creating virtual environment..."
    python3 -m venv venv
    if [ $? -ne 0 ]; then
        echo "❌ Failed to create virtual environment"
        exit 1
    fi
    echo "✓ Virtual environment created"
else
    echo "✓ Virtual environment already exists"
fi

# Activate virtual environment
echo "🔧 Activating virtual environment..."
source venv/bin/activate

# Install dependencies
echo "📚 Installing dependencies..."
pip install --upgrade pip
pip install -r requirements.txt

if [ $? -ne 0 ]; then
    echo "❌ Failed to install dependencies"
    exit 1
fi

echo "✓ Dependencies installed successfully"

# Run tests
echo "🧪 Running setup tests..."
python test_setup.py

if [ $? -eq 0 ]; then
    echo ""
    echo "🎉 Setup completed successfully!"
    echo ""
    echo "Next steps:"
    echo "1. Activate the virtual environment: source venv/bin/activate"
    echo "2. Try a quick training run: python -c \"from Game.gym_env import BJJEnv, q_learning; env = BJJEnv(); q_table = q_learning(env, num_episodes=5); print('Training completed!')\""
    echo "3. Explore the notebooks: jupyter notebook"
    echo ""
    echo "Happy training! 🥋"
else
    echo "❌ Setup tests failed. Please check the error messages above."
    exit 1
fi
