/*
 *  
 * Monitoria via Telnet
 *  
 */
void telnet(void){

if (!TelnetClient) {  // otherwise it works only once
    TelnetClient = TelnetServer.available();
    if (TelnetClient.connected()) {
      TelnetClient.println("Telnet Test");
     }
  }

}
