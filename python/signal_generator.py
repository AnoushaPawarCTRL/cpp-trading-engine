import requests
import random
import time

API_URL = "http://localhost:3000/signal"

while True:
    signal = random.choice(["BUY", "SELL"])
    qty = random.randint(1, 100)
    price = round(random.uniform(95, 105), 2)

    payload = {
        "signal": signal,
        "qty": qty,
        "price": price
    }

    try:
        requests.post(API_URL, json=payload)
        print(f"Sent {signal} {qty} @ {price}")
    except Exception as e:
        print("API Error:", e)

    time.sleep(0.01)  # 100 signals/sec