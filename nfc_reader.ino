
bool adminMode = false;
unsigned long timeAdminMode;


uid checkIfCardPresent(void)
{
  uid Uid;

  Uid.uidByte[0] = 0;
  Uid.uidByte[1] = 0;
  Uid.uidByte[2] = 0;
  Uid.uidByte[3] = 0;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return Uid;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return Uid;
  }

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  Serial.print("UID: ");
  Serial.println(uid);

  Uid.uidByte[0] = mfrc522.uid.uidByte[0];
  Uid.uidByte[1] = mfrc522.uid.uidByte[1];
  Uid.uidByte[2] = mfrc522.uid.uidByte[2];
  Uid.uidByte[3] = mfrc522.uid.uidByte[3];

  return Uid;
}

void checkNFC(void)
{
  uid Uid;

  Uid = checkIfCardPresent();


  if (adminMode) {

    // Figure out how to handle when tag id is 0000
    for (int tag = 0; tag < 1; ++tag)
    {

      if (
        Uid.uidByte[0] == tags[tag].uid[0] ||
        Uid.uidByte[1] == tags[tag].uid[1] ||
        Uid.uidByte[2] == tags[tag].uid[2] ||
        Uid.uidByte[3] == tags[tag].uid[3])
      {
        tags[tag].credits = 2;
        Serial.println("Tag credits updated");
        delay(1000);
      }

    }
    // TODO add tag not in memory
    // TODO not add admin card

    if (timeNow - timeAdminMode >= 3000)
    {
      adminMode = false;
      Serial.println("End of Admin Mode");
    }

    return;
  }


  if (
    Uid.uidByte[0] == 57 ||
    Uid.uidByte[1] == 111 ||
    Uid.uidByte[2] == 21 ||
    Uid.uidByte[3] == 163
  )
  {
    Serial.println("Admin card");
    Serial.println("Admin Mode");
    adminMode = true;
    timeAdminMode = millis();

    return;
  }


  // Figure out how to handle when tag id is 0000
  for (int tag = 0; tag < 1; ++tag)
  {

    if (
      Uid.uidByte[0] == tags[tag].uid[0] ||
      Uid.uidByte[1] == tags[tag].uid[1] ||
      Uid.uidByte[2] == tags[tag].uid[2] ||
      Uid.uidByte[3] == tags[tag].uid[3])
    {
      Serial.println("Valid card");

      if ( tags[tag].credits > 0 )
      {
        tags[tag].credits--;
        Serial.print("Credits left: ");
        Serial.println(tags[tag].credits);
        vendingCount = 1;
      }
      else
      {
        Serial.println("No credits on card");
        delay(2500);
      }

      // Stop on first found tag
      break;
    }

  }

}
