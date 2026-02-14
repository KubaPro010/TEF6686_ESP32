import serial
import struct
import time

class I2CPCClient:
    def __init__(self, port, baudrate=115200, timeout=1):
        self.ser = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        self.ser.write(b"~/")
        self.ser.flush()
        while not ((d := self.ser.read_all()) and b"\x01\xff" in d): pass

    def _send_packet(self, payload: bytes):
        length = len(payload)
        self.ser.write(bytes([length]) + payload)

        # Read response length
        resp_len_raw = self.ser.read(1)
        if not resp_len_raw:
            raise TimeoutError("Error")

        resp_len = resp_len_raw[0]
        response = self.ser.read(resp_len)

        if len(response) != resp_len: raise TimeoutError("Incomplete response")

        return response        

    def set_clock(self, clock_hz: int):
        payload = bytes([0]) + struct.pack(">I", clock_hz)
        return self._send_packet(payload)

    def write_i2c(self, addr: int, data: bytes):
        payload = bytes([1, addr]) + data
        return self._send_packet(payload)

    def write_read_i2c(self, addr: int, write_data: bytes, read_len: int):
        payload = bytes([2, addr, len(write_data)]) + write_data + bytes([read_len])
        return self._send_packet(payload)

    def version(self): return self._send_packet(bytes([4]))

    def quit(self): return self._send_packet(bytes([3]))

    def reboot(self): return self._send_packet(bytes([5]))

    def set_baudrate(self, baud: int):
        payload = bytes([6]) + struct.pack(">I", baud)
        out = self._send_packet(payload)
        self.ser.baudrate = baud
        return out

    def close(self):
        self.ser.close()