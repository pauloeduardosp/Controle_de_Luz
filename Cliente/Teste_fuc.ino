void teste_funcao(){

  for(int p = 0; p < 290; p++){
    Serial.print("posicao ");
    Serial.print(p);
    Serial.print(" = ");
    Serial.println(EEPROM.read(p));
  }
}
