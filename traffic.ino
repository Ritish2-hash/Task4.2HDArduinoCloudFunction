/*
Name- Ansh Sharma
Roll Number - 2310994763
The provided Arduino code enables remote control of three LEDs (red, green, and blue) by fetching their states from a Firebase Realtime Database.
It begins by connecting the Arduino Nano to a specified WiFi network and initializing a connection to Firebase using defined credentials.
In the main loop, the code continuously requests JSON data from the Firebase database every five seconds to determine the desired states of the LEDs.
Based on the received JSON response, it processes the LED states and updates the corresponding LEDs on the Arduino, turning them on or off as specified.
This setup allows for a cloud-based solution to control the LED lights remotely, making it ideal for applications such as traffic light management in remote areas.
*/
#include <Firebase_Arduino_WiFiNINA.h>  // Firebase library to interact with Firebase services
#include <WiFiNINA.h>  // WiFi library for connecting to WiFi networks using the WiFiNINA 

// WiFi credentials
char ssid[] = "Ansh";   // Define the SSID (network name) of the WiFi to connect to
char pass[] = "123456789";   // Define the password for the WiFi network

// Firebase configuration
#define FIREBASE_HOST "https://traffic-71265-default-rtdb.asia-southeast1.firebasedatabase.app/" // The URL of your Firebase Realtime Database
#define FIREBASE_AUTH "AIzaSyDctp8Ypd5e2GkZyNn2GRuurfsES5QaGuY" // The Firebase Web API Key, used for authenticating requests
#define FIREBASE_PATH "/leds.json"  // The specific path in the Firebase database where LED states are stored

// Pin assignments for LEDs
int redLed = 2;     // Assigning pin 2 to the red LED
int greenLed = 4;   // Assigning pin 4 to the green LED
int blueLed = 3;    // Assigning pin 3 to the blue LED

FirebaseData firebaseData; // Creating an object to hold data from Firebase

void setup() {
  Serial.begin(9600);  // Start serial communication at a baud rate of 9600 for debugging output

  // Set the LED pins as outputs
  pinMode(redLed, OUTPUT);     // Configure the red LED pin as an output
  pinMode(greenLed, OUTPUT);   // Configure the green LED pin as an output
  pinMode(blueLed, OUTPUT);    // Configure the blue LED pin as an output

  // Connect to WiFi
  connectToWiFi();  // Call the function to establish a WiFi connection

  // Initialize the Firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, ssid, pass); // Start Firebase with the provided host, authentication, SSID, and password
  Firebase.reconnectWiFi(true);  // It enable automatic reconnection to WiFi if the connection drops
}

void loop() {
  // Here is an attempt to fetch LED states from Firebase
  if (Firebase.getJSON(firebaseData, FIREBASE_PATH)) { // It request JSON data from the specified Firebase path
    String jsonResponse = firebaseData.jsonData();  // It retrieve and store the JSON response received from Firebase
    Serial.println(jsonResponse);  // Printing the output to view the response
    
    // Process the LED states based on the received JSON response
    processLEDState(jsonResponse);  // Call the function to interpret the LED states from the response
    
    delay(5000);  // Introduce a delay of 5 seconds before making the next request to avoid overwhelming the Firebase 
  } else {
    // Handles the case where data retrieval from Firebase fails
    Serial.println("Failed to get data from Firebase"); // Output an error message if data retrieval fails
    Serial.println(firebaseData.errorReason()); // Print the reason for the failure to the serial monitor
  }
}

// Function to establish a connection to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi..."); // Print a message indicating the start of the WiFi connection attempt
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) { // Continuously try to connect to WiFi until successful
    delay(1000); // Wait for 1 second before retrying
    Serial.print("."); // Print a dot to show progress during connection attempts
  }
  Serial.println("\nConnected to WiFi"); // Print a message indicating successful WiFi connection
}

// Function to process LED states from the JSON response
void processLEDState(String jsonResponse) {
  // Check the JSON response to determine the state of each LED
  if (jsonResponse.indexOf("\"red\":true") != -1) { // Check if the red LED state is true in the JSON response
    digitalWrite(redLed, HIGH); // If true, turn on the red LED
  } else {
    digitalWrite(redLed, LOW); // If false, turn off the red LED
  }

  if (jsonResponse.indexOf("\"green\":true") != -1) { // Check if the green LED state is true in the JSON response
    digitalWrite(greenLed, HIGH); // If true, turn on the green LED
  } else {
    digitalWrite(greenLed, LOW); // If false, turn off the green LED
  }

  if (jsonResponse.indexOf("\"blue\":true") != -1) { // Check if the blue LED state is true in the JSON response
    digitalWrite(blueLed, HIGH); // If true, turn on the blue LED
  } else {
    digitalWrite(blueLed, LOW); // If false, turn off the blue LED
  }
}
