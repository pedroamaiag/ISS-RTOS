import tkinter as tk
import serial  
from iss_logic import ISSLogic

RFC2217_URL = "rfc2217://localhost:4000"
BAUDRATE = 9600

class CyberInterface:
    def __init__(self, root):
        self.logic = ISSLogic()
        self.root = root
        self.root.title("ISS MISSION CONTROL - UFC")
        self.root.geometry("500x420")
        self.root.configure(bg="black")

        self.ultima_cidade = None

        try:
            self.ser = serial.serial_for_url(
                RFC2217_URL,
                baudrate=BAUDRATE,
                timeout=1
            )
            print(f"Conectado ao Kernel via {RFC2217_URL} (baud={BAUDRATE})")
        except Exception as e:
            print(f"Erro ao conectar no RFC2217: {e}")
            self.ser = None

        tk.Label(
            root, text="MONITORAMENTO GLOBAL ISS",
            font=("Impact", 22),
            fg="white", bg="black"
        ).pack(pady=20)

        self.lbl_lat = tk.Label(
            root, text="LATITUDE: --.----",
            font=("Consolas", 14),
            fg="white", bg="black"
        )
        self.lbl_lat.pack()

        self.lbl_lon = tk.Label(
            root, text="LONGITUDE: --.----",
            font=("Consolas", 14),
            fg="white", bg="black"
        )
        self.lbl_lon.pack()

        tk.Label(
            root, text="CIDADE ATUAL:",
            font=("Consolas", 10),
            fg="gray", bg="black"
        ).pack(pady=(25, 0))

        self.lbl_local = tk.Label(
            root, text="BUSCANDO...",
            font=("Consolas", 16, "bold"),
            fg="#00ff00", bg="black"
        )
        self.lbl_local.pack(pady=10)

        self.btn_alert = tk.Button(
            root, text="TESTAR LED",
            bg="#ff0000", fg="white",
            width=12, height=2,
            borderwidth=0,
            command=self.enviar_alerta_kernel
        )
        self.btn_alert.pack(pady=25)

        self.lbl_status = tk.Label(
            root, text="SISTEMA OPERACIONAL: PRONTO",
            font=("Consolas", 8),
            fg="gray", bg="black"
        )
        self.lbl_status.pack(side="bottom", pady=10)

        self.monitoramento_continuo()

    def enviar_alerta_kernel(self):
        if self.ser and getattr(self.ser, "is_open", False):
            try:
                self.ser.write(b"A")
                self.ser.flush()
                print("Sinal 'A' enviado para o Kernel!")
                self.lbl_status.config(text="SINAL ENVIADO PARA O KERNEL", fg="#ff5555")
            except Exception as e:
                print(f"Erro ao escrever na serial: {e}")
        else:
            print("Erro: conexão serial não disponível.")
            self.lbl_status.config(text="ERRO: SERIAL NÃO DISPONÍVEL", fg="#ff5555")

    def monitoramento_continuo(self):
        coords = self.logic.capturar_coordenadas()

        if coords:
            lat, lon = coords["lat"], coords["lon"]
            self.lbl_lat.config(text=f"LATITUDE: {lat:.4f}")
            self.lbl_lon.config(text=f"LONGITUDE: {lon:.4f}")

            cidade_atual = self.logic.identificar_localizacao(lat, lon)
            self.lbl_local.config(text=cidade_atual.upper())

            if cidade_atual != "OCEANO / AREA REMOTA":
                if cidade_atual != self.ultima_cidade:
                    self.enviar_alerta_kernel()
                    self.ultima_cidade = cidade_atual
                    self.lbl_status.config(text=f"MUDANÇA DETECTADA: {cidade_atual}", fg="#00ff00")
            else:
                self.ultima_cidade = cidade_atual

        self.root.after(4000, self.monitoramento_continuo)

root = tk.Tk()
app = CyberInterface(root)
root.mainloop()
