
#include "ann_bp.hpp"

NeuralNetworkLayer::NeuralNetworkLayer() {
	ParentLayer = NULL;
	ChildLayer = NULL;
	LinearOutput = false;
	UseMomentum = false;
	MomentumFactor = 0.9;
}

NeuralNetworkLayer::Initialize(int NumNodes, 
	NeuralNetworkLayer* parent, NeuralNetworkLayer* child) {
}

void NeuralNetworkLayer::CleanUp(void) {
}

void NeuralNetworkLayer::RandomizeWeights(void) {
}

void NeuralNetworkLayer::CalculateNeuronValues(void) {
}

void NeuralNetworkLayer::CalculateErrors(void) {
}

void NeuralNetworkLayer::AdjustWeights(void) {
}

