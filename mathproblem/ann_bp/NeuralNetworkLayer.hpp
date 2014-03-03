
class NeuralNetworkLayer
{
public:
	int NumberOfNodes;
	int NumberOfChildNodes;
	int NumberOfParentNodes;
	double** Weights;
	double** WeightChanges;
	double* NeuronValues;
	double* DesiredValues;
	double* Errors;
	double* BiasWeights;
	double* BiasValues;
	double LearningRate;

	bool LinearOutput;
	bool UseMomentum;
	double MomentumFactor;

	NeuralNetworkLayer* ParentLayer;
	NeuralNetworkLayer* ChildLayer;

	NeuralNetworkLayer();

	void Initialize(int NumNodes, 
		NeuralNetworkLayer* parent, NeuralNetworkLayer* child);

	void CleanUp(void);
	void RandomizeWeights(void);
	void CalculateErrors(void);
	void AdjustWeights(void);
	void CalculateNeuronValues(void);
};

