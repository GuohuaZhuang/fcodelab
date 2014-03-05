#include "NeuralNetwork.hpp"
#include <stdio.h>
#include <stdlib.h>

double	TrainingSet[14][7]	=	{
	{ 0,	1,		0,	0.2,		0.9,	0.1,	0.1 },	
	{ 0,	1,		1,	0.2,		0.9,	0.1,	0.1 },	
	{ 0,	1,		0,	0.8,		0.1,	0.1,	0.1 },	
	{ 0.1,	0.5,	0,	0.2,		0.9,	0.1,	0.1 },	
	{ 0,	0.25,	1,	0.5,		0.1,	0.9,	0.1 },	
	{ 0,	0.2,	1,	0.2,		0.1,	0.1,	0.9 },	
	{ 0.3,	0.2,	0,	0.2,		0.9,	0.1,	0.1 },	
	{ 0,	0.2,	0,	0.3,		0.1,	0.9,	0.1 },	
	{ 0,	1,		0,	0.2,		0.1,	0.9,	0.1 },	
	{ 0,	1,		1,	0.6,		0.1,	0.1,	0.1 },	
	{ 0,	1,		0,	0.8,		0.1,	0.9,	0.1 },	
	{ 0.1,	0.2,	0,	0.2,		0.1,	0.1,	0.9 },	
	{ 0,	0.25,	1,	0.5,		0.1,	0.1,	0.9 },	
	{ 0,	0.6,	0,	0.2,		0.1,	0.1,	0.9 }
};

NeuralNetwork TheBrain;

void TrainTheBrain(void) {
	int i = 0, c = 0;
	double error = 1.0;
	TheBrain.DumpData("PreTraining.txt");
	while ((error > 0.05) && (c < 50000)) {
		error = 0; c++;
		for (i = 0; i < 14; i ++) {
			TheBrain.SetInput(0, TrainingSet[i][0]);
			TheBrain.SetInput(1, TrainingSet[i][1]);
			TheBrain.SetInput(2, TrainingSet[i][2]);
			TheBrain.SetInput(3, TrainingSet[i][3]);
			TheBrain.SetDesiredOutput(0, TrainingSet[i][4]);
			TheBrain.SetDesiredOutput(1, TrainingSet[i][5]);
			TheBrain.SetDesiredOutput(2, TrainingSet[i][6]);
			TheBrain.FeedForward();
			error += TheBrain.CalculateError();
			TheBrain.BackPropagate();
		}
		error = error / 14.0f;
	}
	TheBrain.DumpData("PostTraining.txt");
}

int main(int argc, const char *argv[])
{
	TheBrain.Initialize(4, 3, 3);
	TheBrain.SetLearningRate(0.2);
	TheBrain.SetMomentum(true, 0.9);
	TrainTheBrain();

	return 0;
}
