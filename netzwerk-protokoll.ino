#include "Arduino.h"
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;

struct Address : public vector<uint16_t> {};

struct Match {
  int positive;
  int negative;
};

Match match(const Address &a1, const Address &a2) {
  size_t maxLen = max(a1.size(), a2.size());
  size_t minLen = min(a1.size(), a2.size());
  Match m = { 0, 0 };

  while (m.positive < minLen && a1[m.positive] == a2[m.positive]) {
    m.positive++;
  }

  m.negative = maxLen - 1 - m.positive;
  return m;
}

struct Connection {
  Address address;
  uint8_t pin;
};

struct Pocket {
  Address address;
  char data[11] = "HELLOWORLD";
  uint16_t checksum;

  void computeCheckSum() {
    checksum = 0;
    for (uint8_t i = 0; i < sizeof(data); i++) {
      checksum ^= data[i];
    }
  }

  Pocket(Address a, const char *d) {
    address = a;
    strncpy(data, d, 10);
    data[sizeof(data) - 1] = '\0';
    computeCheckSum();
  }
};

struct Node {
  vector<Connection> connections;
  Address you;

  void send(Pocket p) {
    Match m = { 0, 0 };
    vector<Connection> sendConnections = connections.at(0);
    Connection sendConnection = connections.at(0);

    for (const auto &connection : connections) {
      Match currentMatch = match(connection.address, p.address);
      if (m.positive <= currentMatch.positive) {
        if (m.positive < currentMatch.positive) {
          sendConnections.clear();
        }
        m = currentMatch;
        sendConnections.push_back(connection);
      }
    }

    m = match(sendConnection.at(0).address, p.address);

    for (const auto &goodConnection : sendConnections) {
      Match currentMatch = match(connection.address, p.address);
      if (currentMatch.negative <= m.negative) {
        sendConnection = goodConnection;
        m = currentMatch;
      }
    }

    Serial.print("Sending data via pin ");
    Serial.println(sendConnection.pin);
  }

  void recieve(Pocket p) {
    if (match(you, p.address).negative == 0) {
      Serial.println("Data Recieved: " + String(p.data));
      return;
    }
    send(p);
  }
};

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // Create a Node and set its own address.
  Node node;
  node.you.push_back(1);
  node.you.push_back(2);
  node.you.push_back(3);

  // Create a Pocket with the same address to trigger the "data received" branch.
  Address pocketAddress;
  pocketAddress.push_back(1);
  pocketAddress.push_back(2);
  pocketAddress.push_back(3);
  Pocket p(pocketAddress, "HELLOWORLD");

  // Call recieve on node with the created Pocket.
  node.recieve(p);
}

void loop() {}
