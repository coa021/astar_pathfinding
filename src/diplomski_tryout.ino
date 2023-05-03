#include <FastLED.h>

//==================================== PIN SETUP
// Dual h bridge motor pins
const int enablePin = 5;    //do i need this?
const int leftMotorInput1 = 6;
const int leftMotorInput2 = 7;
const int rightMotorInput1 = 8;
const int rightMotorInput2 = 9;

// variable for rotation time
int rotationDelayTime = 205;    //almost perfect 90 degree rotation
int forwardDelayTime = 700; //however i want
const int indicatorDelay = 20;  //TBD

// LED strip object
const int ledPin = 13;
const int numLeds = 23;
CRGB leds[numLeds];
//==================================== PIN SETUP

//==================================== MOVEMENT SETUP

void stopMovement()
{
  digitalWrite(leftMotorInput1, LOW);
  digitalWrite(rightMotorInput2, LOW);
  digitalWrite(rightMotorInput1, LOW);
  digitalWrite(leftMotorInput2, LOW);


  delay(1500);
}


void rotateRight() // Turn Right Function for Motor Driver.
{
  digitalWrite(rightMotorInput1, LOW);
  digitalWrite(leftMotorInput1, HIGH);
  digitalWrite(rightMotorInput2, HIGH);
  digitalWrite(leftMotorInput2, LOW);

  delay(rotationDelayTime);


  stopMovement();

  Serial.println("rotateRight()\n");
}

void moveForward() // Move Backward Function for Motor Driver.
{
  digitalWrite(rightMotorInput1, LOW);
  digitalWrite(rightMotorInput2, HIGH);
  digitalWrite(leftMotorInput1, LOW);
  digitalWrite(leftMotorInput2, HIGH);

  delay(forwardDelayTime);

  stopMovement();

  Serial.println("moveForward()\n");
}
//==================================== MOVEMENT SETUP


//==================================== ALGO SETUP
int map_width{5};
int map_height{5};

int direction{1}; //starting direction is always right
struct Node
{
  bool b_Obstacle{false};
  int x;
  int y;
  char value;
  Node *neighbours[4];
  int num_neighbours{0};

  bool b_Path_visited{false};
};

Node **nodes;
Node *startNode{nullptr};
Node *endNode{nullptr};

bool initialize()
{
  nodes = new Node *[map_width];
  char hardcoded_graph[][5] = {
    {'S', 'o', 'x', ' ', 'x'},
    {'x', 'o', 'x', ' ', 'x'},
    {'o', 'o', 'x', ' ', ' '},
    {'o', 'x', 'o', 'o', 'o'},
    {'o', 'o', 'o', 'x', 'o'},
  };

  for (int x = 0; x < map_width; x++) {
    nodes[x] = new Node[map_height];
    for (int y = 0; y < map_height; y++) {
      Node node;
      node.x = x;
      node.y = y;
      node.value = hardcoded_graph[y][x]; //or should it be x,y?
      node.b_Obstacle = node.value == 'x' ? true : false;

      nodes[x][y] = node;
      //            std::cout << nodes[x][y].value << " ";
    }
  }

  for (int x = 0; x < map_width; x++) {
    for (int y = 0; y < map_height; y++) {
      Node *node = &nodes[y][x];
      // checking for bounds
      // if node is on the edges
      // theres no possibility to go in some directions

      if (y > 0)
        node->neighbours[node->num_neighbours++] = &nodes[y - 1][x];
      if (y < map_height - 1)
        node->neighbours[node->num_neighbours++] = &nodes[y + 1][x];
      if (x > 0)
        node->neighbours[node->num_neighbours++] = &nodes[y][x - 1];
      if (x < map_width - 1)
        node->neighbours[node->num_neighbours++] = &nodes[y][x + 1];
    }
  }

  startNode = &nodes[0][0];
  //    startNode->value = 'S';
  //manual position of start/end

  endNode = &nodes[map_width - 1]
            [map_height - 1]; //check this, should change values if they are set wrong
  //    endNode->value = 'E';
  return true;
}

void move_robot()
{
  // Set the current node to the start node
  Node *currentNode = startNode;
  do {
    Node *nextNode = nullptr;
    for (const auto &neighbour : currentNode->neighbours) {
      if (!neighbour->b_Path_visited && neighbour->value == 'o') {
        nextNode = neighbour;
        neighbour->b_Path_visited = true;
        break;
      }
    }

    // Calculate the direction to move
    int xDiff = nextNode->x - currentNode->x;
    int yDiff = nextNode->y - currentNode->y;

    int nextDirection{};

    if (xDiff == 0 && yDiff == -1) {
      // Moving down
      nextDirection = 0;
    } else if (xDiff == 0 && yDiff == 1) {
      // Moving up
      nextDirection = 2;
    } else if (xDiff == -1 && yDiff == 0) {
      // Moving left
      nextDirection = 3;
    } else if (xDiff == 1 && yDiff == 0) {
      // Moving right
      nextDirection = 1;
    }

    while (direction != nextDirection) {
      rotateRight();
      direction = (direction + 1) % 4;
    }

    moveForward();
    currentNode = nextNode;

  } while (currentNode != endNode);
}

//==================================== ALGO SETUP


void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  //setting up shift register

  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);

  pinMode(leftMotorInput1, OUTPUT);
  pinMode(leftMotorInput2, OUTPUT);
  pinMode(rightMotorInput1, OUTPUT);
  pinMode(rightMotorInput2, OUTPUT);

  // FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLeds);
  FastLED.show();

  initialize();
  move_robot();
}

void loop() {
  // put your main code here, to run repeatedly:

  while (1)
  {}  //do nothing just loop
}
