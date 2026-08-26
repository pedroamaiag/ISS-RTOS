import requests
import reverse_geocoder as rg

class ISSLogic:
    def __init__(self):
        self.api_url = "http://api.open-notify.org/iss-now.json"

    def capturar_coordenadas(self):
        try:
            response = requests.get(self.api_url, timeout=5)
            if response.status_code == 200:
                data = response.json()
                return {
                    "lat": float(data["iss_position"]["latitude"]),
                    "lon": float(data["iss_position"]["longitude"])
                }
        except Exception as e:
            print(f"Erro ao capturar dados: {e}")
            return None

    def identificar_localizacao(self, lat, lon):
        try:
            resultado = rg.search((lat, lon), verbose=False)
            if resultado:
                cidade = resultado[0]['name']
                pais = resultado[0]['cc']
                return f"{cidade}, {pais}"
        except:
            return "OCEANO / AREA REMOTA"
        return "LOCAL DESCONHECIDO"
