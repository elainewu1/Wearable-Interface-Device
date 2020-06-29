/*WHID Controller Program V1
  written by Shawn Heimbigner
  Written for uno rev2 w/ Wifi specifically atmega4809, Max153cpp ADC, and 16 channel mux
  
  Program has1 TCA Timer and 4 TCB Timers, TCB0 at 250ns, TCB1 at 4.5ms, TCB2 at 5ms, TCB3 which is variable length:
  TCA0: Currently unused no prescaler selected.

  TCB has 3 options for prescale 1,2, or match TCA clock
  TCB0: (Prescale 1)controls adc read and write operations cycling every 250ns  with control switches to change function
        additionally resets TCB3 after getting clock cycle value to store in sensor[15], should always be 
        less than 9600 (600us when calculated at 16Mhz no prescaling)
  TCB1: (Prescale 2)cycles twice to account for 9ms requrire to read all 15 sensors and changes values that deactivate
        ADC functions to allow for other functions in 20ms window.
  TCB2: (Prescale 2)cycles 4 times to restart ADC sensor sweep at 20ms intervals
  TCB3: (Prescale 1)Used as cycle counter, value is stored as int denoting number of cycles to steady state. Can be reverse
        engineered on computer side knowing maximum possible value of cycles for individual sensor to determine
        AIMS magnetic effect on circuit IE changes in distance.
  */

int ADCCTRL = 0; //ADC phase:
                 //0) CS,WR = 0, RD = 1
                 //1) CS,WR = 1, RD = 1
                 //2) WR = 0, CS, RD = 0
                 //3) WR,RD,CS = 1
