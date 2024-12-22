#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x26, 16, 2);

const int UP = 6;
const int DOWN = 7;
const int LEFT = 8;
const int RIGHT = 9;
const int Speed_Pin = A0;

int snakeX[32], snakeY[32];
int snakeLength = 3;
int foodX, foodY;
int direction = 0; // 0=右，1=下，2=左，3=上
bool gameOver = false;


byte bcd_code[10][4] = {
  {0,0,0,0},
  {0,0,0,1},
  {0,0,1,0},
  {0,0,1,1},
  {0,1,0,0},
  {0,1,0,1},
  {0,1,1,0},
  {0,1,1,1},
  {1,0,0,0},
  {1,0,0,1}
};

void setup()
{
  for(int a=2;a<8;a++) pinMode(a, OUTPUT);
  Serial.begin(9600);
  lcd.init();					
  lcd.backlight();
  lcd.setCursor(0,0);
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  
  randomSeed(analogRead(0));

  snakeX[0] = 3; snakeY[0] = 0;
  snakeX[1] = 2; snakeY[1] = 0;
  snakeX[2] = 1; snakeY[2] = 0;
  snakeLength = 3;
  direction = 0;

  spawnFood();
  lcd.clear();
}

void loop()
{
	if (snakeLength - 3 >= 9) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You Win ! ");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(snakeLength - 3);
    disp(snakeLength - 3);  
    while(1);
  }
  
  disp(snakeLength - 3);
  handleInput();
  moveSnake();

  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    spawnFood();
  }

  render();
  
  int Speed = map(analogRead(Speed_Pin), 0, 1023, 500, 100);
  delay(Speed);
}

void disp(byte d) 
{
  for(byte i=0; i<5; i++) 
  digitalWrite(5-i, bcd_code[d][i]);  
  
}

void handleInput() {
  if (digitalRead(UP) == LOW && direction != 1) direction = 3;
  if (digitalRead(DOWN) == LOW && direction != 3) direction = 1;
  if (digitalRead(LEFT) == LOW && direction != 0) direction = 2;
  if (digitalRead(RIGHT) == LOW && direction != 2) direction = 0;
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  switch (direction) {
    case 0: snakeX[0]++; break;
    case 1: snakeY[0]++; break;
    case 2: snakeX[0]--; break;
    case 3: snakeY[0]--; break;
  }

  if (snakeX[0] < 0) snakeX[0] = 15;
  if (snakeX[0] > 15) snakeX[0] = 0;
  if (snakeY[0] < 0) {
    snakeY[0] = 1;
    //direction = 0;
    //snakeX[0]++;
  }
  if (snakeY[0] > 1) {
    snakeY[0] = 0;
    //direction = 0;
    //snakeX[0]++;
  }
}

void spawnFood() {
  bool valid;
  do {
    valid = true;
    foodX = random(0, 16);
    foodY = random(0, 2);

    for (int i = 0; i < snakeLength; i++) {
      if (foodX == snakeX[i] && foodY == snakeY[i]) {
        valid = false;
        break;
      }
    }
  } while (!valid);
}

void render() {
  lcd.clear();
  for (int i = 0; i < snakeLength; i++) {
    lcd.setCursor(snakeX[i], snakeY[i]);
    lcd.print("*");
  }
  lcd.setCursor(foodX, foodY);
  lcd.print("$");
}