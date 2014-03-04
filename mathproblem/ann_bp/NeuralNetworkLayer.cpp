
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
	int i = 0, j = 0;
	// prepare memorys
	NeuronValues = (double*) malloc(sizeof(double) * NumberOfNodes);
	DesiredValues = (double*) malloc(sizeof(double) * NumberOfNodes);
	Errors = (double*) malloc(sizeof(double) * NumberOfNodes);
	if (NULL != parent) { ParentLayer = parent; }
	if (NULL != child) {
		ChildLayer = child;
		Weights = (double**) malloc(sizeof(double*) * NumberOfNodes);
		WeightChanges = (double**) malloc(sizeof(double*) * NumberOfNodes);
		for (i = 0; i < NumberOfNodes; i ++) {
			Weights[i] = (double*) malloc(sizeof(double) * NumberOfChildNodes);
			WeightChanges[i] = (double*) malloc(sizeof(double) * NumberOfChildNodes);
		}
		BiasValues = (double*) malloc(sizeof(double) * NumberOfChildNodes);
		BiasWeights = (double*) malloc(sizeof(double) * NumberOfChildNodes);
	} else {
		Weights = NULL;
		BiasValues = NULL;
		BiasWeights = NULL;
		WeightChanges = NULL;
	}
	// memset all to blank
	for (i = 0; i < NumberOfNodes; i ++) {
		NeuronValues[i] = 0; DesiredValues[i] = 0; Errors[i] = 0;
		if (NULL != ChildLayer) {
			for (j = 0; j < NumberOfChildNodes; j ++) {
				Weightsp[i][j] = 0; WeightChanges[i][j] = 0;
			}
		}
	}
	// initialize bias and weight
	if (NULL != ChildLayer) {
		for (j = 0; j < NumberOfChildNodes; j ++) {
			BiasValues[j] = -1; BiasWeights[j] = 0;
		}
	}
}

void NeuralNetworkLayer::CleanUp(void) {
	int i = 0;
	free(NeuronValues);
	free(DesiredValues);
	free(Errors);
	if (NULL != Weights) {
		for (i = 0; i < NumberOfNodes; i ++) {
			free(Weights[i]);
			free(WeightChanges[i]);
		}
		free(Weights);
		free(WeightChanges);
	}
	if (NULL != BiasValues) { free(BiasValues); }
	if (NULL != BiasWeights) { free(BiasWeights); }
}

void NeuralNetworkLayer::RandomizeWeights(void) {
	int i = 0, j = 0, min = 0, max = 200, number = 0;
	srand((unsigned)time(NULL));
	for (i = 0; i < NumberOfNodes; i ++) {
		for (j = 0; j < NumberOfChildNodes; j ++) {
			number = (abs(rand())%(max-min-1))+min;
			if (number > max) { number = max; }
			if (number < min) { number = min; }
			Weights[i][j] = number / 100.0f - 1;
		}
	}
	for (j = 0; j < NumberOfChildNodes; j ++) {
		number = (abs(rand())%(max-min-1))+min;
		if (number > max) { number = max; }
		if (number < min) { number = min; }
		BiasWeights[i][j] = number / 100.0f - 1;
	}
}

void NeuralNetworkLayer::CalculateNeuronValues(void) {
	int i = 0, j = 0;
	double x = .0;
	if (NULL != ParentLayer) {
		for (j = 0; j < NumberOfNodes; j ++) {
			x = 0;
			for (i = 0; i < NumberOfParentNodes; i ++) {
				x += ParentLayer->NeuronValues[i] * ParentLayer->Weights[i][j];
			}
			x += ParentLayer->BiasValues[i] * ParentLayer->BiasWeights[j];
			if ((NULL == ChildLayer) && LinearOutput) {
				NeuronValues[j] = x;
			} else {
				NeuronValues[j] = 1.0f/(1+exp(-x));
			}
		}
	}
}

void NeuralNetworkLayer::CalculateErrors(void) {
	int i = 0, j = 0;
	double sum = .0;
	if (NULL == ChildLayer) { // output layer
		for (i = 0; i < NumberOfNodes; i ++) {
			Errors[i] = (DesiredValues[i] - NeuronValues[i]) 
				* NeuronValues[i] * (1.0f - NeuronValues[i]);
		}
	} else if (NULL == ParentLayer) { // input layer
		for (i = 0; i < NumberOfNodes; i ++) {
			Errors[i] = 0.0f;
		}
	} else { // hidden layer
		for (i = 0; i < NumberOfNodes; i ++) {
			sum = .0;
			for (j = 0; j < NumberOfChildNodes; j ++) {
				sum += ChildLayer->Errors[j] * Weights[i][j];
			}
			Errors[i] = sum * NeuronValues[i] * (1.0f - NeuronValues[i]);
		}
	}
}

void NeuralNetworkLayer::AdjustWeights(void) {
	int i = 0, j = 0;
	double dw = .0;
	if (NULL != ChildLayer) {
		for (i = 0; i < NumberOfNodes; i ++) {
			for (j = 0; j < NumberOfChildNodes; j ++) {
				dw = LearningRate * ChildLayer->Errors[j] * NeuronValues[i];
				if (UseMomentum) {
					Weights[i][j] += dw + MomentumFactor * WeightChanges[i][j];
					WeightChanges[i][j] = dw;
				} else {
					Weights[i][j] += dw;
				}
			}
		}
		for (j = 0; j < NumberOfChildNodes; j ++) {
			BiasWeights[j] += LearningRate * ChildLayer->Errors[j] * BiasValues[j];
		}
	}
}

