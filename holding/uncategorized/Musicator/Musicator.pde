/*

 * VUmeter / Light organ (Full db presentation)
 * Horizontal presentation - center-out linear display
 * (c) Copyright 2009 QS@quantsuff.com
 * All Rights Reserved
 
 *
 
 * LED Matrix row:1-5; col:1-6
 * Map PORTB == D8:D12 - pin[row+7] : +v
 * PORTD == D2:D7 - pin[8-col] ; Gnd
 * Our output: col::D2:D7 -ve (LOW) while row::D8:D13 +ve
 
 */

char msg[] = {
  0x11, 0x0a, 0x0e, 0x0e, 0x0a, 0x11, // Starburst
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
  0x11, 0x19, 0x1e, 0x00, 0x1f, 0x10, // Q U
  0x1f, 0x00, 0x1e, 0x05, 0x1e, 0x00, // A
  0x1f, 0x06, 0x0c, 0x1f, 0x00, 0x03, // N T
  0x1f, 0x03, 0x00, 0x12, 0x15, 0x09, // S
  0x00, 0x1f, 0x10, 0x1f, 0x00, 0x1f, // U F
  0x05, 0x05, 0x00, 0x1f, 0x05, 0x05, // F
  0,0,0,0,0,0 }; //Plus an overrun buffer

// bit pattern, 1 byte per column. Top = lsb

char display[6], hdisp[6] ;

int w10kbuf[2], w1kbuf[20], w100buf[100]; // Assume all zeroed (:P)
int w10kidx, w1kidx, w100idx;
// int w10kmax, w5kmax, w1kmax, w500max, w100max;
long w, w10ksum=0, w1ksum=0, w100sum=0;
int www, w10k, w1k, w100, meter[5] ;

int scale=3; // log scale (start at 23)
int recal=0; //AGC rate
int inPin=1 ; // analogRead on A01

void setup() // run once, when the sketch starts
{
  for (int ledPin=2; ledPin<=12; ledPin++) {
    pinMode(ledPin, OUTPUT); // sets the digital pin as output
    digitalWrite(ledPin,(ledPin<=7)) ; //Disable all cols
  }
  lmpWrite(msg,0,15); //Cycle delay is 1/10 secs
  // (it's really to initialize the counters)
  analogReference(INTERNAL) ; // Do not use DEFAULT or EXTERNAL!
  int i= digitalRead(inPin); // remove old data
}

void loop() // run over and over again
{
  w100 = (w100sum / 23) >> scale; // A cheap filter @ app 100Hz
  w1k = abs((w1ksum / 3)-w100) >> scale; // 1kHz
  w10k = (www+w10ksum) >> scale; // 10kHz
  w= (w100 + w1k + w10k) ; // Total power (VU function)

  if (w<13) {
    recal++;
    if (recal>=15) { //350mS counts before recalibration of meter
      recal=0;
      scale-= (scale>3);
    } 
  }// increase res only if less than 1/2 scale

  // if ((w100>33) || (w1k>30) || (w10k>33)) { // log2
  if ((w100+w1k+w10k)>83) { // Peaked: make it less sensitive
    recal=0;
    scale++;
    w100= w100>>1;
    w1k = w1k >>1;
    w10k = w10k >>1;
  }
  meter[0]= min(w100>>1,31);
  meter[1]= min(abs(w100+w1k-w10k)>>2,31);
  meter[2]= min(w1k>>2,31);
  meter[3]= min(abs(w1k+w10k-w100)>>2,31);
  meter[4]= min((w10k*5)>>3,31);

  for (int col=0;col<6;col++) {
    int c=0;
    for (int mm=0; mm<=4; mm++) {
      c= c<<1;
      c+= (meter[mm] > col);
    }
    hdisp[col]= c;
  }
  display[0]= hdisp[4];
  display[1]= hdisp[2];
  display[2]= hdisp[0];
  display[3]= hdisp[0];
  display[4]= hdisp[2];
  display[5]= hdisp[4];

  lmpWrite(display,0,1);

  display[0]= hdisp[5];
  display[1]= hdisp[3];
  display[2]= hdisp[1];
  display[3]= hdisp[1];
  display[4]= hdisp[3];
  display[5]= hdisp[5];

  /* Vertical display routine
   *
   fillVBar(display,0,w100);
   fillVBar(display,1,abs(w100+w1k-w10k));
   fillVBar(display,2,w1k);
   fillVBar(display,3,abs(w1k+w10k-w100));
   fillVBar(display,4,w10k);
   //fillVBar(display,5,w10k>>1);
   //fillVBar (display,5,scale+1);
   int i=0;
   int s=scale-1;
   for (int ii=0; ii<5; ii++) {
   i= i | (s & 1);
   i= i<<1;
   s= s>>1;
   }
   display[5]= i;
   *
   */

  lmpWrite(display,0,1);
}

void fillVBar(char disp[],int o, int val) {
  int j= B100000; // digit pattern
  val= constrain(val,2,127) ;
  while (val>2) {
    j|= (j>>1) ;
    val= val>>1 ;
  }
  disp[o]=j;
}

void lmpWrite(char disp[],int stchar,int steps)
/* Usage: Loads bit pattern in disp[], starting at stchar,
 
 * 1 byte per column, left-to-right, top-to-bottom
 * Map PORTB == D8:D12 - pin[row+7] : +v
 * PORTD == D2:D7 - pin[8-col] ; Gnd
 
 */
{
  int col, row, cval, lit=0, ww=0;
  long delayStart=millis() + 90*(constrain(steps,1,100)) ;
  // Display / loop time
  www= 0 ;
  do {
    for (col=0; col<6; col++) {
      cval = disp[col+stchar];
      for (row=1; row<=5; row++) { //only 5 lower bits
        if (cval & 1) { // light this LED
          if (lit==0)
            digitalWrite(7-col,LOW); //col needs activating
          digitalWrite(row+7,HIGH);
          lit++ ; // in case we need to know how many LEDs are lit
        }
        // Cycle stealing here, to read VU input
        ww=max(ww,analogRead(inPin)) ; //Raw volume data (rectified)
        www=max(ww,www) ; //Peak retect
        delayMicroseconds(80); //This makes sure the panel is
        // lit the same period of time.
        digitalWrite(row+7,LOW); //Turn LED off
        cval = cval>>1 ; // check the next bit
      }
      digitalWrite(7-col,HIGH); // turn col OFF
      w10ksum+= www-w10kbuf[w10kidx]; // This is our 'capacitor'
      w10kbuf[w10kidx]=www;
      w10kidx++;
      w10kidx= ((w10kidx<1)); // 0 or 1
      //if (w10kidx>3) w10kidx=0;
      //w10ksum=ww ; // Increase the high-end response

      w1ksum+= ww-w1kbuf[w1kidx];
      w1kbuf[w1kidx]=ww;
      w1kidx++;
      if (w1kidx>19) w1kidx=0;
      //w1kidx= ((w1kidx<19) && (w1kidx++)); // 0 - 19

      w100sum+= ww-w100buf[w100idx];
      w100buf[w100idx]=ww;
      w100idx++;
      if (w100idx>99) w100idx=0;
      //w100idx= ((w100idx<1) && (w100idx++)); // 0 - 199

      lit = 0;
    }
  }
  while (delayStart > millis()) ;
}

