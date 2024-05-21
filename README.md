# Embedded AI Project

## Project Files

### 1. Embedded_AI_Project_2.ipynb

**Description:**
This Jupyter notebook contains the initial steps for preparing and training the machine learning model. The key sections of the notebook include:

- **Data Preprocessing:** Normalizing image data and preparing it for model training.
- **Model Training:** Training the neural network model using TensorFlow.
- **Model Optimization:** Pruning the model to remove insignificant weights and applying INT8 quantization to reduce model size and improve inference efficiency.
- **Model Conversion:** Converting the trained model into a format suitable for deployment on the Arduino Nano 33 BLE Sense.
- **Exporting the Model:** Exporting the optimized and quantized model to a C header file that can be included in the Arduino sketch.

**Usage:**
Run this notebook to preprocess data, train the model, apply optimizations, and export the final model for deployment.

### 2. sketch_may14a.ino

**Description:**
This Arduino sketch is responsible for deploying the trained and optimized machine learning model on the Arduino Nano 33 BLE Sense. The key sections of the sketch include:

- **Setup:** Initializing the Arduino Nano 33 BLE.
- **Model Integration:** Including the quantized model in C array format and setting up TensorFlow Lite Micro for inference.
- **Inference:** Running the model inference on the provided images and obtaining classification results.
- **Output Handling:** Displaying the inference results as the class with highest probability.

**Usage:**
Upload this sketch to the Arduino Nano 33 BLE Sense to run the machine learning model and perform image classification. Ensure that the required libraries and dependencies are installed in the Arduino IDE.
