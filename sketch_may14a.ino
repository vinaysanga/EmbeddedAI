/*
  IMU Classifier
  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU, once enough samples are read, it then uses a
  TensorFlow Lite (Micro) model to try to classify the movement as a known gesture.
  Note: The direct use of C/C++ pointers, namespaces, and dynamic memory is generally
        discouraged in Arduino examples, and in the future the TensorFlowLite library
        might change to make the sketch simpler.
  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.
  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry
  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"

#include "model.h"
#include "test_1.h"

// Define the image dimensions and number of classes
const int IMAGE_WIDTH = 28;
const int IMAGE_HEIGHT = 28;
const int IMAGE_CHANNELS = 1;
const int NUM_CLASSES = 10;

// Array to map class index to class name
const char* CLASS_NAMES[] = {
  "Class 0",
  "Class 1",
  "Class 2",
  "Class 3",
  "Class 4",
  "Class 5",
  "Class 6",
  "Class 7",
  "Class 8",
  "Class 9",
};

// Global variables used for TensorFlow Lite (Micro)
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 70 * 1024;

byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

int normalizeData(int input) {
  // Define the input and output ranges
  int minInput = 0;
  int maxInput = 255;
  int minOutput = -128;
  int maxOutput = 127;

  // Normalize the input to the range [0, 1]
  float normalized = (float)(input - minInput) / (maxInput - minInput);

  // Scale to the range [minOutput, maxOutput]
  int scaled = (int)(normalized * (maxOutput - minOutput) + minOutput);

  return scaled;
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("Serial communication started.");

  // Get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  Serial.println("Model loaded successfully.");

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize);

 
  // Allocate memory for the model's input and output tensors
  if (tflInterpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("AllocateTensors() failed");
    while (1);
  }
  Serial.println("Tensors allocated.");

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
  Serial.println("Input and output tensors obtained.");

  // Load the input image data into the input tensor
  for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_CHANNELS; i++) {
    tflInputTensor->data.int8[i] = static_cast<int>(normalizeData(image_data[i]));
  }
  Serial.println("Image data loaded into input tensor.");

  // Run inferencing
  TfLiteStatus invokeStatus = tflInterpreter->Invoke();
  if (invokeStatus != kTfLiteOk) {
    Serial.println("Invoke failed!");
    while (1);
  }
  Serial.println("Inference completed.");

  // Find the class with the maximum probability
  float maxProbability = 0;
  int maxClass = -999;
  int ans = 0;

  for (int i = 0; i < NUM_CLASSES; i++) {
    int probability = tflOutputTensor->data.int8[i];
    if(probability > maxClass){
      maxClass = probability;
      ans = i;
    }
  }
    Serial.print("Class: ");
    Serial.print(ans);
}

void loop() {
  // Empty loop because we are doing all the inference in the setup
}