
import asyncio
from bleak import BleakClient
from bleak.backends.characteristic import BleakGATTCharacteristic
import db_write 

address = "FD:90:8A:90:23:A8"
CHAR_NBR_UUID = "00001526-1212-efde-1523-785feabcd123"


data_arr = []
started = 0

def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    """Simple notification handler which prints the data received."""
    global data_arr
    global started
    val = int.from_bytes(data,byteorder = "little")
    print(val)

    if ( val < 6):
        if (started != 0):
            #sql funktio
            db_write.db_write(data_arr[0],data_arr[1],data_arr[2],data_arr[3])
            print(data_arr)
            data_arr.clear()
        else:
            started =1
        
        
        #aloita uusirivi
        data_arr.append(val)
    
    else:
        data_arr.append(val)




async def main(address):
    async with BleakClient(address) as client:

        await client.start_notify(CHAR_NBR_UUID, notification_handler)
        await asyncio.sleep(3.0)
        await client.stop_notify(CHAR_NBR_UUID)
        

asyncio.run(main(address))

