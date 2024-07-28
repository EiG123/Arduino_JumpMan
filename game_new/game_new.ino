#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>

#define BUTTON_PIN 2
#define JUMP_DURATION 5

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int playerPos = 1;
int obstaclePos = 15;
bool isJumping = false;
int jumpCounter = 0;
int score = 0;
int iSecret;

int dl = 200;
int posSCORE = 15;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
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

  // Move the obstacle
  obstaclePos--;
  if (iSecret == 0 && obstaclePos < 0) {
    obstaclePos = 15;
    score++;
  }else if (iSecret == 1 && obstaclePos < 0){
    iSecret--;
  }else if (iSecret == 2 && obstaclePos < 0){
    iSecret--;
  }

  // Check collision
  if(iSecret == 0 && playerPos == 1 && obstaclePos == 1 && !isJumping){
      gameOver();
  }else if (iSecret == 1 && playerPos == 1 && !isJumping){
      if(obstaclePos == 1 || obstaclePos == 0){
          gameOver();
      }
  }else if (iSecret == 2 && playerPos == 1 && !isJumping){
      if(obstaclePos == 1 || obstaclePos == 0 || obstaclePos == -1){
          gameOver();
      }
  }
  // Update display
  lcd.clear();
  lcd.setCursor(posSCORE, 0);
  lcd.print(score);
  

  lcd.setCursor(playerPos, 1);
  if (isJumping) {
    lcd.print(" ");
    lcd.setCursor(playerPos, 0);
    lcd.print("O");
  } else {
    lcd.print("O");
  }

  lcd.setCursor(obstaclePos, 1);
  if(obstaclePos == 15){
      iSecret = rand() % 3;
  }

  if (iSecret == 0){
      lcd.print("#");
    } else if (iSecret == 1) {
      lcd.print("##");
    } else if (iSecret == 2) {
      lcd.print("###");
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
  isJumping = false;
  jumpCounter = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jump Man Game");
  lcd.setCursor(0, 1);
  lcd.print("Press to Start");
  waitForButtonPress();
  lcd.clear();
}
