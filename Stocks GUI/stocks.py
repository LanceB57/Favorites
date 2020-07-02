from yahooquery import Ticker
import tkinter as tk
from datetime import datetime


class Application(tk.Frame):
    def __init__(self, master):
        super().__init__(master)
        
        self.grid()
        self.read()
        self.createWidgets()
        self.configure(background = "black")

    def read(self):
        fin = open("mystocks.txt", 'r')
        self.watchlist = []
        self.initial = []
        self.initprices = []
        for line in fin:
            line = line.split()
            self.watchlist.append(line[0])
            self.initprices.append(float(line[1]))
            self.initial.append(float(line[2]))
    
    def createWidgets(self):
        self.labels = []
        self.percentages = []
        for i in range(len(self.watchlist)):
            tk.Label(self, text = self.watchlist[i], font = ("Times", 24), fg = "orange", bg = "black").grid(row = 0, column = i)
            self.labels.append(tk.Label(self, text = "", font = ("Times", 24), bg = "black"))
            tk.Label(self, text = "$" + str(self.initial[i]), font = ("Times", 16), fg = "orange", bg = "black").grid(row = 1, column = i)
            self.labels[i].grid(row = 2, column = i)
            self.percentages.append(tk.Label(self, text = "", font = ("Times", 16), bg = "black"))
            self.percentages[i].grid(row = 3, column = i)
        self.total = tk.Label(self, text = "", font = ("Times", 30), bg = "black", fg = "orange")
        self.total.grid(row = 4, column = 0, columnspan = 2)
        self.time = tk.Label(self, text = "", font = ("Times", 14), bg = "black", fg = "orange")
        self.time.grid(row = 4, column = len(self.watchlist) - 1)
        self.update()

    def update(self):
        tickers = Ticker(self.watchlist)
        prices = tickers.price
        currtot = [0] * len(self.watchlist)
        new = [prices[ticker]['regularMarketPrice'] for ticker in tickers.symbols]
        for i in range(len(self.watchlist)):
            self.percentages[i]["text"] = "%.2f%%" % ((new[i] - self.initprices[i]) / self.initprices[i] * 100)
            self.percentages[i]["fg"] = "green" if new[i] >= self.initprices[i] else "red"
            self.labels[i]["fg"] = "green" if new[i] >= self.initprices[i] else "red"
            self.labels[i]["text"] = "$%.2f" % (self.initial[i] / self.initprices[i] * new[i])
            currtot[i] = self.initial[i] / self.initprices[i] * new[i]
        self.total["text"] = "Total: $%.2f" % (sum(currtot))
        now = datetime.now()
        self.time["text"] = now.strftime("%H:%M:%S")
        root.after(5000, self.update)

root = tk.Tk()
root.title("Stocks!!")
root.configure(background = "black")
app = Application(root)
root.mainloop()
