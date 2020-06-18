# Arduino-GPS-GPRS

# Подключение:
*слева- ноги модуля; справа - куда подключать
*"Source" - источник питания. USB шнур с подпаянными контактами. СЛЕДИ за цветом проводов от шнура. Красный это "+"
*LED GND - короткая ножка

GSM:
VCC(+5V) -> Source+
GND         -> Source-
RX             -> TX1 (Arduino) 
TX             -> RX1 (Arduino) 

GPS:
VCC(+5V) -> Source+
GND         -> Source-
RX             -> TX2 (Arduino) 
TX             -> RX2 (Arduino) 

LED: 
VCC           -> Resistor(47 kOm) -> Source+
GND         -> 2 (Arduino)

Arduino:
RX1           -> TX(GSM)
TX1           -> RX(GSM)
RX2           -> TX(GPS)
TX2           -> RX(GPS)
2               -> GND(LED)
VCC          -> Source+
GND         -> Source-


