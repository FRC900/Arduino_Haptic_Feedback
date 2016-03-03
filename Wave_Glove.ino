
int idArray [4] [3] = {{2, 3, 4},
                        {5, 6, 7},
                        {8, 9, 10},
                        {11, 12, 13}};
                        
int clearMotors [4] [3] = {{0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0}};
                        
int topdown [4] [3] = {{1, 1, 1},
                        {0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0}};
                        
int bottomup [4] [3] = {{0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0},
                        {1, 1, 1}};                        
                       
                

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  int i = 2;
  while (i < 14){
    pinMode(i, OUTPUT);
    i++;
  }
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int Mode = Serial.parseInt();
    int Time = Serial.parseInt();

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255 and invert
      if (Mode == 1){
        propogate_wave(topdown, Time);
      } else if (Mode == 2){
        propogate_wave(bottomup, Time);
      }
      
      Serial.print(Mode);
      Serial.print(" ");
      Serial.println(Time);
    }
  }
}

void propogate_wave (int prevStatus [4] [3], int wait){
  update_motors(prevStatus);
  int x = 0;
  int y = 0;
  int newStatus [4] [3];
  int donecount = 0;
  
  while (x < 4){
    y = 0;
    while (y < 3){
      newStatus[x][y] = prevStatus[x][y];
      y++;
    }
    x++;
  }
  x = 0;
  while (x < 4){
    y = 0;
    String printme = "\n " + String(x);
    printme += ": ";
    //Serial.print(printme);
    while (y < 3){
      //Serial.print(String(prevStatus[x][y]) + " ");
      if (prevStatus[x][y] == 1){
        if (prevStatus[x-1][y] == 0){
          newStatus[x-1][y] = 1;
        }
        if (prevStatus[x][y-1] ==0){
          newStatus[x][y-1] = 1;  
        } 
        if (prevStatus[x+1][y] == 0){
          newStatus[x+1][y] = 1;
        } 
        
        if (prevStatus[x][y+1] == 0){
          newStatus[x][y+1] = 1;
        }
        newStatus[x][y] = 2;
      } else if (prevStatus[x][y] == 2){
        donecount++;
      }
      y++;
    }
    x++;
  }
  delay(wait);
  if (donecount < 12){
    propogate_wave(newStatus, wait);
  } else{
    update_motors(clearMotors);
  }
}

void update_motors(int motorStatus [5] [3]){
  //Serial.println("updating motors");
  int x = 0;
  int y = 0;
  while (x < 4){
    y=0;
    while (y < 3){
      if (motorStatus[x][y] == 1){
        digitalWrite(idArray[x][y], HIGH);
      }else{
        digitalWrite(idArray[x][y], LOW);
      }
      y++;
    }
     x++;
   }
   
}  
