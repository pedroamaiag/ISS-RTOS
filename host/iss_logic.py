import math
import requests
import reverse_geocoder as rg

MAX_CITY_DISTANCE_KM = 150

class ISSLogic:
    def __init__(self):
        self.api_url = "https://api.wheretheiss.at/v1/satellites/25544"

    def capturar_coordenadas(self):
        try:
            response = requests.get(self.api_url, timeout=5)
            if response.status_code == 200:
                data = response.json()
                return {
                    "lat": float(data["latitude"]),
                    "lon": float(data["longitude"])
                }
        except Exception as e:
            print(f"Erro ao capturar dados: {e}")
            return None

    def _calcular_distancia_km(self, lat1, lon1, lat2, lon2):
        raio_terra_km = 6371
        lat1_rad, lon1_rad = math.radians(lat1), math.radians(lon1)
        lat2_rad, lon2_rad = math.radians(lat2), math.radians(lon2)

        delta_lat = lat2_rad - lat1_rad
        delta_lon = lon2_rad - lon1_rad
        a = (math.sin(delta_lat / 2) ** 2 +
             math.cos(lat1_rad) * math.cos(lat2_rad) * math.sin(delta_lon / 2) ** 2)
        return raio_terra_km * 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

    def identificar_localizacao(self, lat, lon):
        try:
            resultado = rg.search((lat, lon), verbose=False)
            if resultado:
                cidade = resultado[0]['name']
                pais = resultado[0]['cc']
                latitude_cidade = float(resultado[0]['lat'])
                longitude_cidade = float(resultado[0]['lon'])
                distancia_km = self._calcular_distancia_km(
                    lat, lon, latitude_cidade, longitude_cidade
                )

                if distancia_km > MAX_CITY_DISTANCE_KM:
                    return "OCEANO / AREA REMOTA"

                return f"{cidade}, {pais}"
        except Exception as e:
            return "OCEANO / AREA REMOTA"
        return "LOCAL DESCONHECIDO"
