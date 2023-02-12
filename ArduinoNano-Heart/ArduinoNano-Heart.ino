// Import TensorFlow libraries
//#include "TensorFlowLite.h"
//#include<cstdio.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>
#include "C:\Users\DELL\Documents\Arduino\ArduinoNano-Heart\main_function.ino"
#include "C:\Users\DELL\Documents\Arduino\ArduinoNano-Heart\debug.ino"
#include "C:\Users\DELL\Documents\Arduino\ArduinoNano-Heart\arduino_outputHandler.ino"
#include "C:\Users\DELL\Documents\Arduino\ArduinoNano-Heart\arduino_main.ino"
#include "C:\Users\DELL\Documents\Arduino\ArduinoNano-Heart\arduino_constants.ino"

// Our model
#include "C:\Users\DELL\Desktop\NCUE\heartBeatTF.h"

// Figure out what's going on in our model
#define DEBUG 1

// Some settings
constexpr int led_pin = 2;
constexpr float pi = 3.14159265;                  // Some pi
constexpr float freq = 0.5;                       // Frequency (Hz) of sinewave
constexpr float period = (1 / freq) * (1000000);  // Period (microseconds)

// TFLite globals, used for compatibility with Arduino-style sketches
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* model_input = nullptr;
  TfLiteTensor* model_output = nullptr;

  // Create an area of memory to use for input, output, and other TensorFlow
  constexpr int kTensorArenaSize = 5 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
} // namespace

void setup() {
  // put your setup code here, to run once:
 // Wait for Serial to connect
#if DEBUG
  while(!Serial);
#endif

  // Let's make an LED vary in brightness
  pinMode(led_pin, OUTPUT);

  // Set up logging (will report to Serial, even within TFLite functions)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure
  model = tflite::GetModel(heartBeatTF);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report("Model version does not match Schema");
    while(1);
  }

  // Pull in only needed operations (should match NN layers)
  tflite::AllOpsResolver resolver;

  // Allocate memory from the tensor_arena for the model's tensors
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    while(1);
  }

  // Assign model input and output buffers (tensors) to pointers
  model_input = interpreter->input(0);
  model_output = interpreter->output(0);

  // Get information about the memory area to use for the model's input
  // Supported data types:
  // https://github.com/tensorflow/tensorflow/blob/master/tensorflow/lite/c/common.h#L226
#if DEBUG
  Serial.print("Number of dimensions: ");
  Serial.println(model_input->dims->size);
  Serial.print("Dim 1 size: ");
  Serial.println(model_input->dims->data[0]);
  Serial.print("Dim 2 size: ");
  Serial.println(model_input->dims->data[1]);
  Serial.print("Input type: ");
  Serial.println(model_input->type);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
