#ifdef CORE_CM7

#include"rpcMemory.h"
#include "RPC_internal.h"

int rx_int;
int rx_p = (int) &rx_int;
int rx_buffer[1600];
int start = micros();

RPCmem sharedMem;

void setup() {
  RPC1.begin();
  sharedMem.begin_('b', 0);

  LL_RCC_ForceCM4Boot();
  Serial.begin(2000000);
}

void loop() {
  
  /// these are printed everyt loop hence they will only update when the following if statement is called (every 4 loops or so)
  Serial.println("==== M4 to M7 ====");
  Serial.println("Buffer: "+ String(sharedMem._buf));
  Serial.println("Address: " + String(sharedMem._adr));
  Serial.println(sharedMem._tx);
  Serial.println(sharedMem._rx);
  Serial.println(rx_buffer[0]);
  
  start = micros();
  sharedMem.update();

  if (sharedMem.readyToRead()) {

    rx_p = String(sharedMem.getAdr()).toInt();
    int *p = (int*) rx_p;

    for (int i  = 0; i < 1600; i++) {
      rx_buffer[i] = *(p + i);
    }
//    for (int i  = 0; i < 16; i++) {
//      Serial.println(rx_buffer[i]);
//    }
    sharedMem.markAsRead();
  }
  
  Serial.println("Loop time (us): " + String(micros()-start));
  
  delay(10);
}

#endif


#ifdef CORE_CM4

#include"rpcMemory.h"
#include "RPC_internal.h"

RPCmem sharedMem;

int tx_buffer[1600];
int p = (int) &tx_buffer;


void setup() {
  RPC1.begin();
  sharedMem.begin_('b', 1, p);
}

void loop() {
  if (sharedMem.readyToWrite()) {
    
    for (int i  = 0; i < 1600; i++) {
      tx_buffer[i] = rand() % 10;
    }
    
    sharedMem.markAsReady();
  }

  sharedMem.update();

  delay(40);
}

#endif