int ADCCkCnt = 0;  //if < 600 CH does not change
int CH = 0;  //incremented channel for mux
int cycl45 = 0; //Adc every 9ms or 2 interrupt cycles turned off
int cycl20ms = 0; //20ms sensor sweep sentinel
int ADCSent = 0; //sentinel turning ADC functions on/off
int CS = 2;
int RD = 3;
int WR = 4;
int Di0 = 5;
int Di1 = 6;
int Di2 = 7;
int Di3 = 8;
int Di4 = 9;
int Di5 = 10;
int Di6 = 11;
int Di7 = 12;
byte val = 0;
volatile int stdystcnt = 0;
volatile int sensors[15]; //sensor clockcycles, max is approx 9600 600us/(1/(16000000))
volatile int i; //index for sensor number
volatile int rst = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(CS, OUTPUT);  //CS used to activate ADC set to zero
  pinMode(RD, OUTPUT);  //Used to read set to zero, CS must also be zero
  pinMode(WR, OUTPUT);  //Used to write voltage to ADC set to zero, CS must also be set to zero
  pinMode(Di0, INPUT);  //LSB
  pinMode(Di1, INPUT);  
  pinMode(Di2, INPUT);
  pinMode(Di3, INPUT);
  pinMode(Di4, INPUT);
  pinMode(Di5, INPUT);
  pinMode(Di6, INPUT);
  pinMode(Di7, INPUT);  //MSB
  pinMode(A1, OUTPUT);  //s0
  pinMode(A2, OUTPUT);  //s1
  pinMode(A3, OUTPUT);  //s2
  pinMode(A4, OUTPUT);  //s3

  noInterrupts();
                       /* TCA TIMER*/
                       /*TCA has 1 timer*/
                       /*CURRENTLY NOT USED*/
  // TCA0.SINGLE.CMP0 = 0x0; /* Compare Register 0: 0x0 */

  // TCA0.SINGLE.CMP1 = 0x0; /* Compare Register 1: 0x0 */

  // TCA0.SINGLE.CMP2 = 0x0; /* Compare Register 2: 0x0 */

  // TCA0.SINGLE.CNT = 0x0; /* Count: 0x0 */

  // TCA0.SINGLE.CTRLB = 0 << TCA_SINGLE_ALUPD_bp /* Auto Lock Update: disabled */
  //     | 0 << TCA_SINGLE_CMP0EN_bp /* Compare 0 Enable: disabled */
  //     | 0 << TCA_SINGLE_CMP1EN_bp /* Compare 1 Enable: disabled */
  //     | 0 << TCA_SINGLE_CMP2EN_bp /* Compare 2 Enable: disabled */
  //     | TCA_SINGLE_WGMODE_NORMAL_gc; /*  */

  // TCA0.SINGLE.CTRLC = 0 << TCA_SINGLE_CMP0OV_bp /* Compare 0 Waveform Output Value: disabled */
  //     | 0 << TCA_SINGLE_CMP1OV_bp /* Compare 1 Waveform Output Value: disabled */
  //     | 0 << TCA_SINGLE_CMP2OV_bp; /* Compare 2 Waveform Output Value: disabled */

  // TCA0.SINGLE.DBGCTRL = 0 << TCA_SINGLE_DBGRUN_bp; /* Debug Run: disabled */

  // TCA0.SINGLE.EVCTRL = 0 << TCA_SINGLE_CNTEI_bp /* Count on Event Input: disabled */
  //     | TCA_SINGLE_EVACT_POSEDGE_gc; /* Count on positive edge event */

  TCA0.SINGLE.INTCTRL = 1 << TCA_SINGLE_CMP0_bp   /* Compare 0 Interrupt: enabled */
                        | 1 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: enabled */
                        | 1 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: enabled */
                        | 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */

  // TCA0.SINGLE.PER = 0xffff; /* Period: 0xffff */

  // TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc /* System Clock */
  //     | 0 << TCA_SINGLE_ENABLE_bp; /* Module Enable: disabled */

                      /* TCB TIMER*/
                      /*TCB has 3 timers*/
                      /*TCB0*/
                      /*250ns timer*/
  TCB0.CCMP = 0x4; /* Compare or Capture: 0x4 */

  TCB0.CNT = 0x0; /* Count: 0x0 */

  // TCB0.CTRLB = 0 << TCB_ASYNC_bp /* Asynchronous Enable: disabled */
  //     | 0 << TCB_CCMPEN_bp /* Pin Output Enable: disabled */
  //     | 0 << TCB_CCMPINIT_bp /* Pin Initial State: disabled */
  //     | TCB_CNTMODE_INT_gc; /* Periodic Interrupt */

  // TCB0.DBGCTRL = 0 << TCB_DBGRUN_bp; /* Debug Run: disabled */

  // TCB0.EVCTRL = 0 << TCB_CAPTEI_bp /* Event Input Enable: disabled */
  //     | 0 << TCB_EDGE_bp /* Event Edge: disabled */
  //     | 0 << TCB_FILTER_bp; /* Input Capture Noise Cancellation Filter: disabled */

  TCB0.INTCTRL = 1 << TCB_CAPT_bp; /* Capture or Timeout: enabled */

  TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc  /* CLK_PER (No Prescaling) */
               | 1 << TCB_ENABLE_bp   /* Enable: enabled */
               | 0 << TCB_RUNSTDBY_bp /* Run Standby: disabled */
               | 0 << TCB_SYNCUPD_bp; /* Synchronize Update: disabled */

                          /*TCB1*/
                          /*4.5ms timer*/
  TCB1.CCMP = 36000; /* Compare or Capture: 0x9c40 */

  TCB1.CNT = 0x0; /* Count: 0x0 */

  // TCB1.CTRLB = 0 << TCB_ASYNC_bp /* Asynchronous Enable: disabled */
  //     | 0 << TCB_CCMPEN_bp /* Pin Output Enable: disabled */
  //     | 0 << TCB_CCMPINIT_bp /* Pin Initial State: disabled */
  //     | TCB_CNTMODE_INT_gc; /* Periodic Interrupt */

  // TCB1.DBGCTRL = 0 << TCB_DBGRUN_bp; /* Debug Run: disabled */

  // TCB1.EVCTRL = 0 << TCB_CAPTEI_bp /* Event Input Enable: disabled */
  //     | 0 << TCB_EDGE_bp /* Event Edge: disabled */
  //     | 0 << TCB_FILTER_bp; /* Input Capture Noise Cancellation Filter: disabled */

  TCB1.INTCTRL = 1 << TCB_CAPT_bp; /* Capture or Timeout: enabled */

  TCB1.CTRLA = TCB_CLKSEL_CLKDIV2_gc  /* CLK_PER/2 (From Prescaler) */
               | 1 << TCB_ENABLE_bp   /* Enable: enabled */
               | 0 << TCB_RUNSTDBY_bp /* Run Standby: disabled */
               | 0 << TCB_SYNCUPD_bp; /* Synchronize Update: disabled */

                          /*TCB2*/
                          /*5 ms timer*/
  TCB2.CCMP = 40000; /* Compare or Capture: 0x0 */

  TCB2.CNT = 0x0; /* Count: 0x0 */

  // TCB2.CTRLB = 0 << TCB_ASYNC_bp /* Asynchronous Enable: disabled */
  //     | 0 << TCB_CCMPEN_bp /* Pin Output Enable: disabled */
  //     | 0 << TCB_CCMPINIT_bp /* Pin Initial State: disabled */
  //     | TCB_CNTMODE_INT_gc; /* Periodic Interrupt */

  // TCB2.DBGCTRL = 0 << TCB_DBGRUN_bp; /* Debug Run: disabled */

  // TCB2.EVCTRL = 0 << TCB_CAPTEI_bp /* Event Input Enable: disabled */
  //     | 0 << TCB_EDGE_bp /* Event Edge: disabled */
  //     | 0 << TCB_FILTER_bp; /* Input Capture Noise Cancellation Filter: disabled */

  TCB2.INTCTRL = 1 << TCB_CAPT_bp; /* Capture or Timeout: disabled */

  TCB2.CTRLA = TCB_CLKSEL_CLKDIV2_gc /* CLK_PER (No Prescaling) */
           | 1 << TCB_ENABLE_bp /* Enable: disabled */
           | 0 << TCB_RUNSTDBY_bp /* Run Standby: disabled */
           | 0 << TCB_SYNCUPD_bp; /* Synchronize Update: disabled */

                        /*TCB3*/
                        /*600us timer*/
  TCB3.CCMP = 9600; /* Compare or Capture: 0x0 */

  TCB3.CNT = 0x0; /* Count: 0x0 */

  // TCB3.CTRLB = 0 << TCB_ASYNC_bp /* Asynchronous Enable: disabled */
  //     | 0 << TCB_CCMPEN_bp /* Pin Output Enable: disabled */
  //     | 0 << TCB_CCMPINIT_bp /* Pin Initial State: disabled */
  //     | TCB_CNTMODE_INT_gc; /* Periodic Interrupt */

  // TCB3.DBGCTRL = 0 << TCB_DBGRUN_bp; /* Debug Run: disabled */

  // TCB3.EVCTRL = 0 << TCB_CAPTEI_bp /* Event Input Enable: disabled */
  //     | 0 << TCB_EDGE_bp /* Event Edge: disabled */
  //     | 0 << TCB_FILTER_bp; /* Input Capture Noise Cancellation Filter: disabled */

  // TCB3.INTCTRL = 0 << TCB_CAPT_bp; /* Capture or Timeout: disabled */

  TCB3.CTRLA = TCB_CLKSEL_CLKDIV1_gc /* CLK_PER (No Prescaling) */
           | 1 << TCB_ENABLE_bp /* Enable: disabled */
           | 0 << TCB_RUNSTDBY_bp /* Run Standby: disabled */
           | 0 << TCB_SYNCUPD_bp; /* Synchronize Update: disabled */
  interrupts();

}

