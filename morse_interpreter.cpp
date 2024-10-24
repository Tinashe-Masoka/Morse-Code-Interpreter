#include <LiquidCrystal.h>  // Include the library for the LCD display

// Pin setup
const int Pin_10 = 10;  // The button is connected to pin 10

// Button and timing variables
int buttonState = 0;         // Variable for reading the pushbutton status
int lastButtonState = 0;     // Previous state of the button
unsigned long lastDebounceTime = 0;  // Last time the button state was toggled
const int debounceDelay = 50;        // Debounce delay in milliseconds
unsigned long press_start = 0;       // Start time of the button press
int start = 1;                       // Flag to indicate the start of a press
float hold_time = 0;                 // Duration the button is held down
int column = 0;                      // LCD column position for top row
int bottom_column = 0;               // LCD column position for bottom row (results display)
int tree_map[20] = {1,1,1,1,1,1,1,1,1,1,1,1,1};  // Array to store Morse code dots and dashes
int len = 0;                         // Length of the current Morse code sequence
int zero_time = 0;                   // Start time when button is released
int low_time = 0;                    // Time the button is held low (not pressed)

LiquidCrystal lcd(4, 7, 5, 11, 12, 13);  // LCD pin configuration

// Struct definition for the Morse code binary tree
struct Node {
    char character;    // The character stored in the node
    Node* left;        // Pointer to the left child (dot)
    Node* right;       // Pointer to the right child (dash)
};

// Function to create a new node in the Morse code tree
Node* createnode(char x) {
    Node* new_node = new Node();  // Dynamically allocate a new node
    new_node-> character = x;     // Set the character value for the node
    new_node-> left = nullptr;    // Initialize the left pointer to null
    new_node-> right = nullptr;   // Initialize the right pointer to null
    return new_node;              // Return the newly created node
}

// Create the root of the Morse code tree
Node* root = createnode('0');

// Function to delete the Morse code tree (if needed for memory management)
void delete_tree(Node* node) {
    if (node == nullptr) return;  // Base case: return if the node is null

    // Recursively delete left and right subtrees
    delete_tree(node->left);
    delete_tree(node->right);

    // Delete the current node
    delete node;
}

// Function to traverse the Morse code tree based on the input map (dots/dashes)
void transverser(int* tree_map, int len) {
  Node* letter = root;  // Start at the root node
  for(int i = 0; i <= len; ++i) {
    if(i == len) {
      lcd.setCursor(bottom_column, 1);  // Move to the bottom row
      lcd.print(letter->character);     // Print the decoded character
      ++bottom_column;                  // Increment the bottom row cursor
      break;
    }
    if(tree_map[i] == 0) letter = letter->left;  // Dot moves left
    else letter = letter->right;                 // Dash moves right
  }
}

// Setup function for initializing the LCD and Morse code tree
void setup() {
  pinMode(Pin_10, INPUT);  // Initialize the button pin as input
  lcd.begin(16, 2);        // Set up the LCD with 16 columns and 2 rows
  lcd.setCursor(0, 0);     // Set the cursor to the start

  // Construct the Morse code binary tree with nodes
  root->left = createnode('e');
  root->right = createnode('t');
  root->left->left = createnode('i');
  root->left->right = createnode('a');
  root->right->left = createnode('n');
  root->right->right = createnode('m');
  root->left->left->left = createnode('s');
  root->left->left->right = createnode('u');
  root->left->right->left = createnode('r');
  root->left->right->right = createnode('w');
  root->right->left->left = createnode('d');
  root->right->left->right = createnode('k');
  root->right->right->left = createnode('g');
  root->right->right->right = createnode('o');
  root->left->left->left->left = createnode('h');
  root->left->left->left->right = createnode('v');
  root->left->left->right->left = createnode('f');
  root->left->right->left->left = createnode('l');
  root->left->right->right->left = createnode('p');
  root->left->right->right->right = createnode('j');
  root->right->left->left->left = createnode('b');
  root->right->left->left->right = createnode('x');
  root->right->left->right->left = createnode('c');
  root->right->left->right->right = createnode('y');
  root->right->right->left->left = createnode('z');
  root->right->right->left->right = createnode('q');
  root->left->right->left->right = createnode('*');
  root->left->right->left->right->left = createnode(' ');
}

// Main loop to handle button press, Morse code input, and debouncing
void loop() {
  // Debouncing logic
  int reading = digitalRead(Pin_10);  // Read the current state of the button

  // If the button state has changed, reset the debounce timer
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // If the button state remains stable for the debounce delay period
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {  // If the stable state differs from the previous state
      buttonState = reading;       // Update the button state

      if (buttonState == HIGH) {  // When the button is pressed
        if (start == 1) {
          press_start = millis();  // Record the start time of the press
          start = 0;
        }
        hold_time = millis() - press_start;  // Calculate the hold duration
      }

      if (hold_time > 0) {  // Determine the length of the press
        if ((hold_time / 1000) > 0.5) {  // Long press detected
          lcd.clear();  // Clear the LCD screen
          bottom_column = 0;
          len = 0;
          column = -1;
        }
        else if ((hold_time / 1000) > 0.15) {  // Dash detected
          lcd.print("-");
          tree_map[len++] = 1;  // Add dash to the map
        }
        else {  // Dot detected
          lcd.print(".");
          tree_map[len++] = 0;  // Add dot to the map
        }
        column = column + 1;
        lcd.setCursor(column, 0);
        hold_time = 0;
        start = 1;
      }
    }
  }

  lastButtonState = reading;  // Update the last button state for next iteration

  // If the button is not pressed
  while (digitalRead(Pin_10) == LOW) {
    if (start == 1) {
      zero_time = millis();  // Record the time the button is released
      start = 0;
    }
    low_time = millis() - zero_time;

    // If the button is released for a long time and a sequence exists
    if (((low_time / 1000) > 0.75) && len != 0) {
      lcd.setCursor(0, 0);
      lcd.print("                ");  // Clear the current Morse input
      transverser(tree_map, len);     // Decode the Morse code
      lcd.setCursor(0, 0);
      column = 0;
      len = 0;
    }
  }
  start = 1;
}
