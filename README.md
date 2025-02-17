# Wypełnianie Siatki Trójkątów - Powierzchnia Beziera 3-go Stopnia

Repozytorium zawiera projekt, którego celem jest modelowanie oświetlenia i nakładanie tekstur na powierzchni 3D opartej na powierzchni Beziera 3-go stopnia. 
Projekt wczytuje dane z pliku tekstowego przy starcie aplikacji i pozwala na interakcję z powierzchnią w celu obserwacji wpływu różnych parametrów oświetlenia oraz wyświetlania tekstur na tych powierzchniach.

## Opis Projektu

### Wejście
Powierzchnia Beziera 3-go stopnia jest wczytywana z pliku tekstowego zawierającego 16 punktów kontrolnych (po 3 liczby rzeczywiste: x, y, z dla każdego punktu).

### Właściwości
- **Triangulacja powierzchni** – Generowanie siatki trójkątów na podstawie powierzchni Beziera.
- **Obroty powierzchni** – Powierzchnia jest obracana wokół osi Z i X, a użytkownik może modyfikować kąty obrotu za pomocą suwaków (kąty α od -45° do 45° oraz β od 0° do 90°).
- **Wypełnianie trójkątów** – Możliwość rysowania samej siatki trójkątów lub wypełniania ich odpowiednimi kolorami na podstawie modelu oświetlenia.
- **Dokładność triangulacji** – Suwak umożliwiający kontrolowanie dokładności triangulacji powierzchni.
- **Interpolacja kolorów** – Obliczanie koloru wypełnienia w trójkątach za pomocą modelu oświetlenia Lamberta (składowe rozproszone i zwierciadlane).
- **Ruch światła** – Animacja ruchu źródła światła po okręgu na płaszczyźnie (z opcją zatrzymania).
- **Reflektory** – Możliwość włączania/wyłączania reflektorów w 3 podstawowych kolorach (czerwony, zielony, niebieski) oraz zmieniania ich parametru mL za pomocą suwaka (ostatni suwak).
- **Tekstura** – Możliwość wybrania tekstury dla obiektu, która będzie zastosowana do powierzchni Beziera.
- **NormalMap** - Możliwość wczytania mapy wektorów normalnych (NormalMap) oraz modyfikowania wektora normalnego na podstawie tej mapy.

### Algorytmy
- **Obroty** – Obrót powierzchni wokół osi Z i X za pomocą odpowiednich macierzy obrotu.
- **Interpolacja barycentryczna** – Kolor wypełnienia punktu w trójkącie obliczany na podstawie współrzędnych barycentrycznych.
- **Obliczanie koloru** – Kolor obiektu jest obliczany za pomocą wzoru oświetlenia Lamberta i uwzględnia refleksję w zależności od kąta między wektorem normalnym a źródłem światła.

### Oświetlenie
- **Model Lamberta**:
  - Wzór na oświetlenie:  
    `I = kd * IL * IO * cos(∠(N, L)) + ks * IL * IO * cos^m(∠(V, R))`
    Gdzie:
    - `kd` i `ks` to współczynniki dla składowych rozproszonych i zwierciadlanych.
    - `IL` i `IO` to kolory światła i obiektu.
    - `N` to wektor normalny.
    - `L` to wersor do źródła światła.
    - `V` to wersor widza (zwykle [0, 0, 1]).
    - `R` to wektor odbicia światła.
    - `m` to współczynnik opisujący jak bardzo trójkąt jest zwierciadlany.

