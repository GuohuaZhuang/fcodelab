
#include "NeuralNetworkLayer.hpp"

class NeuralNetwork
{
public:
	NeuralNetworkLayer InputLayer;
	NeuralNetworkLayer HiddenLayer;
	NeuralNetworkLayer OutputLayer;

	void Initialize(int nNodesInput, int nNodesHidden, int nNodesOutput);
	void CleanUp();
	void SetInput(int i, double value);
	double GetOutput(int i);
	void SetDesiredOutput(int i, double value);
	void FeedForward(void);
	void BackPropagate(void);
	int GetMaxOutputID(void);
	double CalculateError(void);
	void SetLearningRate(double rate);
	void SetLinearOutput(bool useLinear);
	void SetMomentum(bool useMomentum, double factor);
	void DumpData(char* filename);
};

