// The program
unsigned char card[] = {};

// Stack n User Ram
unsigned char Stack[128] = {0};
unsigned char URam[512] = {0};
int sp = 0;
int pc = 0;

void PushStack(unsigned char number) {
  if(sp < 128) Stack[sp++] = number;
}

unsigned char PopStack() {
  if(sp > 0) return Stack[--sp];
  return 0;
}

// Registers n Flags
unsigned char A=0, B=0, C=0, D=0;
unsigned char SR=0;

void EvaluateA(unsigned char b, unsigned char o=0) {
  if(A == 0) SR |= 0x01; else SR &= ~0x01;
  if(A + b > 255) SR |= 0x02; else SR &= ~0x02;
  if(o==1 && A < b) SR |= 0x04; else SR &= ~0x04;
}

unsigned char IsBitSet(unsigned char bytevalue, int pos) {
  unsigned char mask = (1<<pos);
  if((bytevalue & mask) != 0) return 1;
  return 0;
}

void setup() {
  unsigned char run = 1;
  Serial.begin(9600);
  delay(500);

  Serial.println("--- ArduBios v1.0 ---");

  while(run) {
    switch(card[pc]) {
      case 0x00: {
        pc++;
        break;
      }
      
      case 0x01: {
        Serial.print((char)PopStack());
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
        pc++;
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
      
      case 0xFF: {
        Serial.println("\nhalt");
        run = 0;
        break;
      }
      default: { break; }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
