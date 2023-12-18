// base game reference Tank Wars
#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
//lcd pins
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 4;
const byte d7 = 13;

const byte lcdBrightnessPin = 5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const byte buzzerPin = 17;
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

const byte xPin = A0;
const byte yPin = A1;

const byte ledPin = 16;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const byte matrixSize = 16;

const byte initialxPosition = matrixSize - 2;
const byte initialyPosition = 0;
byte cameraXPosition = initialxPosition;
byte cameraYPosition = initialyPosition;
byte xPos = initialxPosition;
byte yPos = initialyPosition;
byte xLastPos = 0;
byte yLastPos = 0;
byte xBoundary = 6;
byte yBoundary = 4;

const int minThreshold = 200;
const int maxThreshold = 600;
const int maxMatrixBrightness = 5;
const byte moveInterval = 100;
unsigned long long lastMoved = 0;

bool matrixChanged = true;
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

byte aimMatrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte leftRobot[] = {
  B00000,
  B01011,
  B01110,
  B00101,
  B01100,
  B01110,
  B01011,
  B00010
};

byte rightRobot[] = {
  B00000,
  B11000,
  B01100,
  B10100,
  B00110,
  B01110,
  B11010,
  B01000
};

byte rightArrow[] = {
  B11000,
  B11100,
  B00110,
  B00011,
  B00011,
  B00110,
  B11100,
  B11000
};

byte hearth[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

unsigned long blinkTime = 0;
const unsigned long blinkPeriod = 500;

const byte unusedPin = 19;

byte oldHight = 1;

const byte aimPin = 3;
const byte shootPin = 2;

volatile int currentButton = -1;
volatile bool possiblePress = false;
volatile bool shouldStartCounter = false;
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 75;
bool buttonReading = HIGH;
bool lastReading = HIGH;

bool aimMode = false;
byte aimDirection = 2;
byte oldAdimDirection = -1;
unsigned long aimTime = 0;
const unsigned long aimPeriod = 1500;

byte left = 0;
byte right = 1;
byte up = 2;
byte down = 3;
bool oldIncrement = 0;
const byte aimPosibilities = 5;

bool startShooting = false;

bool firstReading = true;
bool oldAimMode = false;

int xBullet = -1;
int yBullet = -1;

unsigned long bulletTime = 0;

byte bulletPeriod = 250;

bool firstShoot = false;

const byte maxHeight = 10;
byte currentMaxHeight = 0;
const byte minHeight = 0;
byte startPrint = 0;
byte nextMessage = 0;
byte introMessagesCount = 3;
byte aboutMessagesCount = 6;
byte displayMessagesCount = 2;
bool leftClick = false;
bool rightClick = false;

unsigned long messageTime = 0;
int timer = 0;
const char* introMessages[] = { "RobotFall", "Press any button", "to start" };
const char* optionsMessages[] = { "Start game", "Settings", "About","HowToPlay"};
const char* buttonsMessages[] = { "LeftClk -enter", "RightClk -back" };
const char* settingsMessages[] = { "8x8Intensity", "LCDIntensity","BuzzerOFF"};
const char* aboutMessages[] = { "RobotFall", "Made by", "George Popescu", "github.com/", "GeorgePopescu318","RightClk -back"};
const char* displayMessages[] = { "Operator: ", "Hearts:" };
const char* howToPlayMessages[] = {"How to play","To move use","joystick for ","left","and right","To shoot use","rightclk aim","leftclk shoot","Objective","Go up the hill","defend your home","from aliens","RightClk -back"};
const char* playerName[] = { "AGP" };
const char* endGameMessages[] = { "END" };
const int messagePeriod = 2000;
char test[] = "test";
const byte howtToPlayMessagesCounter = 13;
byte incOptions = 0;
byte oldIncOptions = 255;
bool inOption = 0;
bool optionsMode = 0;
bool updateDisplay = true;
bool moveDone = 0;
bool initDisplay = true;
const int movePeriod = 650;
unsigned long moveTime = 650;
bool movePrinted = false;
byte lcdBrightnessPercentage = 0; 
byte lcdBrightness = 0;
byte lcdBrightnessEEPROMbyte = 0;
byte matrixBrightness = 0;
byte matrixBrightnessEEPROMbyte = 1;
bool firstDisplay = false;
bool endDisplay = false;
//EEPROM
//0 - LCD brigthness
//1 - matrix brigthness
//2 - makeSoud
bool settingsMode = 0;
int points = 0;
byte maxLives = 3;
byte livesRemaining = 0;
const int livesPeriod = 5000;
unsigned long livesTime = 0;
byte incSettings = 0;
bool firstAboutPrint = false;
bool buttonsMode = true;

const uint64_t FULL = {
  0xffffffffffffffff
};


const uint64_t CLEAR = {
  0x0000000000000000
};

const uint64_t IMAGES[]= {
  0x4242ffc3db427f01,
  0x03076efc980c0c18,
  0x3c18181800183c18,
  0x1800187860667e3c
};
bool matrixDrawing = false;
void (*myFunction[])(void){
  game,
  settings,
  about,
  howToPlay
};
unsigned long timeStart = 0;
unsigned long timeEnd = 0;
unsigned int enemyMoveTime = 1000;
unsigned int enemySpawnTime = 1500;
unsigned long enemySpawnPeriod = 0;
bool spawnEnemies = false;
class Enemy{
  public:
    byte x;
    byte y;
    bool visible;
    unsigned long long period = 0;
  Enemy(byte initalX): x(initalX),y(0){
    matrix[x][y] = 1;
    visible = true;
  }
  void move(){
     if (matrix[x][y] == 0){
      this->visible = false;
      if (y >= matrixSize - 1){
        livesRemaining--;
        updateDisplay = true;
      }else{
        if (y != yPos && x!= xPos){
          points-=100;
        }else{
          if (y == yPos && x == xPos){
            livesRemaining--;
            updateDisplay = true;
          }
        }
      }
      
      this->x = 255;
      this->y = 255;
    }
    if(millis() - period >= enemyMoveTime){
      period = millis();
      matrix[x][y] = 0;
      if (y < matrixSize){
        this->y++;
      }else{
        this->visible = false;
        this->x = 255;
        this->y = 255;
      }
      if (matrix[x][y] == 1){
        this->visible = false;
        if (x!=xPos && y!=yPos){
          matrix[x][y] = 0;
        }
        if (y >= 15 ||(x == xPos && y == yPos)){
          livesRemaining--;
          updateDisplay = true;
        }
        this->x = 255;
        this->y = 255;
        
      }else{
        matrix[x][y] = 1;
      }
    }
  }
  ~Enemy(){
    this->visible = false;
  }
};
byte maxEnemies = 5;
byte makeSound = 1;
byte enemiesNo = 0;
const int introSound = 2345;
const int settingsSound = 1500;
const int shootSound = 896;
const int soundDuration = 500;
const byte buzzerBrightnessEEPROMbyte = 2;
Enemy* enemies[10];

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  lc.shutdown(0, false);
  EEPROM.get(matrixBrightnessEEPROMbyte, matrixBrightness);
  lc.clearDisplay(0);

  matrix[xPos][yPos] = 1;

  randomSeed(analogRead(unusedPin));

  pinMode(aimPin, INPUT_PULLUP);
  pinMode(shootPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(aimPin), handleInterruptAim, FALLING);
  attachInterrupt(digitalPinToInterrupt(shootPin), handleInterruptShoot, FALLING);

  pinMode(lcdBrightnessPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.createChar(0, leftRobot);
  lcd.createChar(1, rightRobot);
  lcd.createChar(2, rightArrow);
  lcd.createChar(3, hearth);
  EEPROM.get(lcdBrightnessEEPROMbyte, lcdBrightness);
  lcdBrightnessPercentage = map(lcdBrightness, 0, 254, 0, 9);
  pinMode(buzzerPin, OUTPUT);
  EEPROM.get(buzzerBrightnessEEPROMbyte,makeSound);
  if (makeSound == 1){
    tone(buzzerPin, introSound,soundDuration);
  }
}
void loop() {
  Menu();
  
}

void Menu() {
  if (optionsMode == 1) {
    updatePositions(incOptions, optionsMode);  //menu
    incOptions %= 4;
    if (moveDone == 1) {
      if (movePrinted == false) {
        lcd.setCursor(2, 0);
        lcd.clear();
        movePrinted = true;
      }
      if (millis() - moveTime >= movePeriod) {
        movePrinted = false;
      }
    }
  }
  analogWrite(lcdBrightnessPin, lcdBrightness);
  lc.setIntensity(0, matrixBrightness);
  buttonDebounce();
  if (leftClick == false && rightClick == false && startPrint == 0) {
    introMessage();

  } else {
    if (startPrint != 2) {
      startPrint = 1;
      leftClick = false;
      rightClick = false;
      nextMessage = 0;
      lcd.clear();
    }

    if (startPrint == 1) {

      startPrint = 2;
      optionsMode = 1;
    }
    if (startPrint != 0) {
    }
  }
  if (startPrint == 2) {
    if (buttonsMode == true) {
      buttonsFunction();
    }
    if (leftClick == 1 && inOption == 0) {
      inOption = 1;
      leftClick = 0;
    }
    if (inOption == 1) {
      optionsMode = 0;
      myFunction[incOptions]();
    } else {
      optionsMode = 1;
      if (oldIncOptions != incOptions) {
        displayImage(CLEAR);
        displayImage(IMAGES[incOptions]);
        oldIncOptions = incOptions;
      }
      choicesMessage(optionsMessages[incOptions]);
    }
  }
}

void introMessage() {

  lcd.setCursor(2, 0);
  lcd.print(introMessages[0]);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print(introMessages[nextMessage + 1]);
  if (millis() - messageTime >= messagePeriod) {
    nextMessage++;
    nextMessage %= introMessagesCount - 1;
    messageTime = millis();
    lcd.clear();
  }
}

void choicesMessage(const char printedMessage[]) {
  lcd.setCursor(0, 0);
  lcd.write(byte(2));
  lcd.write(byte(2));
  lcd.setCursor(2, 0);
  lcd.print(printedMessage);
  lcd.setCursor(14, 0);
  lcd.write(byte(2));
  lcd.write(byte(2));
}

void buttonsFunction() {
  lcd.setCursor(0, 1);
  if (millis() - messageTime >= messagePeriod) {
    nextMessage++;
    nextMessage %= 2;
    messageTime = millis();
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.setCursor(0, 1);
  }
  lcd.print(buttonsMessages[nextMessage]);
}

void settings() {
  choicesMessage(settingsMessages[incSettings]);
  buttonDebounce();
  if (leftClick == 1) {
    settingsMode = 0;
  } else {
    settingsMode = true;
  }
  
  if (leftClick == true) {
    settingsMode = false;
    buttonsMode = false;
    lcd.setCursor(0, 1);
    lcd.clear();
    if (incSettings == 0) {
      if (firstDisplay == false) {
        displayImage(FULL);
        lcd.setCursor(2, 1);
        firstDisplay = true;
      }
      lcd.setCursor(2, 1);
      lcd.print(matrixBrightness);
      updatePositions(matrixBrightness, true);  //intensity
      if (matrixBrightness > maxMatrixBrightness) {
        matrixBrightness = maxMatrixBrightness;
      }
      if (matrixBrightness < 1) {
        matrixBrightness = 1;
      }
      EEPROM.put(matrixBrightnessEEPROMbyte, matrixBrightness);
      if (rightClick == true) {
        inOption = 0;
        rightClick = false;
        buttonsMode = true;
        firstDisplay = false;
        settingsMode = true;
        displayImage(CLEAR);
      }
    }
    if (incSettings == 1) {
      if (firstDisplay == false) {
        lcd.clear();
        firstDisplay = true;
      }
      lcd.setCursor(2, 1);
      lcd.print(lcdBrightnessPercentage);
      updatePositions(lcdBrightnessPercentage, true);  //intensity
      if (lcdBrightnessPercentage > 9) {
        lcdBrightnessPercentage = 9;
      }
      if (lcdBrightnessPercentage < 1) {
        lcdBrightnessPercentage = 1;
      }
      lcdBrightness = map(lcdBrightnessPercentage, 0, 9, 0, 254);
      if (rightClick == true) {
        EEPROM.put(lcdBrightnessEEPROMbyte, lcdBrightness);
        leftClick = false;
        rightClick = false;
        settingsMode = true;
        buttonsMode = true;
        firstDisplay = false;
        lcd.clear();
      }
    }
    if (incSettings == 2) {
      if (firstDisplay == false) {
        lcd.clear();
        firstDisplay = true;
      }
      lcd.setCursor(2, 1);
      updatePositions(makeSound, true);  //buzzer
      makeSound %= 2;
      Serial.println(makeSound);
      lcd.print(makeSound);
      
      if (rightClick == true) {
        EEPROM.put(buzzerBrightnessEEPROMbyte, makeSound);
        leftClick = false;
        rightClick = false;
        settingsMode = true;
        buttonsMode = true;
        firstDisplay = false;
        lcd.clear();
      }
    }
  }
  if (buttonsMode == true) {
    buttonsFunction();
  }
  if (settingsMode == true) {
    updatePositions(incSettings, settingsMode);  //settings

    if (moveDone == 1) {
      incSettings %= 3;
      if (movePrinted == false) {
        lcd.setCursor(2, 0);
        lcd.clear();
        movePrinted = true;
      }
    }
  }
  if (rightClick == true) {
    rightClick = false;
    leftClick = false;
    inOption = false;
    lcd.clear();
  }
}

void about() {
  buttonDebounce();
  if (firstAboutPrint == false){
    nextMessage = 0;
    firstAboutPrint = true;
    lcd.clear();
  }
  if (rightClick == true) {
    lcd.clear();
    buttonsMode = true;
    rightClick = false;
    leftClick = false;
    inOption = false;
    nextMessage = 0;
    firstAboutPrint = false;
  } else {
    buttonsMode = false;
    lcd.setCursor(0, 0);
    lcd.print(aboutMessages[nextMessage]);
    lcd.setCursor(0, 1);
    lcd.print(aboutMessages[nextMessage + 1]);
    if (millis() - messageTime >= messagePeriod) {
      nextMessage++;
      nextMessage %= aboutMessagesCount - 1;
      messageTime = millis();
      lcd.clear();
    }
  }
}
void howToPlay() {
  buttonDebounce();
  if (firstAboutPrint == false){
    nextMessage = 0;
    firstAboutPrint = true;
    lcd.clear();
  }
  if (rightClick == true) {
    lcd.clear();
    buttonsMode = true;
    rightClick = false;
    leftClick = false;
    inOption = false;
    nextMessage = 0;
    firstAboutPrint = false;
  } else {
    buttonsMode = false;
    lcd.setCursor(0, 0);
    lcd.print(howToPlayMessages[nextMessage]);
    lcd.setCursor(0, 1);
    lcd.print(howToPlayMessages[nextMessage + 1]);
    if (millis() - messageTime >= messagePeriod) {
      nextMessage++;
      nextMessage %= howtToPlayMessagesCounter - 1;
      messageTime = millis();
      lcd.clear();
    }
  }
}
void displayImage(uint64_t image) {  // from online
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
  }
}

