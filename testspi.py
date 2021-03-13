from machine import SPI,SoftSPI,Pin

# configure the SPI master @ 2MHz
# this uses the SPI non-default pins for CLK, MOSI and MISO (``P19``, ``P20`` and ``P21``)
spi = SPI(2, baudrate=80000000, polarity=0, phase=0, bits=8, firstbit=SPI.MSB, sck=Pin(18), mosi=Pin(23), miso=Pin(19))
spi.write(bytes([0x01, 0x02, 0x03, 0x04, 0x05])) # send 5 bytes on the bus
spi.read(5) # receive 5 bytes on the bus
rbuf = bytearray(5)
spi.write_readinto(bytes([0x01, 0x02, 0x03, 0x04, 0x05]), rbuf) # send a receive 5 bytes
int_val = int.from_bytes(spi.read(5), "big")
print(int_val)
print(spi.read(5))