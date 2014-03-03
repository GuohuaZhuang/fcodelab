
#include "NeuralNetwork.hpp"

void NeuralNetwork::Initialize(int nNodesInput, 
	int nNodesHidden, int nNodesOutput) {
	InputLayer.NumberOfNodes = nNodesInput;
	InputLayer.NumberOfChildNodes = nNodesHidden;
	InputLayer.NumberOfParentNodes = 0;
	InputLayer.Initialize(nNodesInput, NULL, &HiddenLayer);
	InputLayer.RandomizeWeights();

	HiddenLayer.NumberOfNodes = nNodesHidden;
	HiddenLayer.NumberOfChildNodes = nNodesOutput;
	HiddenLayer.NumberOfParentNodes = nNodesInput;
	HiddenLayer.Initialize(nNodesHidden, &InputLayer, &OutputLayer);
	HiddenLayer.RandomizeWeights();

	OutputLayer.NumberOfNodes = nNodesOutput;
	OutputLayer.NumberOfChildNodes = 0;
	OutputLayer.NumberOfParentNodes = nNodesHidden;
	OutputLayer.Initialize(nNodesOutput, &HiddenLayer, NULL);
}

void NeuralNetwork::CleanUp() {
}

void NeuralNetwork::SetInput(int i, double value) {
}

double NeuralNetwork::GetOutput(int i) {
}

void NeuralNetwork::SetDesiredOutput(int i, double value) {
}

void NeuralNetwork::FeedForward(void) {
}

void NeuralNetwork::BackPropagate(void) {
}

int NeuralNetwork::GetMaxOutputID(void) {
}

double NeuralNetwork::CalculateError(void) {
}

void NeuralNetwork::SetLearningRate(double rate) {
}

void NeuralNetwork::SetLinearOutput(bool useLinear) {
}

void NeuralNetwork::SetMomentum(bool useMomentum, double factor) {
}

void NeuralNetwork::DumpData(char* filename) {
}

