/*
  RPC mempory sharing header.
  Simple system for sharine memory between the M7/M4 portenta cores.

*/
#ifndef rpcMemory
#define rpcMemory

#include "RPC_internal.h"

class RPCmem
{
  public:
    RPCmem() {};
    void begin_(char signalChar = 'x', int mode = 0, int ptr = 0) { // rx = 0, tx = 1
      _mode = mode;
      _sigChar = signalChar;
      _ptr = ptr;
    };


    void update() {
      
      // read messege rpc
      int i = 0;
      while (RPC1.available()) {
        _buf[i] =  RPC1.read();
        i++;
      }
      
      char inf0 = _buf[0];
      char inf1 = _buf[1];

      // reciver rx
      if (_mode == 0) {

        if (inf0 == _sigChar) {
          if ( inf1 == '1') {
            _rx = 1;
          }
        }
        if (inf0 == 'a') {
          if ( inf1 == _sigChar) {
            for (int q = 0; q < i - 2; q++) {
              _adr[q] = _buf[q + 2];
              _adr_rx = 1;
              Serial.println("Sent: " + String(_sigChar) + String('0'));
              RPC1.print(String(_sigChar) + String('0'));
            }
          }

        }

        if (_adr_rx == 0) {
          RPC1.print(String('a') + String(_sigChar));
          Serial.println("Request sent! ");

        }

      }

      // transmitter tx
      if (_mode == 1) {
        if (inf0 == _sigChar) {
          if ( inf1 == '0') {
            _tx = 0;
          }
        }
        if (inf0 == 'a') {
          if ( inf1 == _sigChar) {
            RPC1.print(String('a') + String(_sigChar) + String(_ptr));
            _adr_tx = 1;
          }
        }


      }

    };

    char* getBuf() {
      return _buf;
    };

    char* getAdr() {
      return _adr;
    };

    bool readyToRead() {

      if (_rx == 1 and _adr_rx == 1) {
        return true;
      }
      else
      {
        return false;
      }
    }
    bool readyToWrite() {

      if (_tx == 0) {
        return true;
      }
      else
      {
        return false;
      }
    }

    void markAsRead() {
      if (_rx == 1 and _adr_rx == 1) {
        _rx = 0;
        RPC1.print(String(_sigChar) + String('0'));
      }
    }
    void markAsReady() {
      if (_tx == 0) {
        _tx = 1;
        RPC1.print(String(_sigChar) + String('1'));
      }
    }


    // private: will be private in final versions
    int _ptr;
    char _sigChar;
    int _mode = 0;
    int _adr_tx = 0;
    int _adr_rx = 0;
    int _rx = 0;
    int _tx = 0;
    char _adr[16];
    char _buf[16];
};

#endif