void gameDisplay() {
  if (livesRemaining == 1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  if (initDisplay == true) {
    generateMap();
    timeStart = millis();
    cameraXPosition = initialxPosition;
    cameraYPosition = initialyPosition;
    xPos = initialxPosition;
    yPos = initialyPosition;
    xLastPos = 0;
    yLastPos = 0;
    livesRemaining = maxLives;
    lcd.clear();
    buttonsMode = 0;
    lcd.setCursor(0, 0);
    lcd.print(displayMessages[0]);
    lcd.print(playerName[0]);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.print(displayMessages[1]);
    initDisplay = false;
  }
  if (updateDisplay == true) {
    lcd.setCursor(7, 1);
    if (livesRemaining > 0) {
      for (byte i = 0; i < livesRemaining; i++) {
        lcd.write(byte(3));
      }
      lcd.write(" ");
    }
    updateDisplay = false;
  }
}

void endGame() {
 
  buttonDebounce();
  buttonsMode = 1;
  displayImage(CLEAR);
  if (endDisplay == false) {
    lcd.clear();
    endDisplay = true;
    timeEnd = millis();
    points += int((timeEnd - timeStart)*0.01);
  }
  lcd.setCursor(0, 0);
  lcd.print(endGameMessages[0]);
  lcd.print(points);
  if (rightClick == 1) {
    displayImage(CLEAR);
    generateMap();
    for (int i =0; i<= maxEnemies;i++){
      delete [] enemies[i];
      enemies[i] = NULL;
    }
    points = 0;
    livesRemaining = 0;
    endDisplay = false;
    inOption = 0;
    rightClick = 0;
    settingsMode = 1;
    leftClick = 0;
    initDisplay = true;
    updateDisplay = true;
  }
  if (leftClick == 1) {
    generateMap();
    points = 0;
    displayImage(CLEAR);
    livesRemaining = 0;
    initDisplay = true;
    rightClick = 0;
    leftClick = 0;
    endDisplay = false;
    updateDisplay = true;
    game();
  }
}

void enterName(){

}

void game() {

  if (endDisplay == false) {
    updateDisplay = true;
    gameDisplay();
      if (livesRemaining == 0) {
        rightClick = 0;
        leftClick = 0;
        endGame();
    }

    if (yPos > 8){
      spawnEnemies = true;
    }

    if (spawnEnemies == true){
      
      if (millis() - enemySpawnPeriod >= enemySpawnTime){
        enemySpawnPeriod = millis();
        byte position = random(matrixSize -  currentMaxHeight-4,matrixSize - currentMaxHeight-1);
        bool created = false;
        for(int i = 0; i<= maxEnemies;i++){
          if(enemies[i]->visible == false ){
            delete [] enemies[i];
            enemies[i] = NULL;
          }
          if (enemies[i] == NULL && created == false){
            enemies[i] = new Enemy(position);
            created = true;
          }
        }
        enemySpawnTime -=150;
      }
        enemiesNo++;
        if(enemiesNo == maxEnemies){
          enemiesNo = 0;
        }
          if(enemies[enemiesNo]->visible == true){
            enemies[enemiesNo]->move();
            updateMatrix();
          }else{
            delete [] enemies[enemiesNo];
            enemies[enemiesNo] = NULL;
          }

        //   bool allDead = false;
        // for (int i = 0; i<= maxEnemies; i++){
        //   if (enemies[i]->visible == true){
        //     allDead = true;
        //   }
        // }
        // if (allDead == false){
        //   endGame();
        // }

    }
    
    optionsMode = 0;
    if (currentButton != -1) {
      buttonDebounce();
    }

    while (matrix[xPos + 1][yPos] == 0) { //gravity
      matrix[xPos][yPos] = 0;
      cameraXPosition++;
      xPos++;
      matrixChanged = true;
    }

    if (millis() - blinkTime >= blinkPeriod) {

      blinkTime = millis();

      if (matrix[xPos][yPos] == 1) {
        matrix[xPos][yPos] = 0;
      } else {
        matrix[xPos][yPos] = 1;
      }
      matrixChanged = true;
    }

    if (millis() - lastMoved > moveInterval) {

      updatePositions(aimDirection, aimMode);  //game
      aimDirection %= aimPosibilities;
      lastMoved = millis();
    }
    if (matrixChanged == true) {
      updateMatrix();
      matrixChanged = false;
    }

    if (aimMode == 1) {
      aim();
    }

    resetMatrix();

    if (millis() - aimTime >= aimPeriod) {
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          if (aimMatrix[row][col] == 1) {
            if (matrix[row][col] == 1) {
              matrix[row][col] = 0;
            } else {
              matrix[row][col] = 1;
            }
          }
        }
      }
      aimTime = millis();
    }

    if (startShooting) {
      shoot();
    }
  } else {
    endGame();
  }
}

