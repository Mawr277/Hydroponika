

class Sensor {
public:
	Sensor();

	virtual void update() = 0; // Odczytaj dane z sensora i nadpisz aktualnie przechowywane wartości
	virtual bool testConnection() = 0; // Metoda do sprawdzenia, czy komunikacja z sensorem działa
	bool connectionErr(); // Sprawdź, czy wystąpił błąd odczytu

protected:

	bool connectionError; // Czy wystąpił błąd odczytu
};
