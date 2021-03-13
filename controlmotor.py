"""import lib"""
from machine import Pin, SoftSPI,SPI #SPI Conversation
import time
"""************************************************************"""

# Encode Function
slaveSelectEnc1 = 15

def initEncoder(spi,SSEn1): 
    #Raise select pins
    #Communication begins when you drop the individual select signsl
    SSEn1.value(1)
    #spi.init(baudrate=80000000)
    SSEn1.value(0)                #Begin SPI conversation
    spi.write(bytes(0x88))            #Write to MDR0            //0x88 Check IR Register
    #spi.write(bytes([0x01, 0x02, 0x03, 0x04, 0x05])) # send 5 bytes on the bus
    spi.write(bytes(0x03))           #Configure to 4 byte mode //0x03
    SSEn1.value(1)
    
    
def clearEncoder(spi,SSEn1):
    #Set encoder1's data register to 0
    SSEn1.value(0)
    #Begin SPI conversation
    #Write to DTR
    spi.write(bytes(0x98))
    # Load data
    spi.write(bytes([0x00, 0x00, 0x00, 0x00])) # send 4 bytes on the bus
    #spi.write(bytes(0x00))      # Highest order byte
    #spi.write(bytes(0x00))
    #spi.write(bytes(0x00))
    #spi.write(bytes(0x00))      # lowest order byte
    SSEn1.value(1)          # Terminate SPI conversation
    
    time.sleep_us(100)       # provides some breathing room between SPI conversations
    
    # Set encoder1's current data register to center
    SSEn1.value(0)             # Begin SPI conversation
    spi.write(bytes(0xE0))         # Load value from DTR to CNTR
    SSEn1.value(1)             # Terminate SPI conversation



def readEncoder(spi,SSEn1):
    SSEn1.value(0)#Begin SPI conversation
    spi.write(bytes(0x60))  #0x60
    count = spi.read(3)  # read 4 byte
    SSEn1.value(1)
    #int_val = int.from_bytes(count, "big", signed="True") 
    return count



while(True):
    vspi = SPI(2, baudrate=80000000, polarity=0, phase=0, bits=8, firstbit=SPI.MSB, sck=Pin(18), mosi=Pin(23), miso=Pin(19))
    SSEn1 = Pin(slaveSelectEnc1, Pin.OUT)
    initEncoder(vspi,SSEn1)
    clearEncoder(vspi,SSEn1)
    int_val = int.from_bytes(readEncoder(vspi,SSEn1), "big")
    print(int_val)
    
    


    
    
    
    
    
    
    
    
    
    
    
  
  
  
  