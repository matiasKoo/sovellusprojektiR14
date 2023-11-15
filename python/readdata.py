
import asyncio
from bleak import BleakClient
import logging
from bleak.backends.characteristic import BleakGATTCharacteristic

address = "FD:90:8A:90:23:A8"
SER_NBR_UUID = "00001523-1212-efde-1523-785feabcd123"
CHAR_NBR_UUID = "00001526-1212-efde-1523-785feabcd123"

logger = logging.getLogger(__name__)

def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    """Simple notification handler which prints the data received."""
    print(int.from_bytes(data,byteorder = "little"))

async def main(address):
    async with BleakClient(address) as client:
        print("connected",client.is_connected)
        
        #model_number = await client.read_gatt_char(_NBR_UUID)
        #print(model_number)
        #BleakGATTCharacteristic =client.services.get_characteristic("00001526-1212-efde-1523-785feabcd123")
        #print(BleakGATTCharacteristic.properties)
        

        await client.start_notify(CHAR_NBR_UUID, notification_handler)
        await asyncio.sleep(10.0)
        await client.stop_notify(CHAR_NBR_UUID)
        

asyncio.run(main(address))