ISR(TCA0_OVF_vect) {
  /* Insert your TCA overflow interrupt handling code */

  /* The interrupt flag has to be cleared manually */
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR(TCA0_CMP0_vect) {
  /* Insert your TCA Compare 0 Interrupt handling code here */

  /* The interrupt flag has to be cleared manually */
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCA0_CMP1_vect) {
  /* Insert your TCA Compare 1 Interrupt handling code here */

  /* The interrupt flag has to be cleared manually */
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP1_bm;
}

ISR(TCA0_CMP2_vect) {
  /* Insert your TCA Compare 2 Interrupt handling code here */

  /* The interrupt flag has to be cleared manually */
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP2_bm;
}
//TCB0 happens every 250ns
ISR(TCB0_INT_vect) {
  /* Insert your TCB interrupt handling code */
  //if in 9ms window for sensor sweep read sensors
  if (ADCSent == 1) {
       if (rst == 0) {
          rst = 1;
          TCB3.CNT = 0x0;
       }
       analogWrite(A0, 255); //turn on sensor analog output

      //If on first part of adc single read CS,WR go to zero to initiate adc write for 250ns
      if(ADCCTRL == 0) {
        digitalWrite(CS, LOW);
        digitalWrite(WR, LOW);
        ADCCTRL++;
      }
      //if on second part of adc single read CS,WR go high for 250ns to allow stability
      else if (ADCCTRL == 1) {
        digitalWrite(CS, HIGH);
        digitalWrite(WR, HIGH);
        ADCCTRL++;
      }
      //if on third part of adc single read CS,RD go low to read adc value and values stored
      else if (ADCCTRL == 2) {
        digitalWrite(CS, LOW);
        digitalWrite(RD, LOW);
        if (stdystcnt < 5) {
          val = 0;
          val += digitalRead(Di7);
          val << 1;
          val += digitalRead(Di6);
          val << 1;
          val += digitalRead(Di5);
          val << 1;
          val += digitalRead(Di4);
          val << 1;
          val += digitalRead(Di3);
          val << 1;
          val += digitalRead(Di2);
          val << 1;
          val += digitalRead(Di1);
          val << 1;
          val += digitalRead(Di0);
          //if val within percent start counting number of reads, reset if exceeds 
          //if greater than number of reads desired steady state reached
          //1.6V expected approximate (1.6/3.3)*256 = 124 (Vin/Vref)*(maxsize of bits)
          //approximated at 1.58 to 1.65 (+/- 2%)
          if ( val <= 128 && val >= 122) {
            stdystcnt += 1; //increment steady state count must be greater than 5
            if (stdystcnt >=5) {
              sensors[i] = TCB3.CNT;
              if (i < 15) {
                i++;
              }
              else {
                i = 0;
              }
            }
          }
          else {
            stdystcnt = 0;
          }
        }
        ADCCTRL++;
      }
      //if on last phase of adc single read WR,CS,RD all high to reset adc for 250ns completing 1ms cycle
      //ADC 
      else {
        digitalWrite(CS, HIGH);
        digitalWrite(RD, HIGH);
        ADCCTRL = 0;
        ADCCkCnt++;
       //if hasn't been read 600 times stay on current channel
       //if has been read 600 times channel increments to next
       //channel switch should initiat impulse response
        if (ADCCkCnt >= 599) {
          if (CH <=15) {
            CH++;
            ADCCkCnt = 0;
            stdystcnt = 0;
            rst = 0;
          }
          else {
            CH = 0; // reset channel after 15 reached (16th sensor read)
            rst = 0;
          }
        }
      }
    }
    else {
      analogWrite(A0, 0); //Turn off sensor analog output when not in use
    }
  /**
   * The interrupt flag is cleared by writing 1 to it, or when the Capture register
   * is read in Capture mode
   */
  TCB0.INTFLAGS = TCB_CAPT_bm;
}
//TCB1 happens every 4.5ms 2 cycles turns off ADC functionality
ISR(TCB1_INT_vect) {
  /* Insert your TCB interrupt handling code */
  /*If 20ms cycle initiated cnt 4.5ms cycles, after 2 turn off ADC*/
    if (cycl45 < 2){
      ADCSent = 1;
      cycl45++;
    }
    else {
      ADCSent = 0;
    }
  /**
   * The interrupt flag is cleared by writing 1 to it, or when the Capture register
   * is read in Capture mode
   */
  TCB1.INTFLAGS = TCB_CAPT_bm;
}

//TCB1 happens every 5ms 4 cycles starts sensor sweep
ISR(TCB2_INT_vect) {
  /* Insert your TCB interrupt handling code */
  /*If 20ms cycle initiated cnt 4.5ms cycles, after 2 turn off ADC*/
    if (cycl20ms == 0) {
      cycl45 = 0;
    }
    if (cycl20ms <= 3){
     cycl20ms++;
    }
    if (cycl20ms == 3) {
      cycl20ms = 0;
    }
  /**
   * The interrupt flag is cleared by writing 1 to it, or when the Capture register
   * is read in Capture mode
   */
  TCB2.INTFLAGS = TCB_CAPT_bm;
}



void loop() {
  // put your main code here, to run repeatedly:

  
  if (ADCSent == 0) {
    //Do any other function due to sensor sweep not being on
  }
    //ADC is runninign only ADC functions should be in this section
  else {
      //mux controller
        switch (CH) {
          case 0:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 1:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, HIGH);   //s0      
            break;
          case 2:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 3:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, HIGH);   //s0
            break;
          case 4:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 5:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, HIGH);   //s0
            break;
          case 6:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 7:
            digitalWrite(A4, LOW);    //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, HIGH);   //s0
            break;
          case 8:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 9:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, HIGH);   //s0
            break;
          case 10:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 11:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, LOW);    //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, HIGH);   //s0
            break;
          case 12:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 13:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, LOW);    //s1
            digitalWrite(A1, HIGH);   //s0
            break;
          case 14:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, LOW);    //s0
            break;
          case 15:
            digitalWrite(A4, HIGH);   //s3
            digitalWrite(A3, HIGH);   //s2
            digitalWrite(A2, HIGH);   //s1
            digitalWrite(A1, HIGH);   //s0
            break;
         }
  }
}
