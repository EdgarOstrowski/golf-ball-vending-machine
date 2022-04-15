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
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  Serial.print("UID: ");
  Serial.println(uid);

  for (int tag = 0; tag <= 2; tag++)
  {
    if (
      mfrc522.uid.uidByte[0] == tags[tag].uid[0] ||
      mfrc522.uid.uidByte[1] == tags[tag].uid[1] ||
      mfrc522.uid.uidByte[2] == tags[tag].uid[2] ||
      mfrc522.uid.uidByte[3] == tags[tag].uid[3])
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
    
    /*
    else
    {
      Serial.println("Invalid card");
      delay(3000);
    }
    */

  }


}
