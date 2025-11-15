import requests
import time

from utils.raw_text_to_gif import raw_text_to_gif
from pixoo import ESPtoPixoo


def get_quote():
    response = requests.get(url="https://api.kanye.rest/")
    response.raise_for_status()
    data = response.json()
    return data['quote']

if __name__ == '__main__':
    ESP32_IP = "192.168.1.1"  # Replace with your ESP32 IP
    ESP32_PORT = 80           # Port where the ESP32 HTTP server listens

    file_path = raw_text_to_gif(get_quote(), destination_file_path="scrolling_text.gif")

    pixoo = ESPtoPixoo(ESP32_IP, ESP32_PORT)

    pixoo.set_system_brightness(100)
    time.sleep(1)
    pixoo.draw_gif(file_path)