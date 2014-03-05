
#include "NeuralNetwork.hpp"
#include <stdio.h>
#include <math.h>
#include <limits.h>

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
	InputLayer.CleanUp();
	HiddenLayer.CleanUp();
	OutputLayer.CleanUp();
}

void NeuralNetwork::SetInput(int i, double value) {
	if ((i >= 0) && (i < InputLayer.NumberOfNodes)) {
		InputLayer.NeuronValues[i] = value;
	}
}

double NeuralNetwork::GetOutput(int i) {
	if ((i >= 0) && (i < OutputLayer.NumberOfNodes)) {
		return OutputLayer.NeuronValues[i];
	}
	return (double) INT_MAX;
}

void NeuralNetwork::SetDesiredOutput(int i, double value) {
	if ((i >= 0) && (i < OutputLayer.NumberOfNodes)) {
		OutputLayer.DesiredValues[i] = value;
	}
}

void NeuralNetwork::FeedForward(void) {
	InputLayer.CalculateNeuronValues();
	HiddenLayer.CalculateNeuronValues();
	OutputLayer.CalculateNeuronValues();
}

void NeuralNetwork::BackPropagate(void) {
	OutputLayer.CalculateErrors();
	HiddenLayer.CalculateErrors();
	HiddenLayer.AdjustWeights();
	InputLayer.AdjustWeights();
}

int NeuralNetwork::GetMaxOutputID(void) {
	int i = 0, id = 0;
	double maxval = .0;
	maxval = OutputLayer.NeuronValues[0];
	id = 0;
	for (i = 1; i < OutputLayer.NumberOfNodes; i ++) {
		if (OutputLayer.NeuronValues[i] > maxval) {
			maxval = OutputLayer.NeuronValues[i];
			id = i;
		}
	}
	return id;
}

double NeuralNetwork::CalculateError(void) {
	int i = 0;
	double error = .0;
	for (i = 0; i < OutputLayer.NumberOfNodes; i ++) {
		error += pow(OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i], 2);
	}
	error = error / OutputLayer.NumberOfNodes;
	return error;
}

void NeuralNetwork::SetLearningRate(double rate) {
	InputLayer.LearningRate = rate;
	HiddenLayer.LearningRate = rate;
	OutputLayer.LearningRate = rate;
}

void NeuralNetwork::SetLinearOutput(bool useLinear) {
	InputLayer.LinearOutput = useLinear;
	HiddenLayer.LinearOutput = useLinear;
	OutputLayer.LinearOutput = useLinear;
}

void NeuralNetwork::SetMomentum(bool useMomentum, double factor) {
	InputLayer.UseMomentum = useMomentum;
	HiddenLayer.UseMomentum = useMomentum;
	OutputLayer.UseMomentum = useMomentum;
	InputLayer.MomentumFactor = factor;
	HiddenLayer.MomentumFactor = factor;
	OutputLayer.MomentumFactor = factor;
}

void NeuralNetwork::DumpData(const char* filename) {
	int i = 0, j = 0;
	FILE* f = NULL;
	f = fopen(filename, "w");
	fprintf(f, "-----------------------------------------\n");
	fprintf(f, "Input Layer\n");
	fprintf(f, "-----------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "Node Values:\n");
	fprintf(f, "\n");
	for (i = 0; i < InputLayer.NumberOfNodes; i ++) {
		fprintf(f, "(%d) = %f\n", i, InputLayer.NeuronValues[i]);
	}
	fprintf(f, "\n");
	fprintf(f, "Weights:\n");
	fprintf(f, "\n");
	for (i = 0; i < InputLayer.NumberOfNodes; i ++) {
		for (j = 0; j < InputLayer.NumberOfChildNodes; j ++) {
			fprintf(f, "(%d, %d) = %f\n", i, j, InputLayer.Weights[i][j]);
		}
	}
	fprintf(f, "\n");
	fprintf(f, "Bias Weights:\n");
	fprintf(f, "\n");
	for (j = 0; j < InputLayer.NumberOfChildNodes; j ++) {
		fprintf(f, "(%d) = %f\n", j, InputLayer.BiasWeights[j]);
	}
	fprintf(f, "\n");
	fprintf(f, "\n");
	fprintf(f, "-----------------------------------------\n");
	fprintf(f, "Hidden Layer\n");
	fprintf(f, "-----------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "Weights:\n");
	fprintf(f, "\n");
	for (i = 0; i < HiddenLayer.NumberOfNodes; i ++) {
		for (j = 0; j < HiddenLayer.NumberOfChildNodes; j ++) {
			fprintf(f, "(%d, %d) = %f\n", i, j, HiddenLayer.Weights[i][j]);
		}
	}
	fprintf(f, "\n");
	fprintf(f, "Bias Weights:\n");
	fprintf(f, "\n");
	for (j = 0; j < HiddenLayer.NumberOfChildNodes; j ++) {
		fprintf(f, "(%d) = %f\n", j, HiddenLayer.BiasWeights[j]);
	}
	fprintf(f, "\n");
	fprintf(f, "\n");
	fprintf(f, "-----------------------------------------\n");
	fprintf(f, "Output Layer\n");
	fprintf(f, "-----------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "Node Values:\n");
	fprintf(f, "\n");
	for (i = 0; i < OutputLayer.NumberOfNodes; i ++) {
		fprintf(f, "(%d) = %f\n", i, OutputLayer.NeuronValues[i]);
	}
	fprintf(f, "\n");
	fclose(f);
}

