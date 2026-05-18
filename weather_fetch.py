"""
wf.py
-----------------
Fetches current weather + air quality for a city from OpenWeatherMap
and writes a tiny weather.txt file that the C++ simulation reads in
live mode.

Output (weather.txt) format -- one key=value per line:
    rain=<0 or 1>      # 1 if it is currently raining
    aqi=<0..500>       # Air Quality Index estimate
    wind=<int m/s>     # wind speed
"""

import urllib.request
import urllib.parse
import json
import time
import sys

# ---- CONFIG --
API_KEY = "Your API"
CITY    = "Dhaka"
COUNTRY = "BD"
POLL_SECONDS = 30               # Data Pull Frequency(s)

OWM_WEATHER = "https://api.openweathermap.org/data/2.5/weather"
OWM_AIR     = "https://api.openweathermap.org/data/2.5/air_pollution"
OWM_GEO     = "https://api.openweathermap.org/geo/1.0/direct"

# OpenWeatherMap air_pollution returns AQI on a 1..5 scale.
# Map roughly to US-EPA AQI numbers the C++ side expects (0..500).
OWM_AQI_TO_US = {1: 30, 2: 80, 3: 130, 4: 200, 5: 320}


def fetch_json(url, params):
    full = url + "?" + urllib.parse.urlencode(params)
    with urllib.request.urlopen(full, timeout=10) as resp:
        return json.loads(resp.read().decode("utf-8"))


def geocode(city, country, key):
    data = fetch_json(OWM_GEO, {"q": f"{city},{country}", "limit": 1, "appid": key})
    if not data:
        raise RuntimeError("city not found")
    return data[0]["lat"], data[0]["lon"]


def fetch_once(lat, lon, key):
    w = fetch_json(OWM_WEATHER, {"lat": lat, "lon": lon, "appid": key, "units": "metric"})
    a = fetch_json(OWM_AIR,     {"lat": lat, "lon": lon, "appid": key})

    # Rain flag: weather.id codes 200..531 are thunderstorm/drizzle/rain
    code = (w.get("weather") or [{}])[0].get("id", 800)
    rain = 1 if 200 <= code < 600 else 0

    wind = int(round(w.get("wind", {}).get("speed", 0.0)))   # m/s

    owm_aqi = a["list"][0]["main"]["aqi"]            # 1..5
    aqi = OWM_AQI_TO_US.get(owm_aqi, 50)

    return rain, aqi, wind


def write_file(rain, aqi, wind):
    with open("weather.txt", "w") as f:
        f.write(f"rain={rain}\n")
        f.write(f"aqi={aqi}\n")
        f.write(f"wind={wind}\n")


def write_fallback():
    # Writes a clear-weather default if the API call fail.
    with open("weather.txt", "w") as f:
        f.write("rain=0\n")
        f.write("aqi=50\n")
        f.write("wind=5\n")


def main():
    if API_KEY == "YOUR_API_KEY_HERE":
        print("ERROR: edit weather_fetch.py and set API_KEY first.")
        sys.exit(1)

    try:
        lat, lon = geocode(CITY, COUNTRY, API_KEY)
    except Exception as e:
        print("Geocoding failed:", e)
        write_fallback()
        return

    print(f"Polling weather for {CITY},{COUNTRY}  (lat={lat:.3f}, lon={lon:.3f})")
    while True:
        try:
            rain, aqi, wind = fetch_once(lat, lon, API_KEY)
            write_file(rain, aqi, wind)
            print(f"[{time.strftime('%H:%M:%S')}] rain={rain} aqi={aqi} wind={wind}")
        except Exception as e:
            print("Fetch failed:", e)
            write_fallback()

        if POLL_SECONDS <= 0:
            break
        time.sleep(POLL_SECONDS)


if __name__ == "__main__":
    main()
