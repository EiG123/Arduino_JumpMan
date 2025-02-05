#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>

#define BUTTON_PIN 2
#define JUMP_DURATION 5

LiquidCrystal_I2C lcd(0x3F, 16, 2);

byte customPlayer[] = {
  B01110,
  B01010,
  B01110,
  B10100,
  B01110,
  B00101,
  B01010,
  B10001
};

byte customPowerUp[] = {
  B11111,
  B00100,
  B01110,
  B01010,
  B01010,
  B01110,
  B00100,
  B00100
};

int playerPos = 1;
int obstaclePos = 15;
int obstaclePos1 = 20;
int obstaclePosUp = 18;
int powerUpPos = -1;
bool isJumping = false;
int jumpCounter = 0;
int score = 0;
int iSecret;
int iSecret1;
int iSecretUp;
bool hasPowerUp = false;
int powerUpCounter = 0;

int dl = 200;
int posSCORE = 15;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  
  // สร้างตัวละครที่กำหนดเอง
  lcd.createChar(0, customPlayer);
  lcd.createChar(1, customPowerUp);
  
  startGame();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!isJumping) {
      isJumping = true;
      jumpCounter = 0;
    }
  }

  if (isJumping) {
    jumpCounter++;
    if (jumpCounter > JUMP_DURATION) {
      isJumping = false;
    }
  }

  // Generate power-up
  if (powerUpPos < 0 && rand() % 95 == 0) { // Randomly generate power-up
    powerUpPos = 15;
  }

  if (powerUpPos >= 0) {
    powerUpPos--;
    if (powerUpPos < 0 && !hasPowerUp) {
      powerUpPos = -1; // Remove power-up when it goes off screen
    }
  }

  // Check collision with obstacles
  if (checkCollision(obstaclePos, iSecret) || checkCollision(obstaclePos1, iSecret1) || checkCollisionUp(obstaclePosUp, iSecretUp)) {
    gameOver();
  }

  // Check collision with power-up
  if (powerUpPos == playerPos && !isJumping) {
    hasPowerUp = true;
    powerUpCounter = 50; // Power-up lasts for 50 cycles
    powerUpPos = -1; // Remove power-up after collecting
  }

  // Update display
  lcd.clear();
  if (score >= 0 && score < 10) {
    lcd.setCursor(posSCORE, 0);
    lcd.print(score);
  } else if (score >= 10 && score < 100) {
    lcd.setCursor(posSCORE - 1, 0);
    lcd.print(score);
  } else if (score >= 100 && score < 1000) {
    lcd.setCursor(posSCORE - 2, 0);
    lcd.print(score);
  }

  lcd.setCursor(playerPos, 1);
  if (isJumping) {
    lcd.print(" ");
    lcd.setCursor(playerPos, 0);
    lcd.write(byte(0)); // ใช้ customPlayer
  } else {
    lcd.write(byte(0)); // ใช้ customPlayer
  }

  // Move the obstacles
  moveObstacle(obstaclePos, iSecret);
  if(score >= 10){

    moveObstacle(obstaclePos1, iSecret1);
  }
  //moveObstacleUp(obstaclePosUp, iSecretUp);
  

  if (powerUpPos >= 0 && obstaclePos != powerUpPos && obstaclePos1 != powerUpPos) {
    lcd.setCursor(powerUpPos, 1);
    lcd.write(byte(1));
  }

  if (hasPowerUp) {
    powerUpCounter--;
    if (powerUpCounter <= 0) {
      hasPowerUp = false;
    }
  }

  delay(dl);
  if (score == 10) {
    dl = 150;
  } else if (score == 20) {
    dl = 100;
  } else if (score == 30) {
    dl = 75;
  } else if (score == 50) {
    dl = 50;
  } else if (score == 100) {
    dl = 25;
  } else if (score == 500){
    dl = 20;
  } else if (score == 750){
    dl = 15;
  } else if (score == 800){
    dl = 10;
  } else if (score == 900){
    dl = 5;
  } else if (score == 990){
    dl = 1;
  }
}

void waitForButtonPress() {
  while (digitalRead(BUTTON_PIN) == HIGH) {
    // Wait until the button is pressed
  }
}

void gameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  delay(2000);  // Show game over screen for 2 seconds
  waitForButtonPress();  // Wait for the button press to restart
  startGame();  // Restart the game
}

void startGame() {
  score = 0;
  playerPos = 1;
  obstaclePos = 15;
  obstaclePos1 = 20;
  obstaclePosUp = 18;
  powerUpPos = -1;
  isJumping = false;
  jumpCounter = 0;
  hasPowerUp = false;
  powerUpCounter = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jump Man Game");
  lcd.setCursor(0, 1);
  lcd.print("Press to Start");
  waitForButtonPress();
  lcd.clear();
}

void moveObstacle(int &obstaclePos, int &iSecret) {
  obstaclePos--;
  if (obstaclePos < 0 && iSecret > 0) {
    iSecret--;
    obstaclePos = 0; // ป้องกันไม่ให้ตำแหน่งต่ำกว่า 0
  }
  if (obstaclePos < 0 && iSecret == 0) {
    obstaclePos = 15;
    iSecret = rand() % 3;
    score++;
  }

  lcd.setCursor(obstaclePos, 1);
  for (int i = 0; i <= iSecret; i++) {
    lcd.print("#");
  }
}

void moveObstacleUp(int &obstaclePos, int &iSecret) {
  obstaclePos--;
  if (obstaclePos < 0 && iSecret == 0) {
    obstaclePos = 15;
    iSecret = rand() % 2;
    score++;
  } else if (obstaclePos < 0 && iSecret == 1) {
    obstaclePos = 15;
    iSecret = rand() % 2;
  }

  lcd.setCursor(obstaclePos, 0);
  if(iSecret == 0){
    lcd.print("#");
  }
  lcd.setCursor(8, 1);
  lcd.print(iSecret);
}

bool checkCollision(int obstaclePos, int iSecret) {
  for (int i = 0; i <= iSecret; i++) {
    if (playerPos == 1 && obstaclePos + i == 1 && !isJumping && !hasPowerUp) {
      return true;
    }
  }
  return false;
}

bool checkCollisionUp(int obstaclePos, int iSecret){
  if (playerPos == 1 && obstaclePos == 1 && isJumping && !hasPowerUp) {
      return true;
  }
  return false;
}
