// Code Making By Suvojit Rana
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10  // SDA pin of RC522
#define RST_PIN 9  // RST pin of RC522

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::Uid masterUID;

bool masterCardSet = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Scan any RFID card to set as MASTER CARD...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  Serial.print("Card UID: ");
  printUID(rfid.uid);

  if (!masterCardSet) {
    masterUID = rfid.uid;  // Save the first scanned card as master
    masterCardSet = true;
    Serial.println("Master card set successfully!");
  } else {
    if (isMaster(rfid.uid)) {
      Serial.println("✅ Master card detected!");
    } else {
      Serial.println("⛔ Not a master card.");
    }
  }

  rfid.PICC_HaltA(); // Stop reading
  rfid.PCD_StopCrypto1(); // Stop encryption
  delay(1500); // Delay before next read
}

void printUID(MFRC522::Uid uid) {
  for (byte i = 0; i < uid.size; i++) {
    Serial.print(uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(uid.uidByte[i], HEX);
  }
  Serial.println();
}

bool isMaster(MFRC522::Uid uid) {
  if (uid.size != masterUID.size) return false;

  for (byte i = 0; i < uid.size; i++) {
    if (uid.uidByte[i] != masterUID.uidByte[i]) return false;
  }
  return true;
}