void updateMatrix() {
  if (cameraXPosition > matrixSize - yBoundary/2){
    cameraXPosition = matrixSize - yBoundary/2;
  }
  for (int row = cameraXPosition - xBoundary; row < cameraXPosition+xBoundary/2; row++) {
    for (int col = cameraYPosition; col < cameraYPosition+yBoundary*2; col++) {
      lc.setLed(0, row-(cameraXPosition - xBoundary), col-cameraYPosition, matrix[row][col]);
    }
  }
}

void updatePositions(byte& increment, bool specialCommand) {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  xLastPos = xPos;
  yLastPos = yPos;
  byte lastTerrain = 0;
  
    if (millis() - moveTime >= movePeriod && moveDone == 1) {
      moveTime = millis();
      moveDone = 0;
      movePrinted = false;
    }

  if (moveDone == 0) {
    if (xValue < minThreshold && yValue > maxThreshold) {
      if (specialCommand == false) {
        
        if (xPos > 0 && (matrix[xPos - 1][yPos] == 0 && matrix[xPos - 1][yPos - 1] == 0 && matrix[xPos][yPos - 1] == 1)) {
          xPos--;
          yPos--;
          moveDone = 1;
          cameraXPosition = xPos + 1;
        }
        if (cameraYPosition + matrixSize/2 < matrixSize){
              if (cameraYPosition <= yPos){//camera can see 8 pixels in front
              cameraYPosition++;
              matrixChanged = true;
            }

          }
      }
    }

    if (xValue < minThreshold && yValue < minThreshold) {
      if (specialCommand == false) {
        if (xPos > 0 && (matrix[xPos - 1][yPos] == 0 && matrix[xPos - 1][yPos + 1] == 0 && matrix[xPos][yPos + 1] == 1)) {
          xPos--;
          yPos++;
          moveDone = 1;
          cameraXPosition = xPos + 1;
        }
      }
    }

    if (yValue < minThreshold) {
      if (specialCommand == false) {
          if (matrix[xPos][yPos + 1] == 0){
              if (cameraYPosition + matrixSize/2 < matrixSize){
              if (cameraYPosition<= yPos){//camera can see 8 pixels in front
              cameraYPosition++;
              matrixChanged = true;
            }}
              if (yPos < matrixSize - 1 && moveDone == 0){
              yPos++;
              }
            
          }
        
      } else {
        if (makeSound == 1){
          tone(buzzerPin, settingsSound,soundDuration);
        }
        increment++;
        moveDone = 1;
      }
    }

    if (yValue > maxThreshold) {
      if (specialCommand == false) {
          if (matrix[xPos][yPos - 1] == 0){
            if (cameraYPosition >= yPos){
              cameraYPosition--;
              matrixChanged = true;
            }
            if (moveDone == 0){
              yPos--;
              }

          }
        
      } else {
        if (makeSound == 1){
          tone(buzzerPin, settingsSound,soundDuration);
        }
        increment--;
        moveDone = 1;
      }
    }
  }

  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = lastTerrain;
    matrix[xPos][yPos] = 1;
  }
  // Serial.print("cameraY");
  // Serial.println(cameraYPosition);
  // // Serial.print("camerax");
  // // Serial.println(cameraXPosition);
  // Serial.print("ypos");
  // Serial.println(yPos);
  // // Serial.print("xpos");
  // // Serial.println(xPos);
}

void shoot() {

  if (firstShoot == true) {
    if (makeSound == 1){
      tone(buzzerPin,shootSound,soundDuration);
    }
    aimMode = 0;
    resetMatrix();
    firstShoot = false;
    switch (oldAdimDirection) {
      case 2:
        {
          xBullet = xPos - 1;
          yBullet = yPos;
        }
        break;
      case 3:
        {
          xBullet = xPos - 1;
          yBullet = yPos + 1;
        }
        break;
      case 4:
        {
          xBullet = xPos;
          yBullet = yPos + 1;
        }
        break;
      case 1:
        {
          xBullet = xPos - 1;
          yBullet = yPos - 1;
        }
        break;
      case 0:
        {
          xBullet = xPos;
          yBullet = yPos - 1;
        }
        break;
    }
    if (matrix[xBullet][yBullet] == 1) {
      
      matrix[xBullet][yBullet] = 0;
      xBullet = -1;
      yBullet = -1;
      startShooting = false;
      firstShoot = false;
    } else {
      matrix[xBullet][yBullet] = 1;
    }
  }


  if (millis() - bulletTime >= bulletPeriod) {
    if (xBullet != -1 && yBullet != -1) {
      switch (oldAdimDirection) {

        case 2:
          {
            matrix[xBullet][yBullet] = 0;
            xBullet--;
            if (matrix[xBullet][yBullet] == 1) {
              matrix[xBullet][yBullet] = 0;
              xBullet = -1;
              yBullet = -1;
              startShooting = false;
              firstShoot = false;
            } else {
              matrix[xBullet][yBullet] = 1;
            }
          }
          break;
        case 3:
          {
            matrix[xBullet][yBullet] = 0;
            xBullet--;
            yBullet++;
            if (matrix[xBullet][yBullet] == 1) {
              matrix[xBullet][yBullet] = 0;
              xBullet = -1;
              yBullet = -1;
              startShooting = false;
              firstShoot = false;
            } else {
              matrix[xBullet][yBullet] = 1;
            }
          }
          break;
        case 4:
          {
            matrix[xBullet][yBullet] = 0;
            yBullet++;
            if (matrix[xBullet][yBullet] == 1) {
              matrix[xBullet][yBullet] = 0;
              xBullet = -1;
              yBullet = -1;
              startShooting = false;
              firstShoot = false;
            } else {
              matrix[xBullet][yBullet] = 1;
            }
          }
          break;
        case 1:
          {
            matrix[xBullet][yBullet] = 0;
            xBullet--;
            yBullet--;
            if (matrix[xBullet][yBullet] == 1) {
              matrix[xBullet][yBullet] = 0;
              xBullet = -1;
              yBullet = -1;
              startShooting = false;
              firstShoot = false;
            } else {
              matrix[xBullet][yBullet] = 1;
            }
          }
          break;
        case 0:
          {
            matrix[xBullet][yBullet] = 0;
            xBullet = xPos;
            yBullet--;
            if (matrix[xBullet][yBullet] == 1) {
              matrix[xBullet][yBullet] = 0;
              xBullet = -1;
              yBullet = -1;
              startShooting = false;
              firstShoot = false;
            } else {
              matrix[xBullet][yBullet] = 1;
            }
          }
          break;
      }
    }
    if (yBullet < 0 || yBullet > matrixSize - 2 || xBullet > matrixSize - 2 || xBullet < 0) {

      if (xBullet != matrixSize - 1) {
        matrix[xBullet][yBullet] = 0;
      }

      xBullet = -10;
      yBullet = -10;
      startShooting = false;
      firstShoot = false;
    }

    matrixChanged = true;
    bulletTime = millis();
  }
}

