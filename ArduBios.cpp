#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned char card[] = {};

// Stack n User Ram
unsigned char Stack[128] = {0};
unsigned char CStack[128] = {0};
unsigned char URam[512] = {0};
int sp = 0;
int cp = 0;
int pc = 0;

void PushStack(unsigned char number) {
  if(sp < 128) Stack[sp++] = number;
}

void PushCall(unsigned char number) {
  if(cp < 128) CStack[cp++] = number;
}

unsigned char PopStack() {
  if(sp > 0) return Stack[--sp];
  return 0;
}

unsigned char PopCall() {
  if(cp > 0) return CStack[--cp];
  return 0;
}

// Registers n Flags
unsigned char A=0, B=0, C=0, D=0;
unsigned char SR=0;

void EvaluateA(unsigned char b, unsigned char o=0) {
  unsigned char result = A - b;

  if(result == 0) SR |= 0x01; else SR &= ~0x01;

  if(A + b > 255) SR |= 0x02; else SR &= ~0x02;

  if(o==1 && A < b) SR |= 0x04; else SR &= ~0x04;
}

unsigned char IsBitSet(unsigned char bytevalue, int pos) {
  unsigned char mask = (1<<pos);
  if((bytevalue & mask) != 0) return 1;
  return 0;
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  unsigned char run = 1;
  Serial.begin(9600);
  delay(500);

  Serial.println("--- ArduBios v3.0 ---");

  while(run) {
    switch(card[pc]) {
      case 0x00: {
        pc++;
        break;
      }
      
      case 0x01: {
        char c = (char)PopStack();
        lcd.print(c);
        Serial.print(c);
        pc++;
        break;
      }
      case 0x02: {
        PushStack(card[pc+1]);
        pc+=2;
        break;
      }
      case 0x03: {
        PushStack(A);
        pc++;
        break;
      }
      case 0x04: {
        PushStack(B);
        pc++;
        break;
      }
      case 0x05: {
        PushStack(C);
        pc++;
        break;
      }
      case 0x06: {
        PushStack(D);
        pc++;
        break;
      }
      case 0x07: {
        pinMode(13, OUTPUT);
        tone(13, 1000);
        delay(100);
        noTone(13);
        pc++;
        break;
      }
      case 0x08: {
        lcd.clear();
        pc++;
        break;
      }

      case 0x10: {
        URam[card[pc+1]] = card[pc+2];
        pc+=3;
        break;
      }
      case 0x11: {
        PushStack(URam[card[pc+1]]);
        pc+=2;
        break;
      }
      case 0x12: {
        URam[card[pc+1]] = PopStack();
        pc+=2;
        break;
      }

      case 0x20: {
        A = card[pc+1];
        pc+=2;
        break;
      }
      case 0x21: {
        B = card[pc+1];
        pc+=2;
        break;
      }
      case 0x22: {
        C = card[pc+1];
        pc+=2;
        break;
      }
      case 0x23: {
        D = card[pc+1];
        pc+=2;
        break;
      }
      case 0x24: {
        A = PopStack();
        pc++;
        break;
      }
      case 0x25: {
        B = PopStack();
        pc++;
        break;
      }
      case 0x26: {
        C = PopStack();
        pc++;
        break;
      }
      case 0x27: {
        D = PopStack();
        pc++;
        break;
      }

      case 0x30: {
        A += card[pc+1];
        EvaluateA(card[pc+1]);
        pc+=2;
        break;
      }
      case 0x31: {
        A += B;
        EvaluateA(B);
        pc++;
        break;
      }
      case 0x32: {
        A += C;
        EvaluateA(C);
        pc++;
        break;
      }
      case 0x33: {
        A += D;
        EvaluateA(D);
        pc++;
        break;
      }
      case 0x34: {
        A -= card[pc+1];
        EvaluateA(card[pc+1], 1);
        pc+=2;
        break;
      }
      case 0x35: {
        A -= B;
        EvaluateA(B, 1);
        pc++;
        break;
      }
      case 0x36: {
        A -= C;
        EvaluateA(C, 1);
        pc++;
        break;
      }
      case 0x37: {
        A -= D;
        EvaluateA(D, 1);
        pc++;
        break;
      }

      case 0x40: {
        pc = card[pc+1];
        break;
      }
      case 0x41: {
        if(IsBitSet(SR, 0)) pc = card[pc+1];
        else pc+=2;
        break;
      }
      case 0x42: {
        PushCall(pc + 2);
        pc = card[pc+1];
        break;
      }
      case 0x43: {
        pc = PopCall();
        break;
      }

      case 0x50: {
        EvaluateA(card[pc+1], 1);
        pc+=2;
        break;
      }
      case 0x51: {
        EvaluateA(B, 1);
        pc++;
        break;
      }
      case 0x52: {
        EvaluateA(C, 1);
        pc++;
        break;
      }
      case 0x53: {
        EvaluateA(D, 1);
        pc++;
        break;
      }

      case 0x60: {
          delay(card[pc+1]);
          pc += 2;
          break;
      }

      case 0xFF: {
        Serial.println("\nhalt");
        run = 0;
        break;
      }
      default: { 
        Serial.print("Erro: Opcode desconhecido: "); 
        Serial.println(card[pc]);
        pc++; 
        break; 
      }
    }
  }
}

void loop() {
}
