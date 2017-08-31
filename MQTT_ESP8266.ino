#include <Arduino.h>
typedef struct lengthOf{
  uint8_t MSB;
  uint8_t LSB;
  };
 
typedef struct Connect{
  uint8_t connectionHeader;
  char* protocolName;
  lengthOf protocolLength;
  uint8_t protocolLevel;
  uint8_t flagBits;
  lengthOf keepAliveConn;
  char* clientId;
  lengthOf clientIdLength;
  char* username;
  lengthOf usernameLength;
  char* password;
  lengthOf passwordLength;
  };

typedef struct Publish{
  uint8_t publishHeader;
  char* topic;
  lengthOf topicLength;
  char* message;
  };
  
typedef struct Subscibe{
  uint8_t suscribeHeader;
  lengthOf packetID;
  char* topic;
  lengthOf topicLength;
  uint8_t QoS;
  };

uint8_t getConnectFlag(uint8_t usernameFlag, uint8_t passwordFlag, uint8_t willRetain, uint8_t willQoS, uint8_t willFlag, uint8_t cleanSession)
{
  uint8_t Flag = 0x00;
  if (usernameFlag) Flag |= (1<<7);
  if (passwordFlag) Flag |= (1<<6);
  if (willRetain)   Flag |= (1<<5);
  if (willQoS)    Flag |= (1<<4) | (1<<3);
  if (willFlag)   Flag |= (1<<2);
  if (cleanSession) Flag |= (1<<1);
  return Flag;
}

char* getProtocolName(char* protocol)
{
  return protocol;
}
uint8_t getMSB(char* string)
{
  return ((strlen(string) & 0x00FF));
}

uint8_t getLSB(char* string)
{
  return ((strlen(string) & 0xFF00));
}

uint16_t getConnectPacketLength(Connect* packet)//Pass address of the Connect
{
  return(sizeof(packet->protocolLength)
  + strlen(packet->protocolName)
  + sizeof(packet->protocolLevel)
  + sizeof(packet->flagBits)
  + sizeof(packet->keepAliveConn)
  + sizeof(packet->clientIdLength)
  + strlen(packet->clientId)
  + sizeof(packet->usernameLength)
  + strlen(packet->username)
  + sizeof(packet->passwordLength)
  + strlen(packet->password)
  );
}
uint8_t getPublishPacketLength(Publish* packet)
{
  return(sizeof(packet->topicLength)
  + strlen(packet->topic)
  + strlen(packet->message)
  );
}

uint8_t getSuscribePacketLength(Subscibe* packet)
{
  return(sizeof(packet->packetID)
  + sizeof(packet->topicLength)
  + strlen(packet->topic)
  + sizeof(packet->QoS)
  );
}
void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("AT+RST");
  delay(6000);
  Serial.println("AT+CIPMUX=1");
  delay(1000);
  Serial.println("AT+CIPSTART=1,\"TCP\",\"m11.cloudmqtt.com\",11926,7200");
  delay(1000);

  /***PACKET CONFIGURATIONS***/
  Connect Conn;
  Conn.connectionHeader = 0x10;
  Conn.protocolName = "MQIsdp";
  Conn.protocolLength.MSB = getMSB(Conn.protocolName);
  Conn.protocolLength.LSB = getLSB(Conn.protocolName);
  Conn.protocolLevel = 0x03;//Level 3
  Conn.flagBits = getConnectFlag(1,1,0,0,0,1);//Username, password, Clean Session
  Conn.keepAliveConn.MSB = getMSB(60);//Keep Connection On for 60 Secs
  Conn.keepAliveConn.LSB = getLSB(60);
  Conn.clientId = "JN0001";
  Conn.clientIdLength.MSB = getMSB(Conn.clientId);
  Conn.clientIdLength.LSB = getLSB(Conn.clientId);
  Conn.username = "abivspak";
  Conn.usernameLength.MSB = getMSB(Conn.username);
  Conn.usernameLength.LSB = getLSB(Conn.username);
  Conn.password = "qPmfW0oYYfEX";
  Conn.passwordLength.MSB = getMSB(Conn.password);
  Conn.passwordLength.LSB = getLSB(Conn.password);
  
  Publish Pub;
  Pub.publishHeader = 0x30;
  Pub.topic = "EPSUM";
  Pub.topicLength.MSB = getMSB(Pub.topic);
  Pub.topicLength.LSB = getLSB(Pub.topic);
  Pub.message = "Hello JN! From Arduino";

  Subscibe Sub;
  Sub.suscribeHeader = 0x82;
  Sub.packetID.MSB = getMSB(1);
  Sub.packetID.LSB = getLSB(1);
  Sub.topic = "EPSUM";
  Sub.topicLength.MSB = getMSB(Sub.topic);
  Sub.topicLength.LSB = getLSB(Sub.topic);
  Sub.QoS = 0x00;
  
  /***Setup ESP8266***/
  /*Serial.println("AT+CWMODE=1");
  Serial.println("AT+CWJAP_DEF=\"EPSUMLABS\",\"Hello@EpsumLabs\"");
  delay(1000*10);*/

  /*******************/
  /**********************CONNECT MQTT PACKET**********************/
  /*Connect Conn;
  Conn.connectionHeader = 0x10;
  Conn.protocolName = "MQIsdp";
  Conn.protocolLength.MSB = getMSB(Conn.protocolName);
  Conn.protocolLength.LSB = getLSB(Conn.protocolName);
  Conn.protocolLevel = 0x03;//Level 3
  Conn.flagBits = getConnectFlag(1,1,0,0,0,1);//Username, password, Clean Session
  Conn.keepAliveConn.MSB = getMSB(60);//Keep Connection On for 60 Secs
  Conn.keepAliveConn.LSB = getLSB(60);
  Conn.clientId = "JN0001";
  Conn.clientIdLength.MSB = getMSB(Conn.clientId);
  Conn.clientIdLength.LSB = getLSB(Conn.clientId);
  Conn.username = "abivspak";
  Conn.usernameLength.MSB = getMSB(Conn.username);
  Conn.usernameLength.LSB = getLSB(Conn.username);
  Conn.password = "qPmfW0oYYfEX";
  Conn.passwordLength.MSB = getMSB(Conn.password);
  Conn.passwordLength.LSB = getLSB(Conn.password);*/
  /***************************************************************/
  
  /**************************PUBLISH MQTT PACKET******************/
/*
  Publish pub;
  pub.publishHeader = 0x30;
  pub.topic = "JN";
  pub.topicLength.MSB = getMSB(pub.topic);
  pub.topicLength.LSB = getLSB(pub.topic);
  pub.message = "Hello JN!";
  Serial.write(pub.publishHeader);
  Serial.write(getPublishPacketLength(&pub));
  Serial.write(pub.topicLength.LSB);
  Serial.write(pub.topicLength.MSB);
  Serial.write(pub.topic);
  Serial.write(pub.message);*/
  /****************************************************************/
  
  /****************************Suscribe MQTT Packet******************/
 /* Subscibe Sub;
  Sub.suscribeHeader = 0x82;
  Sub.packetID.MSB = getMSB(1);
  Sub.packetID.LSB = getLSB(1);
  Sub.topic = "JN";
  Sub.topicLength.MSB = getMSB(Sub.topic);
  Sub.topicLength.LSB = getLSB(Sub.topic);
  Sub.QoS = 0x00;
  Serial.write(Sub.suscribeHeader);
  Serial.write(getSuscribePacketLength(&Sub));
  Serial.write(Sub.packetID.LSB);
  Serial.write(Sub.packetID.MSB);
  Serial.write(Sub.topicLength.LSB);
  Serial.write(Sub.topicLength.MSB);
  Serial.write(Sub.topic);
  Serial.write(Sub.QoS);*/
  /******************************************************************/

  Serial.print("AT+CIPSEND=1,");
  Serial.println(getConnectPacketLength(&Conn) + getPublishPacketLength(&Pub) + getSuscribePacketLength(&Sub) + 6);
  delay(1000);
  Serial.write(Conn.connectionHeader);
  Serial.write(getConnectPacketLength(&Conn));//Shot Out this error
  Serial.write(Conn.protocolLength.LSB);
  Serial.write(Conn.protocolLength.MSB);
  Serial.write(Conn.protocolName);
  Serial.write(Conn.protocolLevel);
  Serial.write(Conn.flagBits);
  Serial.write(Conn.keepAliveConn.LSB);
  Serial.write(Conn.keepAliveConn.MSB);
  Serial.write(Conn.clientIdLength.LSB);
  Serial.write(Conn.clientIdLength.MSB);
  Serial.write(Conn.clientId);
  Serial.write(Conn.usernameLength.LSB);
  Serial.write(Conn.usernameLength.MSB);
  Serial.write(Conn.username);
  Serial.write(Conn.passwordLength.LSB);
  Serial.write(Conn.passwordLength.MSB);
  Serial.write(Conn.password);

  Serial.write(Pub.publishHeader);
  Serial.write(getPublishPacketLength(&Pub));
  Serial.write(Pub.topicLength.LSB);
  Serial.write(Pub.topicLength.MSB);
  Serial.write(Pub.topic);
  Serial.write(Pub.message);

  Serial.write(Sub.suscribeHeader);
  Serial.write(getSuscribePacketLength(&Sub));
  Serial.write(Sub.packetID.LSB);
  Serial.write(Sub.packetID.MSB);
  Serial.write(Sub.topicLength.LSB);
  Serial.write(Sub.topicLength.MSB);
  Serial.write(Sub.topic);
  Serial.write(Sub.QoS);
}

void loop() {
  
}