void resetMatrix() {
  if (aimMode == false && oldAimMode == true) {
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        if (aimMatrix[row][col] == 1 && matrix[row][col] == 1) {
          aimMatrix[row][col] = 0;
          matrix[row][col] = 0;
        }
      }
    }

    aimDirection = 2;
    firstReading = true;
  }
}

void aim() {
  bool changeDone = false;
  if (aimDirection != oldAdimDirection) {
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        if (aimMatrix[row][col] == 1 && matrix[row][col] == 1) {
          aimMatrix[row][col] = 0;
          matrix[row][col] = 0;
        }
      }
    }
    firstReading = true;
  }

  for (int row = matrixSize - 1; row >= 0; row--) {
    for (int col = 0; col < matrixSize - 1; col++) {
      switch (aimDirection) {
        case 2:
          {
            if (col == yPos && row < xPos) {
              if (firstReading == true) {
                if (matrix[row][col] == 0) {
                  aimMatrix[row][col] = 1;
                }
              }

              if (matrix[row][col] == 0 && aimMatrix[row][col] == 1) {
                matrix[row][col] = 1;
              }
              changeDone = true;
            }
          }
          break;
        case 3:
          {

            if (col + row == xPos + yPos && row < xPos) {
              if (firstReading == true) {
                if (matrix[row][col] == 0) {
                  aimMatrix[row][col] = 1;
                }
              }
              if (aimMatrix[row][col] == 1) {
                matrix[row][col] = 1;
              }
              changeDone = true;
            }
          }
          break;
        case 4:
          {
            if (row == xPos && col > yPos) {
              if (firstReading == true) {
                if (matrix[row][col] == 0) {
                  aimMatrix[row][col] = 1;
                }
              }

              if (matrix[row][col] == 0 && aimMatrix[row][col] == 1) {
                matrix[row][col] = 1;
              }
              changeDone = true;
            }
          }
          break;
        case 1:
          {
            if (abs(row - col) == abs(xPos - yPos) && row < xPos && col < yPos) {
              if (firstReading == true) {
                if (matrix[row][col] == 0) {
                  aimMatrix[row][col] = 1;
                }
              }

              if (matrix[row][col] == 0 && aimMatrix[row][col] == 1) {
                matrix[row][col] = 1;
              }
              changeDone = true;
            }
          }
          break;
        case 0:
          {
            if (row == xPos && col < yPos) {
              if (firstReading == true) {
                if (matrix[row][col] == 0) {
                  aimMatrix[row][col] = 1;
                }
              }

              if (matrix[row][col] == 0 && aimMatrix[row][col] == 1) {
                matrix[row][col] = 1;
              }
              changeDone = true;
            }
          }
          break;
      }
    }
  }
  if (changeDone == true) {
    matrixChanged = true;
  }
  oldAdimDirection = aimDirection;
  firstReading = false;
  oldAimMode = aimMode;
}
void generateMap() {
    int nextHight = 0;
    for (int col = 2; col < matrixSize; col++) {
      nextHight = random(oldHight, oldHight + 2);
      if (nextHight < minHeight){
        nextHight = minHeight;
      }
      if (nextHight > maxHeight){
        nextHight = maxHeight;
      }
      oldHight = nextHight;
      if (nextHight > currentMaxHeight){
        currentMaxHeight = nextHight;
      }
      for (int row = matrixSize - 2; row > matrixSize-nextHight-2; row--) {
          matrix[row][col] = 1;
      }
    }
}

void buttonDebounce() {

  if (possiblePress) {

    if (shouldStartCounter) {
      lastDebounceTime = millis();
      shouldStartCounter = false;
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {

      if (currentButton == 0) {
        buttonReading = digitalRead(aimPin);
      }

      if (currentButton == 1) {
        buttonReading = digitalRead(shootPin);
      }

      if (buttonReading == LOW) {
        if (currentButton == 0) {
          aimMode = !aimMode;
          rightClick = true;
        }
        if (currentButton == 1) {
          startShooting = true;
          firstShoot = true;
          leftClick = true;
        }
        lastReading = buttonReading;
        lastReading = false;
        currentButton = -1;
      }
      possiblePress = false;
    }
  }
}

void handleInterruptAim() {
  possiblePress = true;
  shouldStartCounter = true;
  currentButton = 0;
}

void handleInterruptShoot() {
  possiblePress = true;
  shouldStartCounter = true;
  currentButton = 1;
}