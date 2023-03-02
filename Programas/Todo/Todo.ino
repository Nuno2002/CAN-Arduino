#include <SPI.h>
#include <mcp2515.h>

#define DEBUG(a) Serial.println(a)

struct can_frame canLectura;
struct can_frame canEscribe;
MCP2515 mcp2515(10);

/*
 * uint16_t
 * CanH = a/256  (division entera)
 * CanL = a%256  (resto)
 */

void setup()
{
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  delay(1000);
}

int esperaIntSerial()
{
  int a;
  while(true)
  {
    a = Serial.parseInt();
    if (a > 0)
    {
      break;
    }
  }
  return(a);
}

int entrada(int i)
{
  Serial.println();
  Serial.print("\tIntroduce el byte ");
  Serial.print(i);
  Serial.print(": ");
  int a = esperaIntSerial();
  Serial.print(a);

  return(a);
}

void escritura()
{
  int id = Serial.parseInt();
  //DEBUG(id);
  canEscribe.can_id = id;

  Serial.print("\tIntroduce el tamaño del mensaje (DLC): ");
  int dlc = esperaIntSerial();
  Serial.println(dlc);
  //DEBUG(dlc);
  canEscribe.can_dlc = dlc;
  
  for(int i=0; i < dlc; i++){
    canEscribe.data[i] = entrada(i);
    //DEBUG(entrada(i));
  }
  
  mcp2515.sendMessage(&canEscribe);

  Serial.println("\nMensaje creado\n\n\n");
/*
  Serial.println(canEscribe.can_id);
  Serial.println(canEscribe.can_dlc);
  for(int i=0; i < dlc; i++)
    Serial.println(canEscribe.data[i]);
*/
  Serial.read();
}

void lectura()
{
  Serial.println("------- CAN Read ----------");
  Serial.println("ID \t DLC \t  DATA");

  Serial.print(canLectura.can_id, HEX); // print ID
  Serial.print("\t");
  
  Serial.print(canLectura.can_dlc, HEX); // print DLC
  Serial.print("\t");
    
  for (int i = 0; i<canLectura.can_dlc; i++)  {  // print the data
    Serial.print(canLectura.data[i],HEX);
    Serial.print(" ");
  }
  Serial.println(); 
}

void loop()
{
  Serial.println("Introduce la ID o espera a que se reciba un mensaje para leerlo");

  while (true){
    if (mcp2515.readMessage(&canLectura) == MCP2515::ERROR_OK)
    {
      Serial.println("Leyendo...");
      lectura();
      break;
    }

    if (Serial.available())
    {
      Serial.println("Creando mensaje...");
      escritura();
      break;
    }
  }
}
