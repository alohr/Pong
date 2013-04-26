#include "Pong.h"

#include "LedControl.h"

static const int DATA_PIN = 12;
static const int CLK_PIN  = 11;
static const int CS_PIN   = 10;

static const int PADDLE0_PIN = 0;
static const int PADDLE1_PIN = 1;

static char pongchars[][8] = {
    { 0b00000000, 0b01111100, 0b01000010, 0b01000010,   // P
      0b01111100, 0b01000000, 0b01000000, 0b00000000, },
    { 0b00000000, 0b00111100, 0b01000010, 0b01000010,   // O
      0b01000010, 0b01000010, 0b00111100, 0b00000000, },
    { 0b00000000, 0b01000010, 0b01100010, 0b01010010,   // N
      0b01001010, 0b01000110, 0b01000010, 0b00000000, },
    { 0b00000000, 0b00111100, 0b01000000, 0b01000000,   // G
      0b01111100, 0b01000010, 0b00111100, 0b00000000, },
};

static char digit[][8] = {
    { 0b0000, 0b0111, 0b0101, 0b0101, 0b0101, 0b0111, 0b0000, 0b0000 },
    { 0b0000, 0b0001, 0b0001, 0b0001, 0b0001, 0b0001, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0001, 0b0111, 0b0100, 0b0111, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0001, 0b0111, 0b0001, 0b0111, 0b0000, 0b0000 },
    { 0b0000, 0b0101, 0b0101, 0b0111, 0b0001, 0b0001, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0100, 0b0111, 0b0001, 0b0111, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0100, 0b0111, 0b0101, 0b0111, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0001, 0b0001, 0b0001, 0b0001, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0101, 0b0111, 0b0101, 0b0111, 0b0000, 0b0000 },
    { 0b0000, 0b0111, 0b0101, 0b0111, 0b0001, 0b0111, 0b0000, 0b0000 },
};

class LedMatrixPong : public Pong
{
public:
    LedMatrixPong(LedControl& ledControl, int paddle0_pin, int paddle1_pin) :
	ledControl_(ledControl),
	Pong(ROWS, COLS, paddle0_pin, paddle1_pin) {}

    void showPaddle(Paddle& p) { setPaddle(p, true); }
    void hidePaddle(Paddle& p) { setPaddle(p, false); }

    void showBall(Ball& b) { ledControl_.setLed(0, b.row(), b.col(), true); }
    void hideBall(Ball& b) { ledControl_.setLed(0, b.row(), b.col(), false); }

    void startGame();
    void showScore();

private:
    static const int ROWS = 8;
    static const int COLS = 8;

    void sayPong(int delay_ms);
    void setPaddle(const Paddle& p, bool state);

    LedControl& ledControl_;
};

LedControl ledControl(DATA_PIN, CLK_PIN, CS_PIN, 1);
LedMatrixPong pong(ledControl, PADDLE0_PIN, PADDLE1_PIN);


void LedMatrixPong::startGame()
{
    ledControl.clearDisplay(0);
    sayPong(500);

    for (int row = MAX_ROW; row >= 0; row--) {
	ledControl.setRow(0, row, 0xff);
	delay(100);
	ledControl.setRow(0, row, 0);
    }

    start();
}

void LedMatrixPong::sayPong(int delay_ms)
{
    for (int c = 0; c < 4; c++) {
	for (int r = 0; r <= MAX_ROW; r++) {
	    ledControl_.setRow(0, r, pongchars[c][r]);
	}
	delay(delay_ms);
    }
}

void LedMatrixPong::showScore()
{
    ledControl.clearDisplay(0);

    for (int row = 0; row <= MAX_ROW; row++) {
	ledControl.setRow(0, row, 0xff);
	delay(100);
	ledControl.setRow(0, row, 0);
    }
 
    for (int row = 1; row < 6; row++) {
	int value = (digit[score_[0]][row] << 5) | digit[score_[1]][row];
	if (row == 3) {
	    value |= 0b00011000;
	}
	ledControl.setRow(0, row, value);
    }

    delay(1500);

    for (int row = MAX_ROW; row >= 0; row--) {
	ledControl.setRow(0, row, 0xff);
	delay(100);
	ledControl.setRow(0, row, 0);
    }
}

void LedMatrixPong::setPaddle(const Paddle& p, bool state)
{
    ledControl_.setLed(0, p.row(), p.col(), state);
    ledControl_.setLed(0, p.row() + 1, p.col(), state);
}

void setup()
{
    ledControl.shutdown(0, false);
    ledControl.setIntensity(0, 12);
    ledControl.clearDisplay(0);
    randomSeed(analogRead(0));

    pong.startGame();
}

void loop()
{
    pong.update();
}

// Local variables:
// mode: c++
// End:
