#pragma once

#include <random>
#include <iostream>
#include <NeuralNetwork/Dataset.h>

int RandomFromRange(int lowerLimit, int upperLimit);
void ShuffleVector(Dataset* dataset);