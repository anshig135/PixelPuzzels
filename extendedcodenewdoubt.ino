#include <LedControl.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h> // Include for PROGMEM

// Pins connected to MAX7219
const int DIN_PIN = 11;  // Data in pin
const int CLK_PIN = 13;  // Clock pin
const int CS_PIN = 10;   // Chip select pin

// Pins for buttons and buzzer
const int BUTTON_UP = 3;
const int BUTTON_DOWN = 4;
const int BUTTON_LEFT = 5;
const int BUTTON_RIGHT = 6;
const int BUTTON_TOGGLE = 7;  // Toggle LED state
const int BUTTON_COMPARE = 2; // Compare patterns
const int BUTTON_NEW_PATTERN = 8; // Generate new specific shape pattern
const int BUTTON_RESET = 12;  // Reset user's matrix
const int BUZZER_PIN = 9;

// Create LedControl object (DataIn, CLK, CS, # of devices)
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 2);  // Control 2 devices
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// Maximum patterns and shape definitions
const int numShapes = 15; // Increase number of predefined shapes
const int shapes[numShapes][8][8] PROGMEM = {
  { // Square
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Simple Block
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  },
  { // Plus Sign
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Cross (Different Orientation)
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0}
  },
  { // Heart Expanded
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Spiral
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1}
  },
  { // Zigzag
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1}
  },
  { // Diamond
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Star
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Triangle
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  },
  { // Cross
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Checkmark
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Chessboard
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1}
  },
  { // House
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  { // Pin
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

// Store the shapes used state
bool shapeUsed[numShapes] = {false};

int randomPattern[8][8] = {{0}};  // Random pattern stored here (Matrix 1)
int userPattern[8][8] = {{0}};    // User-created pattern stored here (Matrix 2)

int selectedRow = 0;  // Current selected row for user matrix
int selectedCol = 0;  // Current selected column for user matrix

int lastButtonStates[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}; // Track button states
int lastCompareState = HIGH;

// Function to load a specific shape into the randomPattern matrix
void loadSpecificShape(int shapeIndex) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      randomPattern[i][j] = pgm_read_byte_near(&shapes[shapeIndex][i][j]);
    }
  }
  shapeUsed[shapeIndex] = true; // Mark the shape as used
}

int getUnusedShapeIndex() {
  // Check if all shapes have been used
  bool allUsed = true;
  for (int i = 0; i < numShapes; i++) {
    if (!shapeUsed[i]) {
      allUsed = false;
      break;
    }
  }

  // Reset if all shapes have been used
  if (allUsed) {
    for (int i = 0; i < numShapes; i++) {
      shapeUsed[i] = false;
    }
  }

  // Select a random unused shape
  int shapeIndex;
  do {
    shapeIndex = random(0, numShapes);
  } while (shapeUsed[shapeIndex]);
  return shapeIndex;
}

// Update matrix for LED control
void updateMatrix(int deviceIndex, int pattern[8][8]) {
  lc.clearDisplay(deviceIndex);
  for (int row = 0; row < 8; row++) {
    byte rowValue = 0;
    for (int col = 0; col < 8; col++) {
      bitWrite(rowValue, col, pattern[row][col]);
    }
    lc.setRow(deviceIndex, row, rowValue);
  }
}

void updateRandomMatrix() {
  updateMatrix(0, randomPattern);  // Update Matrix 1
}

void updateUserMatrix() {
  updateMatrix(1, userPattern);   // Update Matrix 2
}

void playTone(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
  delay(duration + 10);
  noTone(BUZZER_PIN);
}

void resetUserMatrix() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      userPattern[i][j] = 0;
    }
  }
  updateUserMatrix();
}

// Display messages on the LCD
void displayGameStartMessage() {
    lcd.clear();
    lcd.print("Game Starts");
    lcd.setCursor(0, 1);
    lcd.print("Get Ready!");
    delay(3000);
    lcd.clear();
}

void displayLCDMessage(bool isMatched) {
    lcd.clear();
    if (isMatched) {
        lcd.print("Congratulations!");
        delay(2000);
        lcd.setCursor(0, 1);
        lcd.print("You matched it!");
    } else {
        lcd.print("Sorry");
        delay(2000);
        lcd.setCursor(0, 1);
        lcd.print("Try again!");
    }
    delay(3000);
    lcd.clear();
}

void setup() {
    lcd.begin(16, 2);
    displayGameStartMessage();
    for (int i = 0; i < 2; i++) {
        lc.shutdown(i, false);    // Wake up the MAX7219
        lc.setIntensity(i, 8);    // Set brightness
        lc.clearDisplay(i);       // Clear display
    }

    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(BUTTON_TOGGLE, INPUT_PULLUP);
    pinMode(BUTTON_COMPARE, INPUT_PULLUP);
    pinMode(BUTTON_NEW_PATTERN, INPUT_PULLUP);
    pinMode(BUTTON_RESET, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    // Load the first shape
    int shapeIndex = getUnusedShapeIndex();
    loadSpecificShape(shapeIndex);
    updateRandomMatrix();
}

void loop() {
    int buttonStates[7] = {
        digitalRead(BUTTON_UP),
        digitalRead(BUTTON_DOWN),
        digitalRead(BUTTON_LEFT),
        digitalRead(BUTTON_RIGHT),
        digitalRead(BUTTON_TOGGLE),
        digitalRead(BUTTON_NEW_PATTERN),
        digitalRead(BUTTON_RESET)
    };
    int compareState = digitalRead(BUTTON_COMPARE);

    // Generate new shape on pressing the New Pattern Button
    if (buttonStates[5] == LOW && lastButtonStates[5] == HIGH) {
        int shapeIndex = getUnusedShapeIndex();
        loadSpecificShape(shapeIndex);
        updateRandomMatrix();
    }

    // Reset user's matrix on pressing the Reset Button
    if (buttonStates[6] == LOW && lastButtonStates[6] == HIGH) {
        resetUserMatrix();
    }

    // Check if the button state is pressed and update the user pattern matrix
    if (buttonStates[0] == LOW && lastButtonStates[0] == HIGH) {
      selectedRow = (selectedRow - 1 + 8) % 8;
    }
 
    if (buttonStates[1] == LOW && lastButtonStates[1] == HIGH) {
      selectedRow = (selectedRow + 1) % 8;
    }
 
    if (buttonStates[2] == LOW && lastButtonStates[2] == HIGH) {
      selectedCol = (selectedCol - 1 + 8) % 8;
    }
 
    if (buttonStates[3] == LOW && lastButtonStates[3] == HIGH) {
      selectedCol = (selectedCol + 1) % 8;
    }

    // Toggle LED state
    if (buttonStates[4] == LOW && lastButtonStates[4] == HIGH) {
      userPattern[selectedRow][selectedCol] = !userPattern[selectedRow][selectedCol];
      updateUserMatrix();
    }

    // Check compare button
    if (compareState == LOW && lastCompareState == HIGH) {
        bool isMatched = true;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (randomPattern[i][j] != userPattern[i][j]) {
                    isMatched = false;
                    break;
                }
            }
        }
        playTone(isMatched ? 1000 : 500, 500);
        displayLCDMessage(isMatched);
    }

    // Update last button states
    for (int i = 0; i < 7; i++) {
        lastButtonStates[i] = buttonStates[i];
    }
    lastCompareState = compareState;
}
