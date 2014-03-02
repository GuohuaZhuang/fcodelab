
#include "ann_bp.hpp"

NeuralNetworkLayer::NeuralNetworkLayer() {
	ParentLayer = NULL;
	ChildLayer = NULL;
	LinearOutput = false;
	UseMomentum = false;
	MomentumFactor = 0.9;
}

