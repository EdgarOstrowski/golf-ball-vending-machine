void checkNFC(void)
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {

    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    Serial.println(mfrc522.uid.uidByte[i]);
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  Serial.print("UID: ");
  Serial.println(uid);

  for (int tag = 0; tag <= 2; tag++)
  {
    if ( uid == tags[tag].uid)
    {
      Serial.println("found tag");
    }

  }

  if (uid == "E42F332A")
  {
    Serial.println("Valid card");
    vendingCount = 1;
  }
  else
  {
    Serial.println("Invalid card");
    delay(3000);
  }


}
